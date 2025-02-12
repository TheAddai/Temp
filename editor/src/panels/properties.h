#pragma once

#include "prime/scene/entity.h"
#include "prime/scene/components.h"

namespace prime {

	class Properties
	{
	public:
		void ImGuiRender(Entity entity);

	private:
		void DrawNameComponent();
		void DrawAddComponentButton();

	private:
		Entity m_selection;
	};
}
