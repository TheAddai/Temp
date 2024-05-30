#pragma once

#include "key.h"
#include "button.h"

#include <glm/glm.hpp>

namespace prime {

	class Input
	{
	public:
		static bool IsKeyPressed(const Key key);

		static bool IsMouseButtonPressed(const Button button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
