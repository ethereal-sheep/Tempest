#pragma once

#include "Core.h"

namespace Tempest
{

	class aligned_malloc_resource_t : public std::pmr::memory_resource
	{
	protected:
		[[nodiscard]] void* do_allocate(std::size_t bytes, [[maybe_unused]] std::size_t alignment) override;
		void do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) override;
		bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
	};

	m_resource* aligned_malloc_resource();
}