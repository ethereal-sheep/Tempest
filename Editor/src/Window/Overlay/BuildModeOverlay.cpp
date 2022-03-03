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

		for (auto& i : inter_nest)
			i.update(ImGui::GetIO().DeltaTime);

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
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->WorkPos.y });
		auto size = viewport->WorkSize - viewport->WorkSize;
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

			auto childx = 300.f;



			// if there is transform
			if (current != instance.selected)
			{
				if (instance.scene.get_map().exist(instance.selected) && instance.scene.get_map().get(instance.selected).has<tc::Transform>())
				{
					auto& pf = instance.scene.get_map().get(instance.selected);
					temp_xform = pf.get<tc::Transform>();
					inter_nest[0].start(inter_nest[0].get(), 1, .5f, 0, [](float x) { return glm::backEaseIn(x); });
				}
				else
				{
					inter_nest[0].start(inter_nest[0].get(), 0, .5f, 0, [](float x) { return glm::backEaseOut(x); });
				}
				current = instance.selected;
			}

			bool something_selected = (instance.scene.get_map().exist(current) && instance.scene.get_map().get(current).has<tc::Transform>());

			
			ImGui::SetCursorPos({ ImGui::GetWindowWidth() - childx, cPos.y + ImGui::GetWindowHeight() * 0.05f });

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // ***change opacity to 0 after done testing
			ImGui::BeginChild("Save_button_child", ImVec2{ childx, 50}, false);
			
			//if (UI::UIImageButton((void*)static_cast<size_t>(combatBtn->GetID()), ImVec2{ combatBtn->GetWidth() * 0.7f, combatBtn->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
			if (UI::UIButton_2("Save & Return", "Save & Return", ImVec2{ childx/2.f, 25 }, { 0,0 }, FONT_BODY))
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
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));


			auto child2x = 500.f;
			auto child2y = 600.f;
			place_box_size = { child2x, child2y };
			place_box = { ImGui::GetWindowWidth() - child2x - 100.f - inter_nest[0].get() * (-700.f), ImGui::GetWindowHeight() * 0.5f - child2y / 2.f };

			//
			static int selected_cat = 0;

			{

				ImGui::SetCursorPos(place_box);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.5f); // ***change opacity to 0 after done testing
				ImGui::PushStyleColor(ImGuiCol_Border, { 0.980f, 0.768f, 0.509f, 1.f });
				ImGui::BeginChild("menu_main_modal_child", ImVec2{ child2x, child2y }, true);


				int i = 0;
				for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
				{
					if (cat_name == "Unit")
						continue;
					if (i == selected_cat)
					{
						// each folder is a category
						draw_category(instance, cat_name, proto_cat);
						//ImGui::EndTabBar();
					}
					++i;
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor(1);
			}

			ImGui::PopStyleColor(1);


			{

				float child3x = 50.f;
				ImGui::SetCursorPos(
					{ ImGui::GetWindowWidth() - child2x - 100.f - child3x - inter_nest[0].get() * (-700.f), ImGui::GetWindowHeight() * 0.5f - child2y / 2.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.980f, 0.768f, 0.509f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.980f, 0.768f, 0.509f, 0.6f });
				ImGui::BeginChild("menu_side_modal_child", ImVec2{ child3x, child2y }, false); 



				tsptr<Texture> cat_btns[] = { tex_map["Assets/Dice_1.dds"], tex_map["Assets/Dice_2.dds"], tex_map["Assets/Dice_3.dds"], tex_map["Assets/Dice_4.dds"], tex_map["Assets/Dice_5.dds"] };

				int i = 0;
				for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
				{
					// skip units
					if (cat_name == "Unit")
						continue;

					if (i == selected_cat)
						ImGui::PushStyleColor(ImGuiCol_Button, { 0.980f, 0.768f, 0.509f, 1.f });
					else
						ImGui::PushStyleColor(ImGuiCol_Button, { 1, 1, 1, 0.f });
					
					
					//ImGui::ImageButton((void*)static_cast<size_t>(cat_btns[i]->GetID()), { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2); use for images
					ImGui::ImageButton(0, { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2);
					
					if(ImGui::IsItemClicked())
					{
						selected_cat = i;
					}

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s", cat_name.c_str());
						ImGui::EndTooltip();
					}


					ImGui::PopStyleColor(1);

					++i;
				}


				ImGui::EndChild();
				ImGui::PopStyleColor(2);
			}






			ImVec2 min_pos = viewport->WorkPos;
			ImVec2 max_pos = { viewport->WorkPos.x + bannerTex->GetWidth(),viewport->WorkPos.y + bannerTex->GetHeight() };

			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bannerTex->GetID()), min_pos, max_pos, {banner.get(), 0}, {1 + banner.get(), 1});
			
			


			if (something_selected)
			{
				auto& cam = Service<RenderSystem>::Get().GetCamera();
				// draw transform UI
				auto vp = cam.GetViewport();
				auto& transform = instance.scene.get_map().get(current).force<tc::Transform>();
				auto t_position = transform.position;
				//t_position.y += 2;
				/*
				auto dir = cam.GetPosition() - t_position;
				dir.y = 0.f;
				dir = glm::normalize(dir);
				t_position += dir * 1.f;*/

				auto t_ss = cam.WorldspaceToScreenspace(t_position);
				t_ss.x = (1 + t_ss.x) / 2 * vp.z;
				t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

				auto temp_cursor = ImGui::GetCursorPos();

				auto child4_box = ImVec2{ 150, 50 };
				// Draw whatever thing on their head
				{

					ImGui::PushFont(FONT_BOLD);
					//auto t_text_size = ImGui::CalcTextSize(ICON_FA_ARROW_ALT_CIRCLE_DOWN);
					auto t_cursor_pos = ImVec2{ t_ss.x - child4_box.x / 2.f, t_ss.y - child4_box.y / 2 };
					t_cursor_pos.y += 70.f;
					if (t_cursor_pos.x < viewport->WorkSize.x && t_ss.y + child4_box.y / 2 < viewport->WorkSize.y)
					{
						ImGui::SetCursorPos(t_cursor_pos);

						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.980f, 0.768f, 0.509f, 1.f });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.980f, 0.768f, 0.509f, 0.6f });
						ImGui::BeginChild("some_child", child4_box, false);

						{
							//ImGui::ImageButton((void*)static_cast<size_t>(cat_btns[i]->GetID()), { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2); use for images
							ImGui::ImageButton(0, { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2);

							if (ImGui::IsItemClicked())
							{

								instance.selected = INVALID;
							}

							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("%s", ICON_FA_TRASH);
								ImGui::EndTooltip();
							}
						}

						ImGui::SameLine();

						{

							//ImGui::ImageButton((void*)static_cast<size_t>(cat_btns[i]->GetID()), { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2); use for images
							ImGui::ImageButton(0, { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2);

							if (ImGui::IsItemClicked())
							{
								instance.selected = INVALID;
							}

							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("%s", ICON_FA_CHECK);
								ImGui::EndTooltip();
							}
						}

						ImGui::SameLine();

						{

							//ImGui::ImageButton((void*)static_cast<size_t>(cat_btns[i]->GetID()), { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2); use for images
							ImGui::ImageButton(0, { 40, 40 }, ImVec2(0, 0), ImVec2(1, 1), 2);

							if (ImGui::IsItemClicked())
							{
								transform.rotation *= glm::angleAxis(glm::radians(90.f), glm::vec3{ 0, 1, 0 });
							}

							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("%s", ICON_FA_SYNC_ALT);
								ImGui::EndTooltip();
							}
						}


						ImGui::EndChild();
					}
					
					ImGui::PopFont();
				}
				ImGui::SetCursorPos(temp_cursor);
			}


			/*float posY = (viewport->WorkPos.y + viewport->Size.y - swidth);
			float endY = (viewport->WorkPos.y + viewport->Size.y);
			ImGui::SetCursorPos({ viewport->WorkPos.x,posY });*/
			//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
			//ImGui::BeginChild("ChildTest");
			//draw_splitter(posY);

			//ImGui::SetNextWindowSize({ ImGui::GetWindowWidth(), endY - posY },ImGuiCond_Appearing);
			//ImGui::BeginChild("PrototypeDisplay");
			//for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
			//{
			//	if (ImGui::BeginTabBar("##categorytab"))
			//	{
			//		// each folder is a category
			//		draw_category(instance, cat_name, proto_cat);
			//		ImGui::EndTabBar();
			//	}
			//}
			//ImGui::EndChild();
			//ImGui::EndChild();
			//ImGui::PopStyleColor(1);
		}
		ImGui::End();

		//ImGui::PopStyleVar(2);
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

		//if (ImGui::BeginTabItem(cat_name.c_str()))
		{
			const auto xpadding = 12.f;
			const auto ypadding = 12.f;
			const auto full_space = 500.f;
			auto spacing = 24.f;
			unsigned cols = 4u;

			//assume spacing between items is 15

			spacing = std::max(15.f, spacing);
			cols = std::max(1u, cols);
			const auto space = full_space - xpadding - xpadding - cols * spacing - 30.f;
			int i = 0;


			// filter
			ImGui::Dummy({ 0.01f, 45.f });
			{
				ImGui::Dummy({ 50.f, 0.01f });
				ImGui::SameLine();

				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
				ImGui::PushStyleColor(ImGuiCol_Border, { 0.980f, 0.768f, 0.509f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0, 0, 0, 1.f });
				filter.Draw("##prefab_filter");
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar();

				ImGui::SameLine();
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Filter usage:\n"
					"  \"\"         display all lines\n"
					"  \"xxx\"      display lines containing \"xxx\"\n"
					"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
					"  \"-xxx\"     hide lines containing \"xxx\"", false);

			}
			ImGui::Dummy({ 0.01f, 5.f });

			ImGui::Dummy({ 0.01f, ypadding });
			for (auto& pair : proto_cat)
			{

				if (i % cols == 0)
				{
					ImGui::Dummy({ xpadding, 0.01f });
					ImGui::SameLine();
				}

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
				if (filter.PassFilter(proto_name.c_str()))
				{

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
						{ space / cols, space / cols },
						ImVec2(0, 0), ImVec2(1, 1), 0
					))
						/*if(ImGui::Button(name.c_str()))*/
					{
						//inspector.select(entry.path(), cat);
					}


					//ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + icon_size);
					//ImGui::Text(proto_name.c_str());
					//ImGui::PopTextWrapPos();
					ImGui::EndGroup();
					ImGui::PopID();

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("%s", proto_name.c_str());
						ImGui::EndTooltip();
					}

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
							if (ImGui::IsMouseHoveringRect(place_box, place_box + place_box_size))
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

								if (model)
								{
									tc::Transform t;

									[[maybe_unused]]auto [_1, _2, _3, _4, o_x, o_y, p_x, p_y] = shape_data_from_position(x, y, inter.x, inter.z);

									t.position.x = o_x;
									t.position.y = 0;
									t.position.z = o_y;

									auto transform = &t;

									auto local = proto.get_if<tc::Local>();

									auto test = glm::translate(transform->position)
										* glm::mat4(transform->rotation)
										* glm::translate(local->local_position)
										* glm::mat4(local->local_rotation)
										* glm::scale(local->local_scale)
										* glm::scale(transform->scale);

									std::filesystem::path p{ model->path };
									if (strcmp(p.extension().string().c_str(), ".a"))
									{
										p.replace_extension(".a");
									}
									Service<RenderSystem>::Get().SubmitModel(p.string(), test);


									Service<RenderSystem>::Get().SubmitModel((instance.get_full_path() / p.string()).string(), t);
								}
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

					++i;
					if (i % cols)
					{
						ImGui::SameLine();
						ImGui::Dummy({ spacing - 15.f, 0.01f });
						ImGui::SameLine();
					}
					else
					{
						ImGui::Dummy({ 0.01f, spacing - 10.f });
					}
				}
			}


			//ImGui::EndTabItem();
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
				cam_ctrl.set_world_camera(cam);
				cam_ctrl.set_orbit_camera(cam, transform.position);
			}
			else
			{
				cam_ctrl.set_world_camera(cam);
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