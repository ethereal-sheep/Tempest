#include "DiagnosticsWindow.h"
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Graphics/Basics/RenderSystem.h"

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
					Runtime(instance);
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
				if (ImGui::BeginTabItem("Camera"))
				{
					Camera(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Mouse"))
				{
					Mouse(instance);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

	}

	void DiagnosticsWindow::Runtime(Instance& instance)
	{
		UI::RenderText("Project Name:", 100.f); ImGui::Text(instance.get_name().c_str());
		UI::RenderText("Project Path:", 100.f); ImGui::Text(instance.get_path().string().c_str());
		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("Running FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(io.Framerate).c_str());
		ShowFPSGraph();

		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		if (ImGui::Button("Trigger Error"))
			Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Error triggered by Diagnostics!");
		if (ImGui::Button("Trigger TEvent<string>"))
			Service<EventManager>::Get().instant_dispatch<TEvent<string>>("Error triggered by Diagnostics!");
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

	void DiagnosticsWindow::Camera(Instance&)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		const auto padding = 80.f;
		{

			auto pos = cam.GetPosition();

			if(UI::DragFloat3ColorBox("Position", "##CameraPosDrag", ImVec2{ padding , 0.f }, value_ptr(pos), 0.f, 0.1f))
				cam.SetPosition(pos);
		}

		{
			auto rotation = cam.GetQuatRotation();
			auto eulerDeg = glm::degrees(glm::eulerAngles(rotation));

			if (UI::DragFloat3ColorBox("Rotation", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(eulerDeg), 0.f, 1.f))
			{
				//cam.SetRotation(eulerDeg);
			}
		}
		{
			auto q = cam.GetQuatRotation();

			float roll = (float)atan2(2.0f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
			float pitch = (float)asin(-2.0f * (q.x * q.z - q.w * q.y));
			float yaw = (float)atan2(2.0f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);

			ImGui::Selectable("Yaw", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Yaw");
			if (ImGui::DragFloat(" ", &yaw, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Pitch", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Pitch");
			if (ImGui::DragFloat(" ", &pitch, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Roll", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Roll");
			if (ImGui::DragFloat(" ", &roll, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
		}
		{
			

			auto up = cam.GetUp();
			auto front = cam.GetFront();
			auto left = cam.GetLeft();
			if (UI::DragFloat3ColorBox("up", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(up), 0.f, 1.f)) {}
			if (UI::DragFloat3ColorBox("front", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(front), 0.f, 1.f)) {}
			if (UI::DragFloat3ColorBox("left", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(left), 0.f, 1.f)) {}

		}
	}

	void DiagnosticsWindow::Mouse(Instance& )
	{
		{
			ImGuiIO& io = ImGui::GetIO();

			auto& cam = Service<RenderSystem>::Get().GetCamera();

			//DEBUG FUNTION
			if (ImGui::IsMousePosValid())
			{
				ImGui::Text("ImGui Mouse Position: %.3f , %.3f", io.MousePos.x, io.MousePos.y);
				glm::vec4 lRayStart_NDC(
					((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
					((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
					-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
					1.0f
				);
				glm::vec4 lRayEnd_NDC(
					((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f,
					((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f,
					0.0,
					1.0f
				);

				glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
				glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
				glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;
				glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
				lRayDir_world = glm::normalize(lRayDir_world);
				ImGui::Text("Ray: %.3f , %.3f,  %.3f", lRayDir_world.x, lRayDir_world.y, lRayDir_world.z);
			}
			else
				ImGui::Text("Mouse pos: <INVALID>");

		}
	}

}