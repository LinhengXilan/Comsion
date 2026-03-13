#ifndef CORE_TYPE_H
#define CORE_TYPE_H

#include <pch.h>

template<typename T>
using ObjectRef = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr ObjectRef<T> CreateObjectRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif