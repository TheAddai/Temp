#pragma once

#include "prime/core/defines.h"

#include <glm/glm.hpp>

namespace prime {

	class Camera
	{
	public:
		Camera();

		void SetProjection(f32 size, f32 nearClip, f32 farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		f32 GetSize() const { return m_size; }
		void SetSize(f32 size);
		f32 GetNearClip() const { return m_near; }
		void SetNearClip(f32 nearClip);
		f32 GetFarClip() const { return m_far; }
		void SetFarClip(f32 farClip);

		glm::mat4 GetProjection() { return m_projection; }

	private:
		void CalculateProjection();

	private:
		f32 m_size = 10.0f;
		f32 m_near = -1.0f, m_far = 1.0f;
		f32 m_aspectRatio = 1.7776f;
		glm::mat4 m_projection = glm::mat4(1.0f);
	};
}
