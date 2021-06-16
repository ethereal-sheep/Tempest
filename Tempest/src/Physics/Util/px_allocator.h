#pragma once
#include "Core.h"
#include "Memory.h"

#include "px_include.h"

namespace Tempest
{
	class px_bad_alloc : public std::exception
	{
	public:
		px_bad_alloc(const string& err_msg = "graph exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	class px_allocator : public physx::PxAllocatorCallback
	{
	public:
		px_allocator(m_resource* mem_res = aligned_malloc_resource()) : mem{ mem_res } {}

		void* allocate(size_t size, [[maybe_unused]]const char* typeName, [[maybe_unused]] const char* filename, [[maybe_unused]] int line) //override
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
		void deallocate(void* ptr) //override
		{
			mem->deallocate(ptr, 0);
		}

	private:
		m_resource* mem;
	};



}