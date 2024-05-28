
#include "pch.h"
#include "gl_rendererAPI.h"
#include "prime/platform/platform_detection.h"
#include "prime/core/assert.h"
#include "vertex.h"
#include "renderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#ifdef P_WINDOWS
#include <GLFW/glfw3.h>
#endif // P_WINDOWS

namespace prime {

	struct SpriteVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
	};

	struct Data
	{
		ui32 maxSprites = 0, maxVertices = 0, maxIndices = 0;
		glm::vec4 vertexPositions[4]{};

		// sprite
		ui32 spriteVertexArray = 0, spriteVertexBuffer = 0, spriteIndexBuffer = 0;
		SpriteVertex* spriteVertexBufferPtr = nullptr;
		SpriteVertex* spriteVertexBufferBase = nullptr;
		ui32 spriteIndexCount = 0;
	};

	static Data s_data;

	static GLenum VertexTypeToOpenglType(VertexType vertexType)
	{
		switch (vertexType)
		{
		case VertexType::position:
			return GL_FLOAT;
			break;
		}
		return 0;
	}

	static void SubmitDataToGPU(VertexLayout& layout)
	{
		layout.ProcessVertices();

		ui32 index = 0;
		for (const Vertex& vertex : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				vertex.GetCount(),
				VertexTypeToOpenglType(vertex.type),
				GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)vertex.offset);

			index++;
		}
	}

	glm::mat4 GetTransform(const glm::vec2& position, const glm::vec2& scale, f32 rotation)
	{
		glm::mat4 rot = glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, rotation)));
			
		return glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* rot
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f });
	}

	void GLRendererAPI::Init(void* windowHandle)
	{
#ifdef P_WINDOWS
		glfwMakeContextCurrent((GLFWwindow*)windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		P_ASSERT_MSG(status, "Failed to initialize Glad!");
#endif // P_WINDOWS

		s_data.maxSprites = Renderer::GetConfig().maxSprites;
		s_data.maxVertices = s_data.maxSprites * 4;
		s_data.maxIndices = s_data.maxSprites * 6;

		s_data.vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		InitSpriteRendering();
	}

	void GLRendererAPI::Shutdown()
	{
		// sprite
		delete[] s_data.spriteVertexBufferBase;
		glDeleteBuffers(1, &s_data.spriteVertexBuffer);
		glDeleteBuffers(1, &s_data.spriteIndexBuffer);
		glDeleteVertexArrays(1, &s_data.spriteVertexArray);
	}

	void GLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void GLRendererAPI::SwapBuffers(void* windowHandle)
	{
#ifdef P_WINDOWS
		glfwSwapBuffers((GLFWwindow*)windowHandle);
#endif // P_WINDOWS
	}

	void GLRendererAPI::Flush()
	{
		if (s_data.spriteIndexCount)
		{
			i64 spriteDataSize = (ui8*)s_data.spriteVertexBufferPtr - (ui8*)s_data.spriteVertexBufferBase;
			glBindBuffer(GL_ARRAY_BUFFER, s_data.spriteVertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, (ui32)spriteDataSize, s_data.spriteVertexBufferBase);

			glBindVertexArray(s_data.spriteVertexArray);
			glDrawElements(GL_TRIANGLES, s_data.spriteIndexCount, GL_UNSIGNED_INT, nullptr);
		}
	}

	void GLRendererAPI::BeginDrawing()
	{
		StartBatch();
	}

	void GLRendererAPI::EndDrawing()
	{
		Flush();
	}

	void GLRendererAPI::DrawQuad(const glm::vec2& position, const glm::vec2& scale)
	{
		if (s_data.spriteIndexCount >= s_data.maxIndices)
		{
			Flush();
			StartBatch();
		}

		glm::mat4 transform = GetTransform(position, scale, 0);
		for (auto i = 0; i < 4; i++)
		{
			s_data.spriteVertexBufferPtr->position = transform * s_data.vertexPositions[i];
			s_data.spriteVertexBufferPtr++;
		}
		s_data.spriteIndexCount += 6;
	}
	
	void GLRendererAPI::InitSpriteRendering()
	{
		glGenVertexArrays(1, &s_data.spriteVertexArray);
		glBindVertexArray(s_data.spriteVertexArray);

		glGenBuffers(1, &s_data.spriteVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, s_data.spriteVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, s_data.maxVertices * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);
		s_data.spriteVertexBufferBase = new SpriteVertex[s_data.maxVertices];

		VertexLayout spriteLayout;
		spriteLayout.AddVertex(Vertex(VertexType::position, "a_position"));
		SubmitDataToGPU(spriteLayout);

		// indices
		ui32* indices = new ui32[s_data.maxIndices];
		ui32 offset = 0;
		for (ui32 x = 0; x < s_data.maxIndices; x += 6)
		{
			indices[x + 0] = offset + 0;
			indices[x + 1] = offset + 1;
			indices[x + 2] = offset + 2;

			indices[x + 3] = offset + 2;
			indices[x + 4] = offset + 3;
			indices[x + 5] = offset + 0;

			offset += 4;
		}

		glGenBuffers(1, &s_data.spriteIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data.spriteIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_data.maxIndices, indices, GL_DYNAMIC_DRAW);
		delete[] indices;
	}
	
	void GLRendererAPI::StartBatch()
	{
		// sprite
		s_data.spriteIndexCount = 0;
		s_data.spriteVertexBufferPtr = s_data.spriteVertexBufferBase;
	}
}