/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "MemoryObject.h"

namespace Tempest
{
	m_resource* MemoryObject::get()
	{
		if (debug_flag)
			return debug_memory_wrapper.get();;

		switch (strat.resource_strategy)
		{
		case ResourceStrategy::NEW_DELETE:
			return std::pmr::new_delete_resource();
		case ResourceStrategy::MALLOC:
			return malloc_resource();
		case ResourceStrategy::MONOTONIC: [[fallthrough]];
		case ResourceStrategy::UNSYNC_POOL: [[fallthrough]];
		case ResourceStrategy::SYNC_POOL:
			return memory.get();
		default: [[fallthrough]];
		case ResourceStrategy::DEFAULT:
			return std::pmr::get_default_resource();
		}


	}
	const debug_resource* MemoryObject::get_debug() const
	{
		return debug_memory_wrapper.get();
	}
	bool MemoryObject::has_debug() const
	{
		return debug_flag;
	}
	const MemoryStrategy& MemoryObject::get_strategy() const
	{
		return strat;
	}
	MemoryObject::~MemoryObject()
	{
		if (debug_memory_wrapper)
			debug_memory_wrapper.reset();
		if (memory)
			memory.reset();
	}
	MemoryObject::MemoryObject(MemoryStrategy strategy) : strat{ strategy }
	{
		debug_flag = strat.debug != DebugFlag::NONE;
		switch (strat.resource_strategy)
		{
		case ResourceStrategy::NEW_DELETE:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug New/Delete Memory", std::pmr::new_delete_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		case ResourceStrategy::MALLOC:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Malloc Memory", malloc_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		case ResourceStrategy::MONOTONIC:
			memory = make_uptr<std::pmr::monotonic_buffer_resource>(strat.block_size, std::pmr::new_delete_resource());
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Monotonic Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		case ResourceStrategy::UNSYNC_POOL:
			memory = make_uptr<std::pmr::unsynchronized_pool_resource>(std::pmr::new_delete_resource());
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Unsynchronized Pool Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		case ResourceStrategy::SYNC_POOL:
			memory = make_uptr<std::pmr::synchronized_pool_resource>(std::pmr::new_delete_resource());
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Synchronized Pool Memory", memory.get());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		default: [[fallthrough]];
		case ResourceStrategy::DEFAULT:
			if (debug_flag)
			{
				debug_memory_wrapper = make_uptr<debug_resource>("Debug Default Memory", std::pmr::get_default_resource());
				debug_memory_wrapper->set_verbose(strat.debug == DebugFlag::DEBUG_VERBOSE);
				debug_memory_wrapper->set_strict(strat.debug == DebugFlag::DEBUG_STRICT);
			}
			break;
		}
	}
}