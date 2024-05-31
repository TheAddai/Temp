
#include "pch.h"
#include "gl_rendererAPI.h"
#include "prime/platform/platform_detection.h"
#include "prime/core/assert.h"
#include "vertex.h"
#include "renderer.h"
#include "shaders.h"
#include "prime/core/math.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>

#ifdef P_WINDOWS
#include <GLFW/glfw3.h>
#endif // P_WINDOWS

namespace prime {

	struct SpriteVertex
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		glm::vec2 textureCoords = glm::vec2(0.0f);
		f32 textureIndex = 0.0f;
	};

	struct LineVertex
	{
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
	};

	struct Data
	{
		ui32 maxSprites = 0, maxVertices = 0, maxIndices = 0;
		glm::vec4 vertexPositions[4]{};
		ui32 cameraUniformBuffer = 0;

		// texture
		ui32 textureSlotIndex = 1;
		static const ui32 maxTextureSlots = 16;
		glm::vec2 textureCoords[4]{};
		std::array<Ref<Texture>, maxTextureSlots> textureSlots;

		// sprite
		ui32 spriteVertexArray = 0, spriteVertexBuffer = 0, spriteIndexBuffer = 0;
		ui32 spriteVertexShader = 0, spriteFragmentShader = 0;
		ui32 spriteShaderProgram = 0;
		SpriteVertex* spriteVertexBufferPtr = nullptr;
		SpriteVertex* spriteVertexBufferBase = nullptr;
		ui32 spriteIndexCount = 0;

		// line
		ui32 lineVertexArray = 0, lineVertexBuffer = 0;
		ui32 lineVertexShader = 0, lineFragmentShader = 0, lineShaderProgram = 0;
		LineVertex* lineVertexBufferPtr = nullptr;
		LineVertex* lineVertexBufferBase = nullptr;
		ui32 lineVertexCount = 0;
	};

	static Data s_data;

	static GLenum VertexTypeToOpenglType(VertexType vertexType)
	{
		switch (vertexType)
		{
		case VertexType::position:
		case VertexType::color:
		case VertexType::textureCoords:
		case VertexType::textureIndex:
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

	static ui32 CompileShader(const char* shaderSource, i32 shaderType)
	{
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, 0);
		glCompileShader(shader);

		// check to see if it compile
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled == 0)
		{
			GLint max_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
			char log_message[2000];
			memset(log_message, 0, sizeof(log_message));
			glGetShaderInfoLog(shader, max_length, &max_length, log_message);

			if (shaderType == GL_VERTEX_SHADER) { P_ERROR("vertex shader compilation failed"); }
			if (shaderType == GL_FRAGMENT_SHADER) { P_ERROR("fragment shader compilation failed"); }
			P_ERROR("{0}", log_message);
			P_ASSERT(false);
		}
		return shader;
	}

	static ui32 GenerateProgram(ui32 vertexShader, ui32 fragmentShader)
	{
		ui32 program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		// check if mProgram linked
		GLint linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked == 0)
		{
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
			char log_message[4000];
			memset(log_message, 0, sizeof(log_message));
			glGetProgramInfoLog(program, max_length, &max_length, log_message);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			P_ERROR("{0}", log_message);
			P_ASSERT_MSG(false, "program linking failed");
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		return program;
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

		s_data.textureCoords[0] = { 0.0f, 0.0f };
		s_data.textureCoords[1] = { 1.0f, 0.0f };
		s_data.textureCoords[2] = { 1.0f, 1.0f };
		s_data.textureCoords[3] = { 0.0f, 1.0f };

		s_data.textureSlots[0] = Texture::Create(1, 1, TextureFilter::linear, TextureWrap::repeat);

		InitSpriteRendering();
		InitLineRendering();

		glGenBuffers(1, &s_data.cameraUniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, s_data.cameraUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_data.cameraUniformBuffer);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GLRendererAPI::Shutdown()
	{
		// sprite
		delete[] s_data.spriteVertexBufferBase;
		glDeleteBuffers(1, &s_data.spriteVertexBuffer);
		glDeleteBuffers(1, &s_data.spriteIndexBuffer);
		glDeleteVertexArrays(1, &s_data.spriteVertexArray);

		glDetachShader(s_data.spriteShaderProgram, s_data.spriteVertexShader);
		glDetachShader(s_data.spriteShaderProgram, s_data.spriteFragmentShader);
		glDeleteProgram(s_data.spriteShaderProgram);
		glDeleteShader(s_data.spriteVertexShader);
		glDeleteShader(s_data.spriteFragmentShader);

		// sprite
		delete[] s_data.lineVertexBufferBase;
		glDeleteBuffers(1, &s_data.lineVertexBuffer);
		glDeleteVertexArrays(1, &s_data.lineVertexArray);

		glDetachShader(s_data.lineShaderProgram, s_data.lineVertexShader);
		glDetachShader(s_data.lineShaderProgram, s_data.lineFragmentShader);
		glDeleteProgram(s_data.lineShaderProgram);
		glDeleteShader(s_data.lineVertexShader);
		glDeleteShader(s_data.lineFragmentShader);

		glDeleteBuffers(1, &s_data.cameraUniformBuffer);
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

			for (ui32 i = 0; i < s_data.textureSlotIndex; i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, s_data.textureSlots[i]->GetID());
			}

			glUseProgram(s_data.spriteShaderProgram);
			glBindVertexArray(s_data.spriteVertexArray);
			glDrawElements(GL_TRIANGLES, s_data.spriteIndexCount, GL_UNSIGNED_INT, nullptr);
		}

		if (s_data.lineVertexCount)
		{
			i64 lineDataSize = (ui8*)s_data.lineVertexBufferPtr - (ui8*)s_data.lineVertexBufferBase;
			glBindBuffer(GL_ARRAY_BUFFER, s_data.lineVertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, (ui32)lineDataSize, s_data.lineVertexBufferBase);

			glUseProgram(s_data.lineShaderProgram);
			glBindVertexArray(s_data.lineVertexArray);
			glDrawArrays(GL_LINES, 0, s_data.lineVertexCount);
		}
	}

	void GLRendererAPI::BeginDrawing(const glm::mat4& projection)
	{
		StartBatch();

		glBindBuffer(GL_UNIFORM_BUFFER, s_data.cameraUniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection);
	}

	void GLRendererAPI::EndDrawing()
	{
		Flush();
	}

	void GLRendererAPI::SetViewport(ui32 width, ui32 height)
	{
		glViewport(0, 0, width, height);
	}

	Ref<Texture> GLRendererAPI::GetDefaultTexture()
	{
		return s_data.textureSlots[0];
	}

	void GLRendererAPI::DrawQuad(Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation)
	{
		if (s_data.spriteIndexCount >= s_data.maxIndices)
		{
			Flush();
			StartBatch();
		}

		if (s_data.textureSlotIndex > s_data.maxTextureSlots)
		{
			Flush();
			StartBatch();
		}

		f32 textureIndex = 0.0f;
		if (texture.get()) { textureIndex = GetTextureIndex(texture); }

		glm::mat4 transform = GetTransform(position, scale, rotation);
		for (auto i = 0; i < 4; i++)
		{
			s_data.spriteVertexBufferPtr->position = transform * s_data.vertexPositions[i];
			s_data.spriteVertexBufferPtr->textureCoords = s_data.textureCoords[i];
			s_data.spriteVertexBufferPtr->textureIndex = textureIndex;
			s_data.spriteVertexBufferPtr->color = color;
			s_data.spriteVertexBufferPtr++;
		}
		s_data.spriteIndexCount += 6;
	}

	void GLRendererAPI::DrawLine(const glm::vec2& point, const glm::vec2& point2, const glm::vec4& color)
	{
		if (s_data.spriteIndexCount >= s_data.maxIndices)
		{
			Flush();
			StartBatch();
		}

		s_data.lineVertexBufferPtr->position = { point.x, point.y};
		s_data.lineVertexBufferPtr->color = color;
		s_data.lineVertexBufferPtr++;

		s_data.lineVertexBufferPtr->position = { point2.x, point2.y};
		s_data.lineVertexBufferPtr->color = color;
		s_data.lineVertexBufferPtr++;

		s_data.lineVertexCount += 2;
	}

	void GLRendererAPI::DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation)
	{
		if (s_data.spriteIndexCount >= s_data.maxIndices)
		{
			Flush();
			StartBatch();
		}

		glm::mat4 transform = GetTransform(position, scale, rotation);
		glm::vec2 rectVertices[4]{};
		for (size_t i = 0; i < 4; i++)
			rectVertices[i] = transform * s_data.vertexPositions[i];

		static ui32 index[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };
		for (size_t x = 0; x < 8; x++)
		{
			s_data.lineVertexBufferPtr->position = rectVertices[index[x]];
			s_data.lineVertexBufferPtr->color = color;
			s_data.lineVertexBufferPtr++;
		}
		s_data.lineVertexCount += 8;
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
		spriteLayout.AddVertex(Vertex(VertexType::color, "a_color"));
		spriteLayout.AddVertex(Vertex(VertexType::textureCoords, "a_textureCoords"));
		spriteLayout.AddVertex(Vertex(VertexType::textureIndex, "a_textureIndex"));
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

		s_data.spriteVertexShader = CompileShader(s_spriteVertexShaderSource, GL_VERTEX_SHADER);
		s_data.spriteFragmentShader = CompileShader(s_spritePixelShaderSource, GL_FRAGMENT_SHADER);
		s_data.spriteShaderProgram = GenerateProgram(s_data.spriteVertexShader, s_data.spriteFragmentShader);

		// texture
		int32_t samplers[s_data.maxTextureSlots]{};
		for (uint32_t i = 0; i < s_data.maxTextureSlots; i++) { samplers[i] = i; }

		glUseProgram(s_data.spriteShaderProgram);
		GLint location = glGetUniformLocation(s_data.spriteShaderProgram, "u_textures");
		glUniform1iv(location, s_data.maxTextureSlots, samplers);
	}

	void GLRendererAPI::InitLineRendering()
	{
		glGenVertexArrays(1, &s_data.lineVertexArray);
		glBindVertexArray(s_data.lineVertexArray);

		glGenBuffers(1, &s_data.lineVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, s_data.lineVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, s_data.maxVertices * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);
		s_data.lineVertexBufferBase = new LineVertex[s_data.maxVertices];

		VertexLayout lineLayout;
		lineLayout.AddVertex(Vertex(VertexType::position, "a_position"));
		lineLayout.AddVertex(Vertex(VertexType::color, "a_color"));
		SubmitDataToGPU(lineLayout);

		s_data.lineVertexShader = CompileShader(s_lineVertexShaderSource, GL_VERTEX_SHADER);
		s_data.lineFragmentShader = CompileShader(s_linePixelShaderSource, GL_FRAGMENT_SHADER);
		s_data.lineShaderProgram = GenerateProgram(s_data.lineVertexShader, s_data.lineFragmentShader);

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2.0f);
	}
	
	void GLRendererAPI::StartBatch()
	{
		// sprite
		s_data.spriteIndexCount = 0;
		s_data.spriteVertexBufferPtr = s_data.spriteVertexBufferBase;

		// line
		s_data.lineVertexCount = 0;
		s_data.lineVertexBufferPtr = s_data.lineVertexBufferBase;

		// texture
		s_data.textureSlotIndex = 1;
	}
	
	f32 GLRendererAPI::GetTextureIndex(Ref<Texture>& texture)
	{
		f32 textureIndex = 0.0f;
		for (ui32 i = 1; i < s_data.textureSlotIndex; i++)
		{
			if (*s_data.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (f32)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			s_data.textureSlotIndex++;
		}
		return textureIndex;
	}
}