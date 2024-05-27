
#include "pch.h"
#include "renderer.h"
#include "rendererAPI.h"

namespace prime {

	Renderer::API Renderer::s_API;
	static RendererAPI* s_rendererAPI = nullptr;
	static void* s_windowHandle = nullptr;

	static RendererAPI* CreateAPI()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			break;
		}
		return nullptr;
	}

	void Renderer::SetClearColor()
	{
		s_rendererAPI->SetClearColor();
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