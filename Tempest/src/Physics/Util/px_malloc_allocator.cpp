#include "px_malloc_allocator.h"

namespace Tempest
{
	px_malloc_allocator_t& px_malloc_allocator()
	{
		static px_malloc_allocator_t alloc;
		return alloc;
	}
}