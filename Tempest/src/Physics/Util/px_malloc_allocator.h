/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Memory.h"
#include "px_allocator.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Allocator adapter for PxAllocatorCallback using explicit 
	 * aligned malloc.
	 */
	struct px_malloc_allocator_t : public physx::PxAllocatorCallback
	{
		void* allocate(
			size_t size, 
			[[maybe_unused]] const char* typeName, 
			[[maybe_unused]] const char* filename, 
			[[maybe_unused]] int line) override
		{
			try
			{
				return _aligned_malloc(size, platform_dependent_alignment);
			}
			catch (const std::bad_alloc&)
			{
				// should be bad alloc, anything else don't catch
				std::string s;
				s += "Bad Physx Alloc: MemReq by ";
				s += typeName;
				s += " from ";
				s += filename;
				s += " at line ";
				s += std::to_string(line);
				s += "!";
				throw px_bad_alloc(s);
			}
		}
		void deallocate(void* ptr) override
		{
			_aligned_free(ptr);
		}
	};

	/**
	 * @brief Used for PhysX. Allocator adapter for PxAllocatorCallback using explicit
	 * aligned malloc.
	 */
	px_malloc_allocator_t& px_malloc_allocator();


}