#pragma once

#include "prime/core/defines.h"
#include "prime/core/ref.h"
#include "prime/renderer/editor_camera.h"
#include "texture.h"

#include <glm/glm.hpp>

namespace prime {

	class Window;
	class Scene;
	class Renderer
	{
	public:
		struct Config
		{
			ui32 maxSprites = 20;
		};

		enum class API
		{
			openGL
		};

		struct Info
		{
			std::string name = "", version = "";
			std::string vendor = "", card = "";
		};

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void SetViewport(ui32 width, ui32 height);

		static void DrawSceneRuntime(Ref<Scene>& scene);
		static void DrawSceneEditor(Ref<Scene>& scene, EditorCamera& camera);

		static Ref<Texture> GetDefaultTexture();

		static API GetAPI() { return s_API; }
		static const Config GetConfig() { return s_config; }
		static const Info GetInfo() { return s_info; }

		static void DrawPhysicsColliders(Ref<Scene>& scene, EditorCamera& camera);

	private:
		static void Init(void* windowHandle);
		static void Shutdown();
		static void SwapBuffers();

		static void Draw(Ref<Scene>& scene);

	private:
		static API s_API;
		static Config s_config;
		static Info s_info;

		friend class Engine;
	};
}