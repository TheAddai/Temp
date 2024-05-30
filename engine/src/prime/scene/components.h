#pragma once

#include "prime/core/defines.h"
#include "prime/core/guid.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <string>

namespace prime {

	class IDComponent
	{
	public:
		IDComponent() = default;
		IDComponent(Guid guid) : guid(guid) {};
		IDComponent(const IDComponent&) = default;

	public:
		Guid guid;
	};

	class TransformComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec2& position) : position(position) {}

	public:
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 scale = { 1.0f, 1.0f };
		f32 rotation = 0.0f;
	};

	class SpriteComponent
	{
	public:
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color) : color(color) {}

	public:
		glm::vec4 color = glm::vec4(1.0f);
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	public:
		b8 primary = false;
		Camera camera;
	};

	struct NameComponent
	{
		std::string name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			: name(name) {}
	};

	class LineComponent
	{
	public:
		LineComponent() = default;
		LineComponent(const LineComponent&) = default;

	public:
		glm::vec2  endPosition = glm::vec2(1.0f, 0.0f);
		glm::vec4 color = glm::vec4(1.0f);
	};
}