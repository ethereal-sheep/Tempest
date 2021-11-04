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
#include "Graphics/Basics/RenderSystem.h"
#include "Util/GuizmoController.h"
#include "CameraControls.h"
#include "Actions/EditorAction.h"


namespace Tempest
{
	class ViewportWindow : public Window
	{
		CameraControls cam_ctrl;
		id_t current = INVALID;

		const char* window_name() override
		{
			return "Viewport";
		}

		void init(Instance&) override
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

			Service<GuizmoController>::Register();
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



			}
			ImGui::End();
			ImGui::PopStyleVar(3);



			auto& cam = Service<RenderSystem>::Get().GetCamera();
			cam_ctrl.show_debug(cam);
			cam_ctrl.update(cam);


			if (io.KeyCtrl)
			{
				for (auto c : io.InputQueueCharacters)
				{
					c += 'a' - 1;
					switch (c)
					{
					case 'h':
					{
						Service<EventManager>::Get().instant_dispatch<ToggleMenuBar>();
					}
					break;
					case 'z':
					{
						instance.action_history.Undo(instance);
					}
					break;
					case 'y':
					{
						instance.action_history.Redo(instance);
					}
					break;
					case 's':
					{
						instance.action_history.Redo(instance);
					}
					break;
					case 'd':
					{
						// deselect
						current = INVALID;
					}
					break;
					default:
						break;
					}
				}
			}


			// if there is transform
			if (current != instance.selected)
			{
				if (instance.scene.get_map().exist(instance.selected) && instance.scene.get_map().get(instance.selected).has<tc::Transform>())
				{
					auto& pf = instance.scene.get_map().get(instance.selected);
					auto& transform = pf.force<tc::Transform>();
					cam_ctrl.set_world_camera();
					cam_ctrl.set_orbit_camera(cam, transform.position);
				}
				else
				{
					cam_ctrl.set_world_camera();
				}
				current = instance.selected;
			}


			if (instance.scene.get_map().exist(current) && instance.scene.get_map().get(current).has<tc::Transform>())
			{
				auto& pf = instance.scene.get_map().get(instance.selected);
				ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
				auto& GC = Service<GuizmoController>::Get();
				auto& transform = pf.force<tc::Transform>();

				cam_ctrl.set_orbit_axis(transform.position);
				GC.SetRotateSnapping(90.f);
				GC.SetTranslateSnapping(1.f);
				GC.Draw(cam, transform);

				vec3 tDelta;
				vec3 rDelta;
				vec3 sDelta;

				GC.GetDelta(tDelta, rDelta, sDelta);

				tDelta.x = std::round(tDelta.x);
				tDelta.y = 0;
				tDelta.z = std::round(tDelta.z);


				if (GC.IsEnd())
				{
					if (GC.GetInitial() != transform)
					{
						instance.action_history.Commit<TransformPrefab>(current, GC.GetInitial());
					}
				}


				for (auto c : io.InputQueueCharacters)
				{
					switch (c)
					{
					case 'w':
					{
						GC.SetOperation(GuizmoController::Operation::TRANSLATE);
						
					}
					break;
					case 'r':
					{
						GC.SetOperation(GuizmoController::Operation::ROTATE);
					}
					break;
					case 'q':
					{

					}
					break;
					default:
						break;
					}
				}


				switch (GC.GetOperation())
				{
				case Tempest::GuizmoController::Operation::TRANSLATE:
				{
					if (glm::length2(tDelta) > els::epsilon<float>)
					{
						if (io.KeyAlt)
						{
							auto [it, b] = instance.scene.get_map().create(pf);
							instance.selected = it->first;
							current = instance.selected;
							if (auto new_transform = it->second.force_if<tc::Transform>())
								new_transform->position = transform.position + tDelta;
							instance.action_history.Commit<CreatePrefab>(it->first);
							GC.ForceEnd();
						}
						else
						{
							transform.position += tDelta;
						}
						//cam_ctrl.look_at(cam, to_glvec3(transform.position));
						//transform->scale += sDelta;
					}
				}
					break;
				case Tempest::GuizmoController::Operation::ROTATE:
				{

					if (glm::length2(rDelta) > els::epsilon<float>)
					{
						//transform
						transform.rotation *= glm::angleAxis(glm::radians(rDelta.y), glm::vec3{ 0, 1, 0 });
						//cam_ctrl.look_at(cam, to_glvec3(transform.position));
						//transform->scale += sDelta;
					}
				}
					break;
				case Tempest::GuizmoController::Operation::SCALE:
				{

				}
					break;
				default:
					break;
				}



				if (auto shape = pf.get_if<tc::Shape>())
				{
					const int& x = shape->x;
					const int& y = shape->y;
					const int one = 1;

					AABB box;
					box.min.x = transform.position.x - .5f - (x - 1) / 2.f;
					box.min.z = transform.position.z - .5f - (y - 1) / 2.f;
					box.min.y = 0;

					box.max.x = transform.position.x + .5f + (x - 1) / 2.f;
					box.max.z = transform.position.z + .5f + (y - 1) / 2.f;
					box.max.y = 0;

					/*Line l;
					l.p0 = glm::vec3(-.1, 0, -.1);
					l.p1 = glm::vec3(.1, 0, .1);

					Line r;
					r.p0 = glm::vec3(-.1, 0, .1);
					r.p1 = glm::vec3(.1, 0, -.1);*/

					Service<RenderSystem>::Get().DrawLine(box, { 0.1,0.1,0.1,1 });
					//Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
					//Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });
				}
			}

			

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