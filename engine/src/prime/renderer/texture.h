#pragma once

#include "prime/core/defines.h"
#include "prime/core/ref.h"

#include <string>

namespace prime {

	enum class TextureFilter
	{
		nearest,
		linear
	};

	enum class TextureWrap
	{
		repeat,
		clamp
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual ui32 GetID() const = 0;
		virtual std::string GetPath() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		static Ref<Texture> Create(ui32 width, ui32 height, TextureFilter filter, TextureWrap wrap);

	private:
		static Ref<Texture> Create(const std::string& path);
		friend class ResourceManager;
	};
}
