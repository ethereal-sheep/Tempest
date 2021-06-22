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
		void init() override;
		void show(Instance& instance) override;
	};
}