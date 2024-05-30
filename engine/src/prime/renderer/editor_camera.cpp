
#include "pch.h"
#include "editor_camera.h"
#include "prime/core/input.h"
#include "prime/core/dispatcher.h"
#include "prime/core/logger.h"

#include <glm/gtc/matrix_transform.hpp>

namespace prime {

	EditorCamera::EditorCamera()
	{
		CalculateProjection();
	}

	void EditorCamera::SetAspectRatio(f32 aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		CalculateProjection();
	}

	void EditorCamera::SetPosition(const glm::vec2& position)
	{
		m_position = position;
		CalculateView();
	}

	void EditorCamera::SetRotation(f32 rotation)
	{
		m_rotation = rotation;
		CalculateView();
	}

	void EditorCamera::Update()
	{
		bool control = Input::IsKeyPressed(Key::keyLeftControl) || Input::IsKeyPressed(Key::keyLeftControl);

		if (Input::IsKeyPressed(Key::keyRight))
		{
			if (control)
			{
				m_position.x -= m_speed;
			}
		}

		if (Input::IsKeyPressed(Key::keyLeft))
		{
			if (control)
			{
				m_position.x += m_speed;
			}
		}

		if (Input::IsKeyPressed(Key::keyDown))
		{
			if (control)
			{
				m_position.y += m_speed;
			}
		}

		if (Input::IsKeyPressed(Key::keyUp))
		{
			if (control)
			{
				m_position.y -= m_speed;
			}
		}

		if (Input::IsKeyPressed(Key::keyEqual))
		{
			if (control)
			{
				m_zoomLevel += m_scrollSpeed;
				m_zoomLevel = std::max(m_zoomLevel, m_scrollSpeed);
				CalculateProjection();
			}
		}

		if (Input::IsKeyPressed(Key::keyMinus))
		{
			if (control)
			{
				m_zoomLevel -= m_scrollSpeed;
				m_zoomLevel = std::max(m_zoomLevel, m_scrollSpeed);
				CalculateProjection();
			}
		}

		SetPosition(m_position);
	}

	void EditorCamera::Resize(f32 width, f32 height)
	{
		m_aspectRatio = width / height;
		CalculateProjection();
	}

	void EditorCamera::SetZoomLevel(f32 level)
	{
		m_zoomLevel = level;
		CalculateProjection();
	}

	void EditorCamera::SubscribeToEvent()
	{
		Dispatcher::Get().sink<WindowResizeEvent>().connect<&EditorCamera::OnWindowResize>(this);
		Dispatcher::Get().sink<MouseScrolledEvent>().connect<&EditorCamera::OnMouseScroll>(this);
	}

	void EditorCamera::CalculateProjection()
	{
		m_projectionMatrix = glm::ortho(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
			-m_zoomLevel, m_zoomLevel, -1.0f, 1.0f);

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void EditorCamera::CalculateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { m_position.x, m_position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
	
	void EditorCamera::OnWindowResize(const WindowResizeEvent& e)
	{
		Resize((f32)e.GetWidth(), (f32)e.GetHeight());
	}

	void EditorCamera::OnMouseScroll(const MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.GetOffsetY() * m_scrollSpeed;
		m_zoomLevel = std::max(m_zoomLevel, m_scrollSpeed);
		CalculateProjection();
	}
}