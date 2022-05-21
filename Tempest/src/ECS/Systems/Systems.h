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

#include "..\..\TMath.h"
#include "..\ECS.h"

namespace Tempest::Systems
{
	struct dynamic_system
	{
		template <typename Func>
		void operator()(Tempest::runtime_view view, Func func)
		{
			view.each(func);
		}
	};

}

namespace ts = Tempest::Systems;