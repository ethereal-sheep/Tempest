#pragma once
#include "Core.h"
#include "px_deleter.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Smart pointer management for
	 * created pointers from PhysX.
	 */
	template <typename T>
	tsptr<T> px_make(T* s)
	{
		return tsptr<T>(s, px_deleter<T>());
	}
}