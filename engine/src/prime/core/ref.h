#pragma once

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

// create a shared pointer
template<typename TClass, typename ... TArgs>
constexpr Ref<TClass> CreateRef(TArgs&& ... args)
{
	return std::make_shared<TClass>(std::forward<TArgs>(args)...);
}