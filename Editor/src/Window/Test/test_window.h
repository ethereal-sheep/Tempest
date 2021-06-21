#pragma once
#include "Instance/Instance.h"
#include "imgui/imgui.h"

namespace Tempest
{
	class test_window : public Window
	{
		const char* window_name() override
		{
			return "test_window";
		}
		
		void show(Instance& instance) override
		{
			auto& edit_time = dynamic_cast<EditTimeInstance&>(instance);


			if(ImGui::Begin(window_name(), &visible, window_flags))
			{
				ImGui::Text("HELLO WORLD!");
			}

			ImGui::End();
		}
	};
}