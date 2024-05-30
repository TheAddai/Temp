#pragma once

#include "prime/core/defines.h"
#include "prime/core/events.h"

#include <glm/glm.hpp>

namespace prime {

	class EditorCamera
	{
	public:
		EditorCamera();
		void SetAspectRatio(f32 aspectRatio);

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec2& position);

		f32 GetRotation() const { return m_rotation; }
		void SetRotation(f32 rotation);

		void Update();
		void Resize(f32 width, f32 height);

		f32 GetZoomLevel() const { return m_zoomLevel; }
		void SetZoomLevel(f32 level);

		void SubscribeToEvent();

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void CalculateProjection();
		void CalculateView();

		void OnWindowResize(const WindowResizeEvent& e);
		void OnMouseScroll(const MouseScrolledEvent& e);

	private:
		glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);

		glm::vec2 m_position = { 0.0f, 0.0f };
		f32 m_rotation = 0.0f, m_zoomLevel = 4.0f;
		f32 m_aspectRatio = 1.77776f, m_scrollSpeed = 0.2f;
		f32 m_speed = .2f,  m_rotationSpeed = 180.0f;
	};
}