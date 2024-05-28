#pragma once

#include "defines.h"
#include "key.h"
#include "button.h"

namespace prime {

	// window events
	class WindowCloseEvent
	{
	public:
		WindowCloseEvent() = default;
	};

	class WindowResizeEvent
	{
	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(ui32 width, ui32 height)
			: m_width(width), m_height(height) {}

		ui32 GetWidth() const { return m_width; }
		ui32 GetHeight() const { return m_height; }

	private:
		ui32 m_width = 0, m_height = 0;
	};

	// key events
	class KeyPressedEvent
	{
	public:
		KeyPressedEvent() = default;
		KeyPressedEvent(const Key key, b8 repeat = false)
			: m_key(key), m_repeat(repeat) {}

		Key GetKey() const { return m_key; }
		b8 GetRepeat() const { return m_repeat; }

	private:
		Key m_key;
		b8 m_repeat = false;
	};

	class KeyReleasedEvent
	{
	public:
		KeyReleasedEvent() = default;
		KeyReleasedEvent(Key key) : m_key(key) {}

		Key GetKey() const { return m_key; }

	private:
		Key m_key;
	};

	// mouse events
	class MouseButtonPressedEvent
	{
	public:
		MouseButtonPressedEvent() = default;
		MouseButtonPressedEvent(Button button) : m_button(button) {}

		Button GetButton() const { return m_button; }

	private:
		Button m_button;
	};

	class MouseButtonReleasedEvent
	{
	public:
		MouseButtonReleasedEvent() = default;
		MouseButtonReleasedEvent(Button button) : m_button(button) {}

		Button GetButton() const { return m_button; }

	private:
		Button m_button;
	};

	class MouseMovedEvent
	{
	public:
		MouseMovedEvent() = default;
		MouseMovedEvent(f32 x, f32 y) : m_x(x), m_y(y) {}

		f32 GetX() const { return m_x; }
		f32 GetY() const { return m_y; }

	private:
		f32 m_x = 0.0f, m_y = 0.0f;
	};

	class MouseScrolledEvent
	{
	public:
		MouseScrolledEvent() = default;
		MouseScrolledEvent(f32 xOffset, f32 yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		f32 GetOffsetX() const { return m_xOffset; }
		f32 GetOffsetY() const { return m_yOffset; }

	private:
		f32 m_xOffset = 0.0f, m_yOffset = 0.0f;
	};
}