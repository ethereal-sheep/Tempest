#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"

namespace Tempest
{
	class HierarchyWindow : public Window
	{

		const char* window_name() override
		{
			return ICON_FA_SHAPES " Entities";
		}
		void init(Instance&) override;
		void show(Instance& instance) override;

		ImGuiTextFilter filter;
	};
}