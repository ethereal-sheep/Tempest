#pragma once

#include "..\..\Core.h"

namespace Tempest
{

	class malloc_resource_t : public std::pmr::memory_resource
	{
	protected:
		[[nodiscard]] void* do_allocate(std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
		{
			void* rv = malloc(bytes);
			if (nullptr == rv) {
				throw std::bad_alloc();
			}
			return rv;
		}
		void do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) override
		{
			free(ptr);
		}
		bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
		{
			return this == &other;
		}
	};

	memres* malloc_resource() 
	{
		static malloc_resource_t s_malloc_res;
		return &s_malloc_res;
	}
}