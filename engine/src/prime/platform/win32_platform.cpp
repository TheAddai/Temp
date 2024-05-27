
#include "pch.h"
#include "platform_detection.h"
#include "platform.h"
#include "prime/core/assert.h"

#ifdef P_WINDOWS

#include <GLFW/glfw3.h>

namespace prime {

	static void GLFWErrorCallback(int error, const char* description)
	{
		P_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	void Platform::Init(const std::string& title, ui32 width, ui32 height, b8 fullScreen)
	{
		i8 status = glfwInit();
		P_ASSERT_MSG(status, "Glfw failed to initialize");
		glfwSetErrorCallback(GLFWErrorCallback);

		m_data.title = title;
		m_data.width = width;
		m_data.height = height;
		m_data.fullScreen = fullScreen;

		if (fullScreen) { glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); }
		m_windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer((GLFWwindow*)m_windowHandle, &m_data);
	}

	void Platform::Shutdown()
	{
		glfwDestroyWindow((GLFWwindow*)m_windowHandle);
		glfwSetErrorCallback(NULL);
		glfwTerminate();
	}

	void Platform::Update()
	{
		glfwPollEvents();
	}
}

#endif // P_WINDOWS
