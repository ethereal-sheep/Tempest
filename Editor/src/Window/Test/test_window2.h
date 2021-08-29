#pragma once
#include "Instance/Instance.h"

namespace Tempest
{
	class test_window2 : public Window
	{
		const char* window_name() override
		{
			return "test_window2";
		}



		void show(Instance& instance) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				//if (ImGui::Button(buffer))
					//ax::NodeEditor::NavigateToContent();

			}
		}
	};
}