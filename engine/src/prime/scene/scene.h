#pragma once

#include <entt/entt.hpp>
#include <memory>

namespace prime {

	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		static std::shared_ptr<Scene> Create();

	private:
		entt::registry m_registry;
	};
}
