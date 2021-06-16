#pragma once
#include "Core.h"
#include "Memory.h"

#include "px_include.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Bad alloc exception for physx allocations.
	 */
	class px_bad_alloc : public std::exception
	{
	public:
		px_bad_alloc(const string& err_msg = "graph exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Used for PhysX. Allocator adapter for PxAllocatorCallback using
	 * pmr resources. 
	 */
	class px_allocator : public physx::PxAllocatorCallback
	{
	public:
		/**
		 * @brief Constructor. Stores a polymorphic resource and allocate from it.
		 * Default to aligned_malloc_resource as we need to align all allocations
		 * to 16 bytes.
		 */
		px_allocator(m_resource* mem_res = aligned_malloc_resource()) : mem{ mem_res } {}

		/**
		 * @brief Allocates some number of bytes, aligned to 16 bytes. and returns a pointer to it.
		 */
		void* allocate(
			size_t size, 
			[[maybe_unused]]const char* typeName, 
			[[maybe_unused]] const char* filename, 
			[[maybe_unused]] int line) override
		{
			try
			{
				return mem->allocate(size, platform_dependent_alignment); 
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
		/**
		 * @brief Deallocates a pointer.
		 */
		void deallocate(void* ptr) override
		{
			mem->deallocate(ptr, 0);
		}

	private:
		m_resource* mem;
	};



}