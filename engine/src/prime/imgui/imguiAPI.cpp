
#include "pch.h"
#include "imguiAPI.h"
#include "prime/renderer/renderer.h"

// openGL
#include "gl_imguiAPI.h"

namespace prime {

	ImGuiAPI* prime::ImGuiAPI::Create()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return new GLImGuiAPI();
			break;
		}
		return nullptr;
	}
}
