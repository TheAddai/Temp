
#include "pch.h"
#include "renderer.h"
#include "rendererAPI.h"

// openGL
#include "gl_rendererAPI.h"

namespace prime {

	Renderer::API Renderer::s_API;
	static RendererAPI* s_rendererAPI = nullptr;
	static void* s_windowHandle = nullptr;

	struct Data
	{
		glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	static Data s_data;

	static RendererAPI* CreateAPI()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return new GLRendererAPI();
			break;
		}
		return nullptr;
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_data.ClearColor = color;
		s_rendererAPI->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		s_rendererAPI->Clear();
	}

	void Renderer::Init(void* windowHandle)
	{
		s_windowHandle = windowHandle;
		s_rendererAPI = CreateAPI();
		s_rendererAPI->Init(windowHandle);
	}

	void Renderer::Shutdown()
	{
		s_rendererAPI->Shutdown();
		delete s_rendererAPI;
		s_rendererAPI = nullptr;
	}
	
	void Renderer::SwapBuffers()
	{
		s_rendererAPI->SwapBuffers(s_windowHandle);
	}
}