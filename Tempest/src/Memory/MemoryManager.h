
#pragma once
#include "../Core.h"
#include "Resource.h"

namespace Tempest::Memory
{
	enum class ResourceStrategy
	{
		DEFAULT, NEW_DELETE, MALLOC, MONOTONIC, UNSYNC_POOL, SYNC_POOL
	};
	struct MemoryStrategy
	{
		bool debug = false;
		ResourceStrategy resource_strategy;
		size_t block_size;

		MemoryStrategy(bool debug_f = false, ResourceStrategy strategy = ResourceStrategy::DEFAULT, size_t block_s = (size_t)1e6) 
			: debug(debug_f), resource_strategy(strategy), block_size(block_s) {}

	};

	class MemoryManager
	{

	public:
		[[nodiscard]] const MemoryStrategy& current_strategy() const;

		[[nodiscard]] bool is_debug() const;

		[[nodiscard]] bool is_active() const;

		[[nodiscard]] const debug_resource* get_debug_resource() const;

		[[nodiscard]] m_resource* request(MemoryStrategy strategy = {});

		void reclaim();

	private:
		bool debug_flag = false;
		bool active_flag = false;

		MemoryStrategy strat;
		tuptr<debug_resource> debug_memory_wrapper;
		tuptr<m_resource> memory;

	};


}

namespace tmem = Tempest::Memory;
