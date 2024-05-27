
#include "pch.h"
#include "gl_rendererAPI.h"
#include "prime/platform/platform_detection.h"
#include "prime/core/assert.h"

#include <glad/glad.h>
#ifdef P_WINDOWS
#include <GLFW/glfw3.h>
#endif // P_WINDOWS

namespace prime {

	void GLRendererAPI::Init(void* windowHandle)
	{
#ifdef P_WINDOWS
		glfwMakeContextCurrent((GLFWwindow*)windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		P_ASSERT_MSG(status, "Failed to initialize Glad!");
#endif // P_WINDOWS
	}

	void GLRendererAPI::Shutdown()
	{

	}

	void GLRendererAPI::SetClearColor()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}

	void GLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void GLRendererAPI::SwapBuffers(void* windowHandle)
	{
		glfwSwapBuffers((GLFWwindow*)windowHandle);
	}
}