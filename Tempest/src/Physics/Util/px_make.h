#pragma once
#include "Core.h"
#include "px_deleter.h"

namespace Tempest
{

	template <typename T>
	tsptr<T> px_make(T* s)
	{
		return tsptr<T>(s, px_deleter<T>());
	}
}