
#include "pch.h"
#include "platform_detection.h"
#include "platform.h"
#include "prime/core/assert.h"
#include "prime/core/events.h"
#include "prime/core/Dispatcher.h"

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

				Dispatcher::Get().enqueue<WindowResizeEvent>(width, height);
			});

		glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* window)
			{
				Dispatcher::Get().enqueue<WindowCloseEvent>();
				
			});

		glfwSetKeyCallback(windowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Platform::Data& data = *(Platform::Data*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					Dispatcher::Get().enqueue<KeyPressedEvent>((Key)key, false);
					break;
				}
				case GLFW_RELEASE:
				{
					Dispatcher::Get().enqueue<KeyReleasedEvent>((Key)key);
					break;
				}
				case GLFW_REPEAT:
				{
					Dispatcher::Get().enqueue<KeyPressedEvent>((Key)key, true);
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
					Dispatcher::Get().enqueue<MouseButtonPressedEvent>((Button)button);
					break;
				}
				case GLFW_RELEASE:
				{
					Dispatcher::Get().enqueue<MouseButtonReleasedEvent>((Button)button);
					break;
				}
				}
			});

		glfwSetScrollCallback(windowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				Dispatcher::Get().enqueue<MouseScrolledEvent>((f32)xOffset, (f32)yOffset);
			});

		glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* window, double xPos, double yPos)
			{
				Dispatcher::Get().enqueue<MouseMovedEvent>((f32)xPos, (f32)yPos);
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

	void Platform::SetVSync(b8 enable)
	{
		if (enable) { glfwSwapInterval(1); }
		else { glfwSwapInterval(0); }
		m_data.vSync = enable;
	}

	void Platform::SetTitle(const std::string& title)
	{
		m_data.title = title;
		glfwSetWindowTitle((GLFWwindow*)m_windowHandle, title.c_str());
	}
}

#endif // P_WINDOWS
