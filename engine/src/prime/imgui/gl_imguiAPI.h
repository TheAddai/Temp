#pragma once

#include "imguiAPI.h"

namespace prime {

	class GLImGuiAPI : public ImGuiAPI
	{
	public:
		GLImGuiAPI() = default;

		void Init() override;
		void Shutdown() override;

		void BeginRender() override;
		void EndRender() override;
	};
}
