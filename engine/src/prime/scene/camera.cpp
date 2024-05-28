
#include "pch.h"
#include "camera.h"
#include "prime/core/assert.h"

#include <glm/gtc/matrix_transform.hpp>

namespace prime {

	Camera::Camera()
	{
		CalculateProjection();
	}

	void Camera::SetProjection(f32 size, f32 nearClip, f32 farClip)
	{
		m_size = size;
		m_near = nearClip;
		m_far = farClip;
		CalculateProjection();
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		P_ASSERT(width > 0 && height > 0);
		m_aspectRatio = (f32)width / (f32)height;
		CalculateProjection();
	}

	void Camera::SetSize(f32 size)
	{
		m_size = size;
		CalculateProjection();
	}

	void Camera::SetNearClip(f32 nearClip)
	{
		m_near = nearClip;
		CalculateProjection();
	}

	void Camera::SetFarClip(f32 farClip)
	{
		m_far = farClip;
		CalculateProjection();
	}

	void Camera::CalculateProjection()
	{
		f32 orthoLeft = -m_size * m_aspectRatio * 0.5f;
		f32 orthoRight = m_size * m_aspectRatio * 0.5f;
		f32 orthoBottom = -m_size * 0.5f;
		f32 orthoTop = m_size * 0.5f;

		m_projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_near, m_far);
	}
}