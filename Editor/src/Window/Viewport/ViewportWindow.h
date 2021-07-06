#pragma once
#include "Instance/Instance.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	class ViewportWindow : public Window
	{
		const char* window_name() override
		{
			return "Viewport";
		}

		void init() override
		{
			window_flags |=
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground 
				;
		}

		void show(Instance& instance) override
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::SetNextWindowDockID(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Viewport", nullptr, window_flags))
			{
				// DockSpace
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGui::DockSpace(
						dockspace_id, 
						ImVec2(0.0f, 0.0f), 
						ImGuiDockNodeFlags_PassthruCentralNode | 
						ImGuiDockNodeFlags_NoDockingInCentralNode);
				}

				auto& cam = Service<RenderSystem>::Get().GetCamera();
				if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !io.WantCaptureMouse)
				{
					auto direction = els::to_vec2(io.MouseDelta);
					auto yaw_speed = 1.f / 4.f;
					auto pitch_speed = 1.f / 4.f;
					auto pan_speed = 1.f / 4.f;
					auto forward_speed = 1.f / 4.f;
					auto scroll_speed = forward_speed * 2.f;

					if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
					{
						//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
						//(m_SavedMousePos.x, m_SavedMousePos.y);
					}

					if (io.MouseDown[0] && io.MouseDown[1])
					{
						// Do Nothing UwU
					}
					else if (io.MouseDown[0])
					{
						if (!els::is_zero(direction))
						{
							auto rot = cam.GetQuatRotation();
							auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
							rot = rot * yaw;
							cam.SetRotation(rot);

							auto currentPos = cam.GetPosition();
							auto forward = glm::normalize(glm::cross(glm::vec3{ 0, 1, 0 }, cam.GetLeft())) * io.MouseDelta.y;
							auto newPos = currentPos + forward * forward_speed;
							cam.SetPosition(newPos);
							//cam.Pitch(to_rad(asin(io.MouseDelta.y / els::length(els::vec2{ io.MouseDelta.x, io.MouseDelta.y }))));
						}
					}
					else if (io.MouseDown[1])
					{
						if (!els::is_zero(direction))
						{
							auto rot = cam.GetQuatRotation();

							auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
							rot = rot * yaw;
							cam.SetRotation(rot);
							auto pitch = glm::angleAxis(to_rad(pitch_speed * -io.MouseDelta.y), cam.GetLeft());
							rot = rot * pitch;
							cam.SetRotation(rot);
						}
					}
					else if (io.MouseDown[2]) // Pan
					{
						if (!els::is_zero(direction))
						{

							auto up = glm::vec3{ 0, 1, 0 };
							auto right = cam.GetLeft();

							auto currentPos = cam.GetPosition();
							auto worldSpaceDirection = glm::normalize(up * direction.y + right * direction.x);
							auto newPos = currentPos - worldSpaceDirection * pan_speed;

							cam.SetPosition(newPos);
						}
					}

					cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
				}



			}
			ImGui::End();
			ImGui::PopStyleVar(3);

			// change this to instance cam

			



			// WIP for transparent child window in viewport

			//ImGuiWindowClass window_class;
			//window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_PassthruCentralNode;
			//ImGui::SetNextWindowClass(&window_class);
			/*ImGui::SetNextWindowDockID(dockspace_id);
			ImGui::SetNextWindowBgAlpha(0.f);
			if (ImGui::Begin("123", nullptr, ImGuiWindowFlags_NoBackground))
			{

				ImGuiID internal_dock = ImGui::GetID("JustForFun");
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGui::DockSpace(
						internal_dock,
						ImVec2(0.0f, 0.0f),
						ImGuiDockNodeFlags_PassthruCentralNode |
						ImGuiDockNodeFlags_NoDockingInCentralNode);
				}


				ImGui::SetCursorPos({ 5.f,5.f });
				ImGui::Text("HELLO");

			}*/

			/*ImGui::SetNextWindowDockID(dockspace_id);

			ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGuiWindowFlags host_window_flags = 0;
			host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_NoMove;
			host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				host_window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Window", nullptr, host_window_flags);
			ImGui::PopStyleVar(3);

			auto* central_node = ImGui::DockBuilderGetCentralNode(dockspace_id);

			auto x = central_node->Pos.x;
			auto y = central_node->Pos.y;
			auto width = central_node->Size.x;
			auto height = central_node->Size.y;

			ImGui::SetCursorPos({ x + 5.f, y + 5.f });
			ImGui::Text("HELLO");
			ImGui::Text("HELLO");

			ImGuiID internal_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(internal_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
			ImGui::End();*/




		}
	};
}