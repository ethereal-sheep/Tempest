/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"

namespace Tempest
{
	class InspectorWindow : public Window
	{
		const char* window_name() override
		{
			return ICON_FA_SEARCH " Inspector";
		}
		void init(Instance&) override;
		void show(Instance& instance) override;
	};
}