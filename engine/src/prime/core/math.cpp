
#include "pch.h"
#include "math.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace prime {

	glm::mat4 GetTransform(const glm::vec2& position, const glm::vec2& scale, f32 rotation)
	{
		glm::mat4 rot = glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, rotation)));

		return glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* rot
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f });
	}

	glm::mat4 GetTransform(const glm::vec2& position, f32 rotation)
	{
		glm::mat4 rot = glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, rotation)));

		return glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* rot;
	}
}