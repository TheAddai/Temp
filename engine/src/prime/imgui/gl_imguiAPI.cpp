
#include "pch.h"
#include "gl_imguiAPI.h"
#include "prime/core/engine.h"

#include <imgui/imgui.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#ifdef P_WINDOWS
#include <GLFW/glfw3.h>
#endif // P_WINDOWS

namespace prime {

	void GLImGuiAPI::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

#ifdef P_WINDOWS
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Engine::GetWindowHandle(), true);
#endif // P_WINDOWS
		ImGui_ImplOpenGL3_Init("#version 400");
	}

	void GLImGuiAPI::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
#ifdef P_WINDOWS
		ImGui_ImplGlfw_Shutdown();
#endif // P_WINDOWS
		ImGui::DestroyContext();
	}

	void GLImGuiAPI::BeginRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
#ifdef P_WINDOWS
		ImGui_ImplGlfw_NewFrame();
#endif // P_WINDOWS
		ImGui::NewFrame();
	}

	void GLImGuiAPI::EndRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((f32)Engine::GetWindowWidth(), (f32)Engine::GetWindowHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
#ifdef P_WINDOWS
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
#endif // P_WINDOWS
		}
	}
}
