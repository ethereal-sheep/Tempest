/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "BuildModeOverlay.h"
#include "ECS/Prototypes/Prototype_Category.h"
#include "Triggers/SimulationTriggers.h"
#include "Events/Events/ScriptEvents.h"
#include <Editor/src/Actions/EditorAction.h>
#include "Util/GuizmoController.h"
#include "Util/shape_manip.h"
#include <Tempest/src/Instance/EditTimeInstance.h>
#include <Editor/src/InstanceManager/InstanceConfig.h>

namespace Tempest
{
	void BuildModeOverlay::open_popup(const Event& )
	{
		OverlayOpen = true;
		cam_ctrl.update(Service<RenderSystem>::Get().GetCamera());
		AudioEngine ae;
		MapBuilderBGM = ae.Play("Sounds2D/CoReSyS_BGM_BuildingMode.wav", "BGM", 0.7f, true);
	}

	void BuildModeOverlay::show(Instance& instance)
	{
		//const ImGuiViewport* viewport = ImGui::GetMainViewport();
		//ImGui::SetNextWindowPos(viewport->Pos);
		//ImGui::SetNextWindowSize(viewport->Size);

		banner.update(ImGui::GetIO().DeltaTime);
		if(banner.is_finished())
			banner.start(1, 0, 10);

		if (OverlayOpen)
		{
			//renderTop();
			
			renderBtm(instance);
		}
	}

	void BuildModeOverlay::renderTop()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos({ viewport->WorkPos.x,viewport->WorkPos.y });
		ImGui::SetNextWindowSize({ viewport->WorkSize.x ,viewport->Size.y * 0.1f });

		if (ImGui::Begin("Top Build Mode Screen", nullptr, window_flags))
		{
			ImGui::Button("TEST");
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}

	void BuildModeOverlay::renderBtm(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0,0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos({ viewport->WorkPos.x,viewport->WorkPos.y });
		ImGui::SetNextWindowSize(viewport->WorkSize);

		if (ImGui::Begin("Btm Build Mode Screen", nullptr, window_flags))
		{
			auto combatBtn = tex_map["Assets/CombatModeBtn.dds"];
			auto bannerTex = tex_map["Assets/BuildModeBanner.dds"];

			auto cPos = ImGui::GetCursorPos();
			auto io = ImGui::GetIO();
			//auto bbY = viewport->Size.y - swidth;
			ImVec2 MousePos = { 0,0 };
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			{
				Service<EventManager>::Get().instant_dispatch<PauseOverlayTrigger>(false);
			}
			//camEnable(instance);

			ImGui::SetCursorPos({cPos.x + ImGui::GetWindowWidth() * 0.85f, cPos.y + ImGui::GetWindowHeight() * 0.05f });

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.f));
			ImGui::BeginChild("Child", ImVec2{}, false);
			
			//if (UI::UIImageButton((void*)static_cast<size_t>(combatBtn->GetID()), ImVec2{ combatBtn->GetWidth() * 0.7f, combatBtn->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
			if (UI::UIButton_2("Save & Return", "Save & Return", ImVec2{ cPos.x + ImGui::GetWindowWidth() * 0.45f,cPos.y + ImGui::GetWindowHeight() * 0.05f }, { 0,0 }, FONT_BODY))
			{
				// go ahead
				auto fn = [&]()
				{
					auto& edit = dynamic_cast<EditTimeInstance&>(instance);
					edit.save_current_scene();
					edit.unload_current_scene();
					OverlayOpen = false;
					AudioEngine ae;
					ae.StopChannel(MapBuilderBGM);
					Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(2);
				};
				// fade in, fade out, visible
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, .0f, fn);

			}
			ImGui::EndChild();
			ImGui::PopStyleColor(1);

			ImVec2 min_pos = viewport->WorkPos;
			ImVec2 max_pos = { viewport->WorkPos.x + bannerTex->GetWidth(),viewport->WorkPos.y + bannerTex->GetHeight() };

			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bannerTex->GetID()), min_pos, max_pos, {banner.get(), 0}, {1 + banner.get(), 1});
			
			float posY = (viewport->WorkPos.y + viewport->Size.y - swidth);
			float endY = (viewport->WorkPos.y + viewport->Size.y);
			ImGui::SetCursorPos({ viewport->WorkPos.x,posY });

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
			ImGui::BeginChild("ChildTest");
			draw_splitter(posY);

			ImGui::SetNextWindowSize({ ImGui::GetWindowWidth(), endY - posY },ImGuiCond_Appearing);
			ImGui::BeginChild("PrototypeDisplay");
			for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
			{
				if (ImGui::BeginTabBar("##categorytab"))
				{
					// each folder is a category
					draw_category(instance, cat_name, proto_cat);
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();
			ImGui::EndChild();
			ImGui::PopStyleColor(1);
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);
	}

	void BuildModeOverlay::draw_splitter(float )
	{
		//ImVec2 backup_pos = ImGui::GetCursorPos();
		//ImGui::SetCursorPosY(posY + swidth);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		// We don't draw while active/pressed because as we move the panes the splitter button will be 1 frame late
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.10f));
		ImGui::Button("##Splitter", ImVec2(-1.f, border));
		ImGui::PopStyleColor(3);

		ImGui::SetItemAllowOverlap(); // This is to allow having other buttons OVER our splitter. 

		if (ImGui::IsItemActive())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
			float mouse_delta = ImGui::GetIO().MouseDelta.y;
			
			// Minimum pane size
			if (mouse_delta > swidth - min_swidth )
				mouse_delta = swidth - min_swidth ;
				
			if (mouse_delta < swidth - max_swidth)
				mouse_delta = swidth -  max_swidth  ;
				

			// Apply resize
			swidth -= mouse_delta;
		}
		else
		{
			if (ImGui::IsItemHovered())
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
			else
				ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}

		//ImGui::SetCursorPos(backup_pos);
	}
	void BuildModeOverlay::draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat)
	{
		// skip units
		if (cat_name == "Unit")
			return;

		if (ImGui::BeginTabItem(cat_name.c_str()))
		{
			for (auto& pair : proto_cat)
			{

				auto& proto_name = pair.first;
				auto& proto = pair.second;
				// each of this json files

				//// check if loaded
				//if (!categories[cat].has(name))
				//{
				//	// try to load
				//	try
				//	{
				//		categories[cat].load_file(entry.path());
				//	}
				//	catch (const std::exception&)
				//	{
				//		// do nothing if we can't
				//		// we can clean if we want as well
				//		return;
				//	}
				//}

				// display
				ImGui::PushID(proto_name.c_str());
				ImGui::BeginGroup();
				auto model = proto.get_if<tc::Model>();
				tsptr<Texture> modelTex;
				ImTextureID texID = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(0));
				if (model->texPath != "")
				{
					modelTex = tex_map[model->texPath];
					texID = (void*)static_cast<size_t>(modelTex->GetID());
				}
					
				if (ImGui::ImageButton(
					texID,
					{ icon_size, icon_size },
					ImVec2(0, 0), ImVec2(1, 1), 2
				))
					/*if(ImGui::Button(name.c_str()))*/
				{
					//inspector.select(entry.path(), cat);
				}
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + icon_size);
				ImGui::Text(proto_name.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndGroup();
				ImGui::PopID();

				//UI::DrawLine();
				if (line.draw(cat_name + proto_name,
					[]() {}, [&]()
					{
						auto& cam = Service<RenderSystem>::Get().GetCamera();

						auto& io = ImGui::GetIO();
						//Checking for mouse
						//if (io.WantCaptureMouse)
						//	return;
						const ImGuiViewport* viewport = ImGui::GetMainViewport();
						auto bbY = viewport->Size.y - swidth;
						if (io.MousePos.y >= bbY)
							return;
						auto ray = cam.GetMouseRay();
						auto start = cam.GetPosition();
						float dist = 0;
						if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
						{
							auto [it, b] = instance.scene.get_map().create(proto);
							AudioEngine ae;
							ae.Play("Sounds2D/ObjectPlacement.wav", "SFX");
							instance.selected = it->first;
							if (auto transform = it->second.force_if<tc::Transform>())
							{
								auto inter = cam.GetPosition() + ray * dist;

								if (auto shape = it->second.get_if<tc::Shape>())
								{
									const int& x = shape->x;
									const int& y = shape->y;

									auto [a_x, a_y, e_x, e_y, o_x, o_y, p_x, p_y] = shape_data_from_position(x, y, inter.x, inter.z);

									inter.x = o_x;
									inter.y = 0;
									inter.z = o_y;
								}

								transform->position = inter;
							}
							instance.action_history.Commit<CreatePrefab>(it->first);


						}
					}
				))
				{
					auto& cam = Service<RenderSystem>::Get().GetCamera();

					auto ray = cam.GetMouseRay();
					auto start = cam.GetPosition();
					float dist = 0;
					if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
					{

						auto inter = cam.GetPosition() + ray * dist;

						if (auto shape = proto.get_if<tc::Shape>())
						{
							const int& x = shape->x;
							const int& y = shape->y;

							AABB box;

							auto [a_x, a_y, b_x, b_y] = shape_bounding_with_position(x, y, inter.x, inter.z);

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

					if (ImGui::BeginPopupContextItem(proto_name.c_str()))
					{
						if (ImGui::MenuItem(ICON_FA_EDIT " Instance"))
						{
							auto [it, b] = instance.scene.get_map().create(proto);
							instance.selected = it->first;
						}

						if (auto meta = proto.get_if<tc::Meta>())
						{
							proto.specialize<tc::Meta>();
							ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
						}

						ImGui::EndPopup();

					}


					ImGui::SameLine();

			}


			ImGui::EndTabItem();
		}


	}
	void Tempest::BuildModeOverlay::camEnable(Instance& instance)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.controls(cam);
		cam_ctrl.update(cam);
		ImGuiIO& io = ImGui::GetIO();

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
				if (GC.GetInitial() != transform)
				{
					instance.action_history.Commit<TransformPrefab>(current, GC.GetInitial());
				}
				instance.action_history.Commit<DeletePrefab>(instance.scene.get_map().extract(current));
				AudioEngine ae;
				ae.Play("Sounds2D/DeleteObject.wav", "SFX");
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
		if (!io.MouseDown[0])
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
	}
}