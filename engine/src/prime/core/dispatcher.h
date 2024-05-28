#pragma once

#include "defines.h"
#include <entt/entt.hpp>

namespace prime {

	class Dispatcher
	{
	public:
		static entt::dispatcher& Get() { return m_dispatcher; }

	private:
		P_INLINE static entt::dispatcher m_dispatcher;
	};
}