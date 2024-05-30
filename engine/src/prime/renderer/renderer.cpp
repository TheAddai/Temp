
#include "pch.h"
#include "renderer.h"
#include "rendererAPI.h"
#include "prime/platform/platform.h"
#include "prime/scene/scene.h"
#include "prime/scene/components.h"
#include "prime/scene/entity.h"
#include "prime/core/math.h"

// openGL
#include "gl_rendererAPI.h"

namespace prime {

	Renderer::API Renderer::s_API;
	Renderer::Config Renderer::s_config;

	static RendererAPI* s_rendererAPI = nullptr;
	static void* s_windowHandle = nullptr;

	struct Data
	{
		glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	static Data s_data;

	static RendererAPI* CreateAPI()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return new GLRendererAPI();
			break;
		}
		return nullptr;
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_data.ClearColor = color;
		s_rendererAPI->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		s_rendererAPI->Clear();
	}

	void Renderer::SetViewport(ui32 width, ui32 height)
	{
		s_rendererAPI->SetViewport(width, height);
	}

	void Renderer::DrawSceneRuntime(Ref<Scene>& scene)
	{
		glm::mat4 cM = glm::mat4(1.0f);
		Camera* mainCamera = nullptr;

		entt::basic_view cEs = scene->m_registry.view<TransformComponent, CameraComponent>();
		for (entt::entity cE : cEs)
		{
			auto [cT, c] = cEs.get<TransformComponent, CameraComponent>(cE);

			if (c.primary)
			{
				mainCamera = &c.camera;
				cM = glm::inverse(GetTransform(cT.position, cT.rotation));
				break;
			}
		}

		if (mainCamera)
		{
			glm::mat4 projection = mainCamera->GetProjection() * cM;

			s_rendererAPI->BeginDrawing(projection);
			Draw(scene);
			s_rendererAPI->EndDrawing();
		}
	}

	void Renderer::DrawSceneEditor(Ref<Scene>& scene, EditorCamera& camera)
	{
		s_rendererAPI->BeginDrawing(camera.GetViewProjectionMatrix());
		Draw(scene);
		s_rendererAPI->EndDrawing();
	}

	void Renderer::Init(void* windowHandle)
	{
		s_windowHandle = windowHandle;
		s_rendererAPI = CreateAPI();
		s_rendererAPI->Init(windowHandle);
	}

	void Renderer::Shutdown()
	{
		s_rendererAPI->Shutdown();
		delete s_rendererAPI;
		s_rendererAPI = nullptr;
	}
	
	void Renderer::SwapBuffers()
	{
		s_rendererAPI->SwapBuffers(s_windowHandle);
	}
	
	void Renderer::Draw(Ref<Scene>& scene)
	{
		s_rendererAPI->Clear();

		entt::basic_view sEs = scene->m_registry.view<TransformComponent, SpriteComponent>();
		for (entt::entity sE : sEs)
		{
			auto [sT, s] = sEs.get<TransformComponent, SpriteComponent>(sE);
			s_rendererAPI->DrawQuad(sT.position, sT.scale, s.color, sT.rotation);
		}
	}
}