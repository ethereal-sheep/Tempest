#include "MemoryManager.h"

namespace Tempest::Memory
{
	[[nodiscard]] const MemoryStrategy& MemoryManager::current_strategy() const
	{
		return strat;
	}

	[[nodiscard]] bool MemoryManager::is_debug() const
	{
		return debug_flag;
	}

	[[nodiscard]] bool MemoryManager::is_active() const
	{
		return active_flag;
	}

	[[nodiscard]] const debug_resource* MemoryManager::get_debug_resource() const
	{
		return debug_memory_wrapper.get();
	}

	[[nodiscard]] m_resource* MemoryManager::request(MemoryStrategy strategy)
	{
		if (active_flag)
			return nullptr;
		// warn here: memory in use already, do not create new memory

		// set current strategy
		strat = strategy;

		// set active to true
		active_flag = true;

		debug_flag = strat.debug;
		switch (strat.resource_strategy)
		{
		case ResourceStrategy::NEW_DELETE:
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug New/Delete Memory", std::pmr::new_delete_resource());
				return debug_memory_wrapper.get();
			}
			else
				return std::pmr::new_delete_resource();
		case ResourceStrategy::MALLOC:
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug Malloc Memory", malloc_resource());
				return debug_memory_wrapper.get();
			}
			else
				return malloc_resource();
		case ResourceStrategy::MONOTONIC:
			memory = make_ptr<std::pmr::monotonic_buffer_resource>(strat.block_size);
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug Monotonic Memory", memory.get());
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		case ResourceStrategy::UNSYNC_POOL:
			memory = make_ptr<std::pmr::unsynchronized_pool_resource>();
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug Unsynchronized Pool Memory", memory.get());
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		case ResourceStrategy::SYNC_POOL:
			memory = make_ptr<std::pmr::synchronized_pool_resource>();
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug Synchronized Pool Memory", memory.get());
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		default: [[fallthrough]];
		case ResourceStrategy::DEFAULT:
			if (debug_flag)
			{
				debug_memory_wrapper = make_ptr<debug_resource>("Debug Default Memory", std::pmr::get_default_resource());
				return debug_memory_wrapper.get();
			}
			else
				return std::pmr::get_default_resource();
		}
	}

	void MemoryManager::reclaim()
	{
		if (!active_flag)
			return;
		// warn here: no memory to reclaim

		active_flag = false;

		if (memory)
			memory.reset();
		if (debug_memory_wrapper)
			debug_memory_wrapper.reset();
	}
}