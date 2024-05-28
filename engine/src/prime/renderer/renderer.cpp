
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

	void Renderer::DrawScene(Ref<Scene>& scene)
	{
		s_rendererAPI->Clear();

		Entity& cameraEntity = scene->GetMainCamera();
		if (cameraEntity)
		{
			TransformComponent& tC = cameraEntity.GetComponent<TransformComponent>();
			glm::mat4 tM = glm::inverse(GetTransform(tC.position, tC.rotation));

			Camera& camera = cameraEntity.GetComponent<CameraComponent>().camera;
			glm::mat4 projection = camera.GetProjection() * tM;

			s_rendererAPI->BeginDrawing(projection);

			entt::basic_view sEs = scene->m_registry.view<TransformComponent, SpriteComponent>();
			for (entt::entity sE : sEs)
			{
				auto [sT, s] = sEs.get<TransformComponent, SpriteComponent>(sE);
				s_rendererAPI->DrawQuad(sT.position, sT.scale, s.color, sT.rotation);
			}

			s_rendererAPI->EndDrawing();
		}
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
}