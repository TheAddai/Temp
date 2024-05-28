#pragma once

#include "pch.h"
#include "gl_frame_buffer.h"
#include "prime/core/logger.h"

#include <glad/glad.h>

namespace prime {

	GLFramebuffer::GLFramebuffer(ui32 width, ui32 height)
	{
		m_width = width;
		m_height = height;
		Generate();
	}

	GLFramebuffer::~GLFramebuffer()
	{
		glDeleteTextures(1, &m_texture);
		glDeleteRenderbuffers(1, &m_renderBuffer);
		glDeleteFramebuffers(1, &m_handle);
	}

	void GLFramebuffer::Resize(ui32 width, ui32 height)
	{
		m_width = width;
		m_height = height;

		if (m_handle)
		{
			glDeleteRenderbuffers(1, &m_renderBuffer);
			glDeleteFramebuffers(1, &m_handle);
			glDeleteTextures(1, &m_texture);

			m_texture = 0;
			m_renderBuffer = 0;
			m_handle = 0;
		}
		Generate();
	}

	void GLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
		glViewport(0, 0, m_width, m_height);
	}

	void GLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void GLFramebuffer::Generate()
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		// color texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		// Create depth/stencil renderbuffer
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Check for completeness
		i32 completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			P_ERROR("Failure to create framebuffer. Complete status: {}", completeStatus);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}