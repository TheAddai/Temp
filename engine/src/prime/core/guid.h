#pragma once

#include "defines.h"

namespace prime {

	class Guid
	{
	public:
		Guid();
		Guid(ui64 guid);
		Guid(const Guid&) = default;

		operator ui64() const { return m_guid; }

	private:
		ui64 m_guid = 0;
	};
}

namespace std {

	template <typename T> struct hash;

	template<>
	struct hash<prime::Guid>
	{
		ui64 operator()(const prime::Guid& guid) const
		{
			return (ui64)guid;
		}
	};
}