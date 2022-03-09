/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
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
#include "Util/shape_manip.h"
#include "stb_image_write.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

#include "Particles/Particles_3D/ParticleSystem_3D.h"

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
			cam_ctrl.controls(cam);
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
							edit_instance->save();
						}
					}
					break;
					case 'd':
					{
						// deselect
						current = INVALID;
					}
					break;
					case 'l':
					{
						//ParticleSystem_3D::GetInstance().CreateTestModelShapeEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f });
						
						ParticleSystem_3D::GetInstance().CreateExplosionEmitter(glm::vec3{ 5.0f, 0.0f, 5.0f });
						//ParticleSystem_3D::GetInstance().CreateChracterDamageEmitter(glm::vec3{ 5.0f, 0.0f, 5.0f });
						//ParticleSystem_3D::GetInstance().CreateMultipleExplosionEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 2.0f, 2.0f, 2.0f }, 3);
						
						//auto emitter = ParticleSystem_3D::GetInstance().CreateUnitTrailEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f });

						//ParticleSystem_3D::GetInstance().CreateTileWaypointEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f });
						//ParticleSystem_3D::GetInstance().CreateInteractiveParticle(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 5.0f, 5.0f, 0.0f });
					}
					break;
					case 'o':
					{
						auto& AAgridShow = Service<RenderSystem>::Get().AAgridShow;
						AAgridShow = !AAgridShow;
						auto& saoMode = Service<RenderSystem>::Get().saoMode;
						saoMode = !saoMode;
						for (auto& window : instance.window_manager.get_windows())
						{
							window->visible = !window->visible;
						}
					}
					break;
					case 'p':
					{						
						vec2 size = vec2(viewport->Size.x, viewport->Size.y);
						//auto& AAgridShow = Service<RenderSystem>::Get().AAgridShow;
						unsigned char* buffer = new unsigned char[static_cast<size_t>(size.x * size.y * 3)];
						glReadPixels(0, 0, (GLsizei)size.x, (GLsizei)size.y, GL_RGB, GL_UNSIGNED_BYTE, buffer);
						stbi_flip_vertically_on_write(1);
						//stbi_write_jpg("test.jpg", size.y, size.y, 3, buffer, size.y * 3);
						stbi_write_jpg("testtests.jpg", (int)size.x, (int)size.y, 3, buffer, (int)size.x * 3);
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
					if (GC.GetInitial() != transform)
					{
						instance.action_history.Commit<TransformPrefab>(current, GC.GetInitial());
					}
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

					AABB box;

					auto [a_x, a_y, b_x, b_y] = shape_bounding_for_rotation(x, y);

					box.min.x = a_x;
					box.min.z = a_y;

					box.max.x = b_x;
					box.max.z = b_y;

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
				if (auto wall = pf.get_if<tc::Wall>())
				{
					auto shape = pf.get_if<tc::Shape>();
					const int& x = shape->x;
					const int& y = shape->y;

					vec3 s, e;

					e.x = .5f;
					e.z = .5f;

					s.x = y ? -.5f : .5f;
					s.z = x ? -.5f : .5f;


					auto rot = transform.rotation;
					//s = glm::rotateY(s, glm::pi<float>()/2.f);
					//e = glm::rotateY(e, glm::pi<float>()/2.f);

					s = rot * s;
					e = rot * e;

					s.x += transform.position.x;
					s.z += transform.position.z;
					s.y = 0;

					e.x += transform.position.x;
					e.z += transform.position.z;
					e.y = 0;


					Line l;
					l.p0 = s;
					l.p1 = e;

					/*Line r;
					r.p0 = glm::vec3(-.1, 0, .1);
					r.p1 = glm::vec3(.1, 0, -.1);*/

					//Service<RenderSystem>::Get().DrawLine(box, { 0.1,0.1,0.1,1 });
					Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
					//Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });
				}
			}
			
			// highlights
			if(!io.MouseDown[0])
			{
				if (io.WantCaptureMouse)
					return;

				auto ray = cam.GetMouseRay();
				auto start = cam.GetPosition();
				float dist = 0;
				if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
				{
					auto inter = cam.GetPosition() + ray * dist;
					id_t id = instance.scene.get_map().find((int)std::round(inter.x - .5f), (int)std::round(inter.z - .5f));

					float r_x = std::round(inter.x) - inter.x;
					float r_y = std::round(inter.z) - inter.z;

					if (abs(r_x) < 0.1f)
					{
						auto c_y = (int)std::round(inter.z - .5f);
						auto a_x = (int)std::round(inter.x);
						auto b_x = (int)std::round(inter.x) - 1;
						if(auto check = instance.scene.get_map().find_edge(a_x, c_y, b_x, c_y))
							id = check;
						
					}
					else if (abs(r_y) < 0.1f)
					{
						auto c_x = (int)std::round(inter.x - .5f);
						auto a_y = (int)std::round(inter.z);
						auto b_y = (int)std::round(inter.z) - 1;
						if(auto check = instance.scene.get_map().find_edge(c_x, a_y, c_x, b_y))
							id = check;
					}

					//instance.scene.get_map().find_edge();

					if (id && instance.scene.get_map().exist(id))
					{
						auto& pf = instance.scene.get_map().get(id);
						
						if (pf.has<tc::Shape>() && pf.has<tc::Transform>())
						{
							auto shape = pf.get_if<tc::Shape>();
							auto& transform = pf.get<tc::Transform>();

							const int& x = shape->x;
							const int& y = shape->y;

							AABB box;

							auto [a_x, a_y, b_x, b_y] = shape_bounding_for_rotation(x, y);

							box.min.x = a_x;
							box.min.z = a_y;

							box.max.x = b_x;
							box.max.z = b_y;

							auto rot = transform.rotation;
							box.min = rot * box.min;
							box.max = rot * box.max;

							box.min.x += transform.position.x;
							box.min.z += transform.position.z;
							box.min.y = 0;

							box.max.x += transform.position.x;
							box.max.z += transform.position.z;
							box.max.y = 0;

							Service<RenderSystem>::Get().DrawLine(box, { 0.1,0.1,0.1,1 });
						}
					}
					else
					{
						AABB box;

						auto [a_x, a_y, b_x, b_y] = shape_bounding_with_position(1, 1, inter.x, inter.z);

						box.min.x = a_x;
						box.min.z = a_y;
						box.min.y = 0;

						box.max.x = b_x;
						box.max.z = b_y;
						box.max.y = 0;

						Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
					}


				}
			}
			
			// click
			if (io.MouseClicked[0])
			{
				if (io.WantCaptureMouse)
					return;

				auto ray = cam.GetMouseRay();
				auto start = cam.GetPosition();
				float dist = 0;
				if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
				{
					auto inter = cam.GetPosition() + ray * dist;
					id_t id = instance.scene.get_map().find((int)std::round(inter.x - .5f), (int)std::round(inter.z - .5f));

					float r_x = std::round(inter.x) - inter.x;
					float r_y = std::round(inter.z) - inter.z;

					if (abs(r_x) < 0.1f)
					{
						auto c_y = (int)std::round(inter.z - .5f);
						auto a_x = (int)std::round(inter.x);
						auto b_x = (int)std::round(inter.x) - 1;
						if (auto check = instance.scene.get_map().find_edge(a_x, c_y, b_x, c_y))
							id = check;

					}
					else if (abs(r_y) < 0.1f)
					{
						auto c_x = (int)std::round(inter.x - .5f);
						auto a_y = (int)std::round(inter.z);
						auto b_y = (int)std::round(inter.z) - 1;
						if (auto check = instance.scene.get_map().find_edge(c_x, a_y, c_x, b_y))
							id = check;
					}

					instance.selected = id;
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