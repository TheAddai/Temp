#pragma once

namespace prime {

	class ImGuiAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		static ImGuiAPI* Create();
	};
}