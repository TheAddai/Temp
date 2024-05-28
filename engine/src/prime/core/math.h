#pragma once

#include "defines.h"

#include <glm/glm.hpp>

namespace prime {

	glm::mat4 GetTransform(const glm::vec2& position, const glm::vec2& scale, f32 rotation);

	glm::mat4 GetTransform(const glm::vec2& position, f32 rotation);
}