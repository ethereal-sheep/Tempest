#include "aligned_malloc_resource.h"
#include "../Util/aligned_malloc.h"

namespace Tempest
{

	[[nodiscard]] void* aligned_malloc_resource_t::do_allocate(std::size_t bytes, std::size_t alignment)
	{
		void* rv = aligned_malloc(bytes, alignment);
		if (nullptr == rv) {
			throw std::bad_alloc();
		}
		return rv;
	}
	void aligned_malloc_resource_t::do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment)
	{
		aligned_free(ptr);
	}
	bool aligned_malloc_resource_t::do_is_equal(const std::pmr::memory_resource& other) const noexcept
	{
		return this == &other;
	}

	m_resource* aligned_malloc_resource()
	{
		static aligned_malloc_resource_t s_a_malloc_res;
		return &s_a_malloc_res;
	}
}