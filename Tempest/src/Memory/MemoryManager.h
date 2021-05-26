
#pragma once
#include "../Core.h"
#include "Resource.h"
#include "MemoryObject.h"

namespace Tempest
{
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
