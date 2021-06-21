#include "DiagnosticsWindow.h"
#include "Font.h"
#include "Events/EventManager.h"

namespace Tempest
{
	void DiagnosticsWindow::show(Instance& instance)
	{
		if (ImGui::Begin(window_name(), &visible, window_flags))
		{
			if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Runtime"))
				{
					Runtime();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("ECS Usage"))
				{
					ECSUsage(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Memory Usage"))
				{
					MemoryUsage(instance);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

	}

	void DiagnosticsWindow::Runtime()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("Running FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(io.Framerate).c_str());
		ShowFPSGraph();

		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		if (ImGui::Button("Test"))
			Service<EventManager>::Get().instant_dispatch<TEvent<string>>("From Debug");
	}

	void DiagnosticsWindow::ShowFPSGraph()
	{
		const int max = 144;
		const int min = 0;
		ImGuiIO& io = ImGui::GetIO();

		// dummy data
		float newValue = io.Framerate;

		fps_values.push_back(newValue);

		if (fps_values.size() > graphsize)
			fps_values.erase(fps_values.begin());


		const float mod = (max - min) * 0.15f;

		// hard coding the min max
		const std::string& text{
			std::to_string(max).c_str() + std::string{"\n\n\n\n\n\n"} + std::to_string(min).c_str()
		};
		ImGui::PlotLines(
			text.c_str(),
			fps_values.data(),
			static_cast<int>(fps_values.size()),
			0,
			nullptr,
			min - mod,
			max + mod,
			ImVec2(0, 100.0f));
	}

	void DiagnosticsWindow::ECSUsage(Instance& instance)
	{
		ImGui::Text("Total entities created:       %u", instance.ecs.size());
	}


	void DiagnosticsWindow::MemoryUsage(Instance& instance)
	{
		if (!instance.has_debug())
		{
			ImGui::Text("MEMORY DEBUG NOT ENABLED");
			return;
		}

		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text(instance.get_debug()->get_name().c_str());
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 0.25));

		if (ImGui::CollapsingHeader("Statistical Data", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			StatisticalData(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
		if (ImGui::CollapsingHeader("Allocation History", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			AllocationHistory(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
	}

	void DiagnosticsWindow::StatisticalData(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Total bytes allocated:        %u", mr.bytes_allocated());
		ImGui::Text("Total bytes deallocated:      %u", mr.bytes_deallocated());
		ImGui::Text("Highest memory usage:         %u", mr.bytes_highwater());
		ImGui::Text("Outstanding Bytes:            %u", mr.bytes_outstanding());
		ImGui::Text("Outstanding Blocks:           %u", mr.blocks_outstanding());
	}

	void DiagnosticsWindow::AllocationHistory(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Last allocated address:       %p", mr.last_allocated_address());
		ImGui::Text("Last allocated bytes:         %u", mr.last_allocated_num_bytes());
		ImGui::Text("Last allocated alignment:     %u", mr.last_allocated_alignment());

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text("Last deallocated address:     %p", mr.last_deallocated_address());
		ImGui::Text("Last deallocated bytes:       %u", mr.last_deallocated_num_bytes());
		ImGui::Text("Last deallocated alignment:   %u", mr.last_deallocated_alignment());
	}

}