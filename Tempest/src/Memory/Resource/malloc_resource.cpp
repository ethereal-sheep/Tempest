/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "malloc_resource.h"

namespace Tempest
{

	[[nodiscard]] void* malloc_resource_t::do_allocate(std::size_t bytes, [[maybe_unused]] std::size_t alignment)
	{
		void* rv = malloc(bytes);
		if (nullptr == rv) {
			throw std::bad_alloc();
		}
		return rv;
	}
	void malloc_resource_t::do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment)
	{
		free(ptr);
	}
	bool malloc_resource_t::do_is_equal(const std::pmr::memory_resource& other) const noexcept
	{
		return this == &other;
	}

	m_resource* malloc_resource()
	{
		static malloc_resource_t s_malloc_res;
		return &s_malloc_res;
	}
}