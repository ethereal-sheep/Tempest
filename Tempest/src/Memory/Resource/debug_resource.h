
/******************************************************************************
 * class    MemoryManager
 *
 * Done
 * 1. Basic Memory Statistics
 * 		- Current allocated bytes
 * 		- Current allocated blocks
 * 		- total allocated bytes
 * 		- total dellcated bytes
 * 2. Basic Memory Leak checks
 *      - address and number of bytes leaked
 * 3. Basic Settings
 * 		- Set Manager Instance name
 * 		- Set verbose mode
 * 4. Basic History
 * 		- last allocation
 *      	- number of bytes
 *      	- address
 *      	- alignment
 * 		- last deallocation
 *      	- number of bytes
 *      	- address
 *      	- alignment
 * 5. Basic check for corrupted memory
 * 		- buffer overrun/underrun
 *
 *
 * Backlog Functionality
 * 1. Defragmentation X
 * 2. Setting Policy(?) X // changing default memory resource in runtime
 *                        // can cause errors. customizing an allocation
 *                        // strategy should be the approach
 *
 *                        // memory resource should differ for both
 *                        // debugging(within editor) and game (release)
 *      - multiple allocation pools
 *          - Pool Allocator
 *          - Monotonic Allocator
 *
 * 3. Check Memory leaks
 *          - location in code X
 *
 * 5. throw exceptions(?) X (debug break)
 * 9. setting allocation limit X ()
 * 10.
 *
 * Notes:
 * Current solution is slower than test_resource impl due to the find in
 * deallocate. Storing meta data in header might be a better solution but
 * still requires a record storing mechanism for memory leaks
 *
 * Alternatively can use a set to store the record. But records will no longer
 * be indexed.
 *
 * Decision on overloading global new/delete to use MemMan
 * - overloading global new/delete means all allocations passes through MemMan
 * - Good to see every single allocation
 * - Good for checking every single aspect of Cardinal
 * - Will be slower due to additional checks per alloc/deallocation
 * - Memory Footprint will be larger due to padding of Memory Blocks
 *
 * Or only use MemMan for specific containers that concern game
 *  (entities, prefabs, particles,...)
 * - Slightly faster as only game related stuff will use it
 * - Leaks caused by non-game stuff will not be caught
 *   (editor, UI)
 *
******************************************************************************/
#pragma once
#include "..\..\Core.h"

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
		m_resource* upstream_resource() const { return m_upstream; }

		// get/set for meta data
		std::string get_name() const noexcept { return m_name; }
		void set_name(const std::string& name) noexcept { m_name = name; }
		bool is_verbose() const noexcept { return m_verbose_flag; }
		void set_verbose(bool is_verbose) noexcept { m_verbose_flag = is_verbose; }
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

		// metadata
		std::string m_name;

		// flags
		std::atomic_int m_verbose_flag{ false };
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

		std::pmr::vector<allocation_rec> m_blocks;
		m_resource* m_upstream; // the upstream memory resource

		static std::atomic_size_t s_leaked_bytes;
		static std::atomic_size_t s_leaked_blocks;
	};

	using debug_mr = debug_resource;
}
