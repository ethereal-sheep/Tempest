/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "MemoryManager.h"

namespace Tempest
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

		debug_flag = strat.debug != DebugFlag::NONE;
		switch (strat.resource_strategy)
		{
		case ResourceStrategy::NEW_DELETE:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug New/Delete Memory", std::pmr::new_delete_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
				return debug_memory_wrapper.get();
			}
			else
				return std::pmr::new_delete_resource();
		case ResourceStrategy::MALLOC:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Malloc Memory", malloc_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
				return debug_memory_wrapper.get();
			}
			else
				return malloc_resource();
		case ResourceStrategy::MONOTONIC:
			memory = make_uptr<std::pmr::monotonic_buffer_resource>(strat.block_size, std::pmr::new_delete_resource());
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Monotonic Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		case ResourceStrategy::UNSYNC_POOL:
			memory = make_uptr<std::pmr::unsynchronized_pool_resource>();
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Unsynchronized Pool Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		case ResourceStrategy::SYNC_POOL:
			memory = make_uptr<std::pmr::synchronized_pool_resource>();
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Synchronized Pool Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
				return debug_memory_wrapper.get();
			}
			else
				return memory.get();
		default: [[fallthrough]];
		case ResourceStrategy::DEFAULT:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Default Memory", std::pmr::get_default_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
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
		if (debug_memory_wrapper)
			debug_memory_wrapper.reset();
		if (memory)
			memory.reset();
	}
}