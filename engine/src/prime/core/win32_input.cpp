
#include "pch.h"
#include "input.h"
#include "prime/platform/platform_detection.h"
#include "engine.h"

#ifdef P_WINDOWS
#include <GLFW/glfw3.h>

namespace prime {

	bool Input::IsKeyPressed(const Key key)
	{
		GLFWwindow* window = (GLFWwindow*)Engine::GetWindowHandle();
		auto state = glfwGetKey(window, static_cast<i32>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const Button button)
	{
		GLFWwindow* window = (GLFWwindow*)Engine::GetWindowHandle();
		auto state = glfwGetMouseButton(window, static_cast<i32>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = (GLFWwindow*)Engine::GetWindowHandle();
		double xpos, ypos;
		glfwGetCursorPos((GLFWwindow*)Engine::GetWindowHandle(), &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}

#endif // P_WINDOWS