#pragma once

#include "prime/core/defines.h"

#include <string>

namespace prime {

	class Platform
	{
	public:
		struct Data
		{
			ui32 width = 0, height = 0;
			b8 fullScreen = false, vSync = true;
			std::string title = "";
		};

		void Init(const std::string& title, ui32 width, ui32 height, b8 fullScreen);
		void Shutdown();

		void Update();
		void SetVSync(b8 enable);

		b8 IsVSync() { return m_data.vSync; }

		void* GetWindowHandle() const { return m_windowHandle; }

		ui32 GetWidth() const { return m_data.width; }
		ui32 GetHeight() const { return m_data.height; }

	private:
		void* m_windowHandle;
		Data m_data;
	};
}