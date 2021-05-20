
#pragma once
#include "../Core.h"
#include "Resource.h"

namespace Tempest
{
	class MemoryManager
	{

	public:
		enum class Strategy
		{
			NEW_DELETE, DEFAULT, MALLOC, MONOTONIC, UNSYNC_POOL, SYNC_POOL
		};

		[[nodiscard]] memres* request(Strategy strategy = Strategy::NEW_DELETE)
		{
			switch (strategy)
			{
			case Strategy::NEW_DELETE:
				return std::pmr::new_delete_resource();
				break;
			case Strategy::DEFAULT:
				return std::pmr::get_default_resource();
				break;
			case Strategy::MALLOC:
				return malloc_resource();
				break;
			case Strategy::MONOTONIC:
				break;
			case Strategy::UNSYNC_POOL:
				break;
			case Strategy::SYNC_POOL:
				break;
			default:
				break;
			}
		}
		

	};
}
