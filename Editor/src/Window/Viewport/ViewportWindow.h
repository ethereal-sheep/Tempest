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
			cam_ctrl.reset(Service<RenderSystem>::Get().GetCamera());
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
			//cam_ctrl.show_debug(cam);
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
						if (auto edit_instance = dynamic_cast<EditTimeInstance*>(&instance))
						{
							Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Saving...");
							Service<EventManager>::Get().instant_dispatch<SaveProjectTrigger>(); 
						}
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

				if (io.KeysDown[io.KeyMap[ImGuiKey_Delete]])
				{
					instance.action_history.Commit<DeletePrefab>(instance.scene.get_map().extract(current));
					instance.selected = INVALID;
					return;
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
						// check if it is still on the correct boundary


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

					int a_x = x, a_y = y, e_x = 0, e_y = 0;
					if (a_x % 2 != a_y % 2)
					{
						a_x = a_y = std::min(x, y);
						e_x = x - a_x;
						e_y = y - a_y;

					}

					box.min.x = - .5f - (a_x - 1) / 2.f;
					box.min.z = - .5f - (a_y - 1) / 2.f;

					box.max.x = + .5f + (a_x - 1) / 2.f + e_x;
					box.max.z = + .5f + (a_y - 1) / 2.f + e_y;

					auto rot = transform.rotation;
					box.min = rot * box.min;
					box.max = rot * box.max;

					box.min.x += transform.position.x;
					box.min.z += transform.position.z;
					box.min.y = 0;

					box.max.x += transform.position.x;
					box.max.z += transform.position.z;
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
			
			// highlights
			if(!io.MouseDown[0])
			{
				auto vp = cam.GetViewport();
				auto width = vp.z;
				auto height = vp.w;

				if (io.WantCaptureMouse)
					return;

				glm::vec4 lRayStart_NDC(
					((float)io.MousePos.x / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
					((float)io.MousePos.y / (float)height - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
					-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
					1.0f
				);
				glm::vec4 lRayEnd_NDC(
					((float)io.MousePos.x / (float)width - 0.5f) * 2.0f,
					((float)io.MousePos.y / (float)height - 0.5f) * -2.0f,
					0.0,
					1.0f
				);

				glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
				glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
				glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
				glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
				lRayDir_world = glm::normalize(lRayDir_world);

				auto start = cam.GetPosition();
				float dist = 0;
				if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
				{
					auto inter = cam.GetPosition() + lRayDir_world * dist;
					id_t id = instance.scene.get_map().find((int)std::round(inter.x-.5f), (int)std::round(inter.z - .5f));
					
					if (id && instance.scene.get_map().exist(id))
					{
						auto& pf = instance.scene.get_map().get(id);
						
						if (pf.has<tc::Shape>() && pf.has<tc::Transform>())
						{
							auto shape = pf.get_if<tc::Shape>();
							auto& transform = pf.get<tc::Transform>();

							const int& x = shape->x;
							const int& y = shape->y;
							const int one = 1;

							AABB box;

							int a_x = x, a_y = y, e_x = 0, e_y = 0;
							if (a_x % 2 != a_y % 2)
							{
								a_x = a_y = std::min(x, y);
								e_x = x - a_x;
								e_y = y - a_y;

							}

							box.min.x = -.5f - (a_x - 1) / 2.f;
							box.min.z = -.5f - (a_y - 1) / 2.f;

							box.max.x = +.5f + (a_x - 1) / 2.f + e_x;
							box.max.z = +.5f + (a_y - 1) / 2.f + e_y;

							auto rot = transform.rotation;
							box.min = rot * box.min;
							box.max = rot * box.max;

							box.min.x += transform.position.x;
							box.min.z += transform.position.z;
							box.min.y = 0;

							box.max.x += transform.position.x;
							box.max.z += transform.position.z;
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
					else
					{
						AABB box;

						int a_x = 1, a_y = 1, e_x = 0, e_y = 0;
						if (a_x % 2 != a_y % 2)
						{
							a_x = a_y = std::min(1, 1);
							e_x = 1 - a_x;
							e_y = 1 - a_y;

						}

						inter.x = a_x % 2 ? std::floor(inter.x) + .5f : std::round(inter.x);
						inter.y = 0;
						inter.z = a_y % 2 ? std::floor(inter.z) + .5f : std::round(inter.z);

						box.min.x = inter.x - .5f - (a_x - 1) / 2.f;
						box.min.z = inter.z - .5f - (a_y - 1) / 2.f;
						box.min.y = 0;

						box.max.x = inter.x + .5f + (a_x - 1) / 2.f + e_x;
						box.max.z = inter.z + .5f + (a_y - 1) / 2.f + e_y;
						box.max.y = 0;

						Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
					}


				}
			}
			
			// click
			if (io.MouseClicked[0])
			{
				auto vp = cam.GetViewport();
				auto width = vp.z;
				auto height = vp.w;

				if (io.WantCaptureMouse)
					return;

				glm::vec4 lRayStart_NDC(
					((float)io.MousePos.x / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
					((float)io.MousePos.y / (float)height - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
					-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
					1.0f
				);
				glm::vec4 lRayEnd_NDC(
					((float)io.MousePos.x / (float)width - 0.5f) * 2.0f,
					((float)io.MousePos.y / (float)height - 0.5f) * -2.0f,
					0.0,
					1.0f
				);

				glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
				glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
				glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
				glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
				lRayDir_world = glm::normalize(lRayDir_world);

				auto start = cam.GetPosition();
				float dist = 0;
				if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
				{
					auto inter = cam.GetPosition() + lRayDir_world * dist;
					instance.selected = instance.scene.get_map().find((int)std::round(inter.x - .5f), (int)std::round(inter.z - .5f));
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