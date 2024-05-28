
#include "pch.h"
#include "platform_detection.h"
#include "platform.h"
#include "prime/core/assert.h"
#include "prime/core/events.h"

#ifdef P_WINDOWS
#include <GLFW/glfw3.h>

namespace prime {

	static void GLFWErrorCallback(int error, const char* description)
	{
		P_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static void SetCallbacks(GLFWwindow* windowHandle)
	{
		glfwSetWindowSizeCallback(windowHandle, [](GLFWwindow* window, int width, int height)
			{
				Platform::Data& data = *(Platform::Data*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				P_INFO("resize callback");
			});

		glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* window)
			{
				P_INFO("close callback");
			});

		glfwSetKeyCallback(windowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Platform::Data& data = *(Platform::Data*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					P_INFO("key pressed callback");
					break;
				}
				case GLFW_RELEASE:
				{
					P_INFO("key released callback");
					break;
				}
				case GLFW_REPEAT:
				{
					P_INFO("key repeat callback");
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(windowHandle, [](GLFWwindow* window, int button, int action, int mods)
			{
				Platform::Data& data = *(Platform::Data*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					P_INFO("mouse pressed callback");
					break;
				}
				case GLFW_RELEASE:
				{
					P_INFO("mouse released callback");
					break;
				}
				}
			});

		glfwSetScrollCallback(windowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				P_INFO("scroll callback");
			});

		glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* window, double xPos, double yPos)
			{
				P_INFO("cursor callback");
			});
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
		SetCallbacks((GLFWwindow*)m_windowHandle);
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
