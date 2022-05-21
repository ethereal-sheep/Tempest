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
