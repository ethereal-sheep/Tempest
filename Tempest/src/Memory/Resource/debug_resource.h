/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#include "..\..\Core.h"
#include <mutex>

namespace Tempest
{

	class debug_resource : public std::pmr::memory_resource
	{
	public:

		debug_resource(
			std::string name = std::string{ "debug_memory_resource" },
			m_resource* upstream = std::pmr::get_default_resource());

		~debug_resource();

		debug_resource(const debug_resource&) = delete;
		debug_resource& operator=(const debug_resource&) = delete;

		// get upstream resource
		const m_resource* upstream_resource() const { return m_upstream; }

		// get/set for meta data
		std::string get_name() const noexcept { return m_name; }
		void set_name(const std::string& name) noexcept { m_name = name; }
		bool is_verbose() const noexcept { return m_verbose_flag; }
		bool is_strict() const noexcept { return m_strict_flag; }
		void set_verbose(bool is_verbose) noexcept { m_verbose_flag = is_verbose; }
		void set_strict(bool is_strict) noexcept { m_strict_flag = is_strict; }
		bool is_no_abort() const noexcept { return m_no_abort_flag; }
		void set_no_abort(bool no_abort) noexcept { m_no_abort_flag = no_abort; }

		// statistics for UI
		size_t bytes_allocated() const noexcept { return m_bytes_allocated; }                         // total bytes allocated in lifetime
		size_t bytes_deallocated() const noexcept { return m_bytes_allocated - m_bytes_outstanding; } // total bytes deallocated in lifetime
		size_t bytes_outstanding() const noexcept { return m_bytes_outstanding; }                     // current outstanding bytes
		size_t bytes_highwater() const noexcept { return m_bytes_highwater; }						  // highest number of outstanding bytes
		size_t blocks_allocated() const noexcept { return m_blocks_allocated; }                       // highest number of outstanding bytes
		size_t blocks_outstanding() const noexcept { return m_blocks.size(); }                        // number of outstanding blocks of memory

		// history
		size_t last_allocated_num_bytes() const noexcept { return m_last_allocated_num_bytes; }
		size_t last_allocated_alignment() const noexcept { return m_last_allocated_alignment; }
		void* last_allocated_address() const noexcept { return m_last_allocated_address; }

		size_t last_deallocated_num_bytes() const noexcept { return m_last_deallocated_num_bytes; }
		size_t last_deallocated_alignment() const noexcept { return m_last_deallocated_alignment; }
		void* last_deallocated_address() const noexcept { return m_last_deallocated_address; }

		// errors
		size_t misc_errors() const noexcept { return m_misc_errors; }
		size_t bad_deallocations() const noexcept { return m_bad_deallocations; }

		// memory leak reporting
		static size_t leaked_bytes() { return s_leaked_bytes; }
		static size_t leaked_blocks() { return s_leaked_blocks; }
		static void clear_leaked()
		{
			s_leaked_bytes = 0;
			s_leaked_blocks = 0;
		}

	protected:
		//override virtual fns
		[[nodiscard]] void* do_allocate(std::size_t bytes, std::size_t alignment) override;
		void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
		bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

	private:
		// record all allocations and hold the results
		struct allocation_rec
		{
			void* m_ptr;
			size_t m_index;
			size_t m_bytes;
			size_t m_alignment;
		};

		mutable std::mutex res_mutex;

		// metadata
		std::string m_name;

		// flags
		std::atomic_int m_verbose_flag{ false };
		std::atomic_int m_strict_flag{ false };
		std::atomic_int m_no_abort_flag{ false };

		// statistics
		std::atomic_size_t m_bytes_allocated{ 0 };
		std::atomic_size_t m_bytes_outstanding{ 0 };
		std::atomic_size_t m_bytes_highwater{ 0 };
		std::atomic_size_t m_blocks_allocated{ 0 };

		// history 
		std::atomic_size_t m_last_allocated_num_bytes{ 0 };
		std::atomic_size_t m_last_allocated_alignment{ 0 };
		std::atomic<void*> m_last_allocated_address{ nullptr };

		std::atomic_size_t m_last_deallocated_num_bytes{ 0 };
		std::atomic_size_t m_last_deallocated_alignment{ 0 };
		std::atomic<void*> m_last_deallocated_address{ nullptr };

		// errors
		std::atomic_size_t m_misc_errors{ 0 };
		std::atomic_size_t m_bad_deallocations{ 0 };

		// deprecated
		//std::pmr::vector<allocation_rec> m_blocks;

		std::pmr::unordered_map<void*, allocation_rec, std::hash<void*>> m_blocks;
		m_resource* m_upstream; // the upstream memory resource

		static std::atomic_size_t s_leaked_bytes;
		static std::atomic_size_t s_leaked_blocks;

	};

	using debug_mr = debug_resource;
}
