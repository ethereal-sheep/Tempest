/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "PlaceUnitsOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Triggers/Triggers.h"
#include "Instance/RuntimeInstance.h"
#include "ECS/Prototypes/Prototype_Category.h"

#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include "Particles/Particles_3D/CharacterSpawnEmitter_3D.h"

namespace Tempest
{
	static float timer = 0.f;

	void draw_indicator(Instance& instance, vec3 pos)
	{
		auto height = 2.5f;
		auto amplitude = .1f;
		auto speed = 2.f;
		auto ring_speed = 2.f;
		auto spin_speed = 2.f;
		auto dice_speed = 4.f;

		pos.y = height;
		pos.y += (float)sin(timer * speed) * amplitude;

		if (auto pf = instance.scene.get_prototype_if("Unit", "Indicator"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * spin_speed, glm::vec3{ 0, -1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}
		if (auto pf = instance.scene.get_prototype_if("Unit", "IndicatorRing"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * ring_speed, glm::vec3{ 0, 1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}

		if (auto pf = instance.scene.get_prototype_if("Unit", "IndicatorDice"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * dice_speed, glm::vec3{ 0, 1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}
	}

	void PlaceUnitsOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		auto a = event_cast<OpenPlaceUnitsOverlay>(e);
		entities = a.entities;
		OpenCombat = a.openNewCombat;

		// destroy entities that are removed from the add units page
		for (const auto thisUnit : chars)
		{
			if (std::find(entities.begin(), entities.end(), thisUnit) == entities.end())
			{
				a.instance.ecs.remove<tc::Unit>(thisUnit);
				a.instance.ecs.destroy(thisUnit);
			}
		}

		chars.clear();
		selected = 0;
		tutorial_index = 0;
		for (auto id : entities)
		{
			if (!a.instance.ecs.get<tc::Character>(id).isInCombat)
				chars.emplace_back(INVALID);
			else
				chars.emplace_back(id);
		}

		window_flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.set_fixed_camera(cam, 90, 45);

		AudioEngine ae;
		ae.StopChannel(voice_line);
		voice_line = 0;
		for (int i = 0; i < 3; ++i)
			voice_played[i] = false;

	}
	void PlaceUnitsOverlay::init(Instance&)
	{
		OverlayOpen = false;

		window_flags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

		Service<EventManager>::Get().register_listener<OpenPlaceUnitsOverlay>(&PlaceUnitsOverlay::open_popup, this);

	//	chars.assign(4, INVALID);
	}


	void PlaceUnitsOverlay::show(Instance& instance [[maybe_unused]] )
	{
		if (!dynamic_cast<RuntimeInstance*>(&instance))
			return;

		auto& runtime = dynamic_cast<RuntimeInstance&>(instance);
		
		{
			float dt = ImGui::GetIO().DeltaTime;

			timer += dt;
		}


		if (OverlayOpen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
			{
				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				{
					Service<EventManager>::Get().instant_dispatch<PauseOverlayTrigger>(false, false, true);
				}

				if (instance.tutorial_enable && !instance.tutorial_temp_exit)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					//if (instance.tutorial_level != 1) //set Slide to false if not tut level 1
					//	instance.tutorial_slide = false;
					if (instance.tutorial_level == 1)
					{
						switch (tutorial_index)
						{
						case 0:
						{
							if (!voice_played[0])
							{
								AudioEngine ae;
								if (!ae.IsPlaying(voice_line))
								{
									voice_line = ae.Play("Sounds2D/C_UnitPlacement_1.wav", "VL", 1.0f);
									voice_played[0] = true;
								}
							}
							else if (!voice_played[1])
							{
								AudioEngine ae;
								if (!ae.IsPlaying(voice_line))
								{
									voice_line = ae.Play("Sounds2D/C_UnitPlacement_2.wav", "VL", 1.0f);
									voice_played[1] = true;
								}
							}
							ImVec2 pos = { 0, viewport->Size.y * 0.19f };
							ImVec2 size = { 380.f, 80.f };

							ImVec2 pos2 = { viewport->Size.x * 0.5f - 300.f, viewport->Size.y * 0.65f - 200.f };
							ImVec2 size2 = { 600.f, 400.f};

							UI::TutArea3(pos, pos2, size, size2);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to add your units into the map";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y * 0.5f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 1:
						{
							// catch
							if (!voice_played[1])
							{
								AudioEngine ae;
								if (!ae.IsPlaying(voice_line))
								{
									voice_line = ae.Play("Sounds2D/C_UnitPlacement_2.wav", "VL", 1.0f);
									voice_played[1] = true;
								}
							}
							ImVec2 pos = { 0, viewport->Size.y * 0.27f };
							ImVec2 size = { 380.f, 80.f };

							ImVec2 pos2 = { viewport->Size.x * 0.5f - 300.f, viewport->Size.y * 0.65f - 200.f };
							ImVec2 size2 = { 600.f, 400.f };

							UI::TutArea3(pos, pos2, size, size2);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to add your units into the map";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y * 0.5f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 2:
						{
							if (!voice_played[2])
							{
								AudioEngine ae;
								if (!ae.IsPlaying(voice_line))
								{
									voice_line = ae.Play("Sounds2D/C_UnitPlacement_3.wav", "VL", 1.0f);
									voice_played[2] = true;
								}
								else
									ae.StopChannel(voice_line);
							}
							ImVec2 pos = { viewport->Size.y * 0.18f, viewport->Size.y * 0.841f };
							ImVec2 size = { 200.f, 70.f };
							UI::TutArea(pos, size);
						}
						break;

						default:
							break;
						}
					}

					//Tutorial Exit Button
					if (instance.tutorial_slide == false)
					{
						auto exitBtn = tex_map["Assets/Tutorial_exit.dds"];
						ImVec2 tut_min = { viewport->Size.x * 0.85f, viewport->Size.y * 0.05f };
						ImVec2 tut_max = { tut_min.x + exitBtn->GetWidth() * 1.f, tut_min.y + exitBtn->GetHeight() * 1.f };
						drawlist->AddImage((void*)static_cast<size_t>(exitBtn->GetID()), tut_min, tut_max);

						if (UI::MouseIsWithin(tut_min, tut_max))
						{
							ImGui::SetMouseCursor(7);
							if (ImGui::IsMouseClicked(0))
							{
								instance.tutorial_temp_exit = true;
								ImGui::OpenPopup("TutorialExitPopupConfirm");
							}
						}
					}
				}

				// exit tutorial
				if (UI::ConfirmTutorialPopup("TutorialExitPopupConfirm", "Do you want to exit the tutorial?", true, [&]() {instance.tutorial_temp_exit = false; }))
				{
					instance.tutorial_temp_exit = false;
					instance.tutorial_enable = false;
					AudioEngine ae;
					ae.StopChannel(voice_line);
					voice_line = 0;
				}

				//bool selectable_hovered = false;
				auto& cam = Service<RenderSystem>::Get().GetCamera();

				cam_ctrl.controls(cam);
				cam_ctrl.update(cam);

				// character list
				/*{
					for (int i = 0; i < (int)chars.size(); ++i)
					{
						string name = "Character_";
						name += std::to_string(i);

						if (ImGui::Selectable(name.c_str(), chars[i] != INVALID))
						{
							selected = i;
						}

						if (ImGui::IsItemHovered())
							selectable_hovered = true;

						if (i == selected)
						{
							ImGui::SameLine();
							ImGui::Text(ICON_FA_CIRCLE);
						}
					}
				}*/

				auto window = ImGui::GetWindowDrawList();

				auto sideBg = tex_map["Assets/FadingSideBG.dds"];
				auto imgWidth = tex_map["Assets/SmallTurnHoverBtn.dds"]->GetWidth();
				ImVec2 sideBarSize = { imgWidth + 20.f, ImGui::GetWindowHeight() * 0.6f };
				ImVec2 sideBgMin = ImGui::GetCurrentWindow()->Pos;
				ImVec2 sideBgMax = { sideBgMin.x + (float)sideBg->GetWidth(), sideBgMin.x + (float)ImGui::GetWindowHeight() };
				ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
				ImGui::PushFont(FONT_TURN);
				string str = "PLACING UNIT";
				auto strSize = ImGui::CalcTextSize(str.c_str());
				ImGui::PopFont();

				//ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
				window->AddImage((void*)static_cast<size_t>(sideBg->GetID()), sideBgMin, sideBgMax);
				window->AddRectFilled({ sideBgMin.x, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, { sideBgMin.x + imgWidth - 20.f,  sideBgMin.y + ImGui::GetWindowHeight() * 0.1f + strSize.y }, ImGui::GetColorU32(col));

				ImGui::PushFont(FONT_TURN);
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, ImGui::GetColorU32({ 0,0,0,1 }), str.c_str());
				ImGui::PopFont();

				ImGui::PushFont(FONT_BODY);
				string str2 = "Place your selected units into the level";
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.15f }, ImGui::GetColorU32({ 1,1,1,1 }), str2.c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosY(sideBgMin.y + ImGui::GetWindowHeight() * 0.2f);
				DrawSideBar(instance, sideBarSize);
				ImVec2 buttonPos = { sideBgMin.x + 100.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.87f };
				
				auto tex = tex_map["Assets/TurnOrderBtn.dds"];
				ImGui::SetCursorPos(ImVec2{ buttonPos.x - tex->GetWidth() * 0.5f * 1.1f,  buttonPos.y - tex->GetHeight() * 0.5f * 1.1f });
				if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 1.1f, tex->GetHeight() * 1.1f }))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>(OpenCombat, chars);
				}
				if (UI::UIButton_2("Play", "Play", { buttonPos.x + 200.f, buttonPos.y }, { -40.f,20.f }, FONT_BODY))
				{
					bool okay = true;
					for (auto i : chars)
						if (i == INVALID) okay = false;
					if (okay)
					{

						// give default action for units with no actions
						for (const auto id : chars)
						{
							auto& charac = instance.ecs.get<tc::Character>(id);
							if (charac.actions.empty())
							{
								auto new_graph = instance.ecs.create();
								instance.ecs.emplace<tc::ActionGraph>(new_graph);
								instance.ecs.emplace<tc::Graph>(new_graph, "NOTHING", graph_type::action);
								charac.actions.emplace_back(new_graph);
							}
						}

						// go to combat mode
						if (OpenCombat)
						{
							Service<EventManager>::Get().instant_dispatch<PlayPlaceUnitCinematic>(instance, chars);
							AudioEngine ae;
							ae.StopChannel(voice_line);
						}
							
							
						else
						{
							Service<EventManager>::Get().instant_dispatch<ChangeTurnOrder>(chars);
							Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(true, instance);
						}
							
						m_characterSpawnEmitter.lock()->m_EM.m_burstCycle = 0;
						OverlayOpen = false;
					}
				}


				// character list
				/*if (ImGui::Button("Fight!"))
				{
					bool okay = true;
					for (auto i : chars)
						if (i == INVALID) okay = false;
					if (okay)
					{
						Service<EventManager>::Get().instant_dispatch<OpenCombatModeTrigger>(chars);
						OverlayOpen = false;
					}
				}*/

				// highlight stuff on viewport
				{
					auto ray = cam.GetMouseRay();
					auto start = cam.GetPosition();
					float dist = 0;
					if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
					{
						auto inter = cam.GetPosition() + ray * dist;

						int w_x = (int)std::round(inter.x - .5f);
						int w_y = (int)std::round(inter.z - .5f);

						id_t id = INVALID;
						if (runtime.character_map.count(w_x) && runtime.character_map[w_x].count(w_y))
							id = runtime.character_map[w_x][w_y];
						
						if (runtime.collision_map[w_x][w_y])
						{
							// draw existing collider
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

								Service<RenderSystem>::Get().DrawLine(box, { 1,0,0,1 });
							}
						}
						else if (selected >= 0 && selected < (int)chars.size())
						{
							// draw one box collider
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

							auto& io = ImGui::GetIO();
							if (io.MouseClicked[0] && !ImGui::IsAnyItemHovered())
							{
								// create or move
								if (chars[selected])
								{
									// move existing
									auto& transform = instance.ecs.get<tc::Transform>(chars[selected]);
									// take note that inter has already been processed
									transform.position = inter;

									// Instantiate the character VFX
									EmitterSystem_3D::GetInstance().CharacterSpawnEmitter(m_characterSpawnEmitter, transform.position);
								}
								else
								{
									// create proto
									// create prefab
									// then throw it into ecs

									auto random_char_id = instance.ecs.view_first<tc::Character>();
									auto proto_p = instance.scene.get_prototype_if("Unit", "Unit");
									auto prefab = proto_p ? proto_p->instance() : create_new_prototype("Unit").instance();
									auto entity = instance.ecs.create(prefab);

									AudioEngine ae;
									ae.Play("Sounds2D/ObjectPlacement.wav", "SFX");

									chars[selected] = entity;
									LOG_ASSERT(instance.ecs.has<tc::Character>(entity));
									LOG_ASSERT(instance.ecs.has<tc::Transform>(entity));
									LOG_ASSERT(instance.ecs.has<tc::Model>(entity));

									/*auto& model = instance.ecs.get<tc::Model>(entity);
									auto& pipeline = Service<RenderSystem>::Get().m_Pipeline;
									if (!pipeline.m_ModelLibrary.count(model.path))
									{
										std::shared_ptr<ModelPBR> temp = std::make_shared<ModelPBR>();
										temp->loadModel(model.path);
										pipeline.m_ModelLibrary.insert(std::make_pair(model.path, std::move(temp)));
									}
									ModelObj modelObj;
									modelObj.m_Model = pipeline.m_ModelLibrary[model.path];
									auto& ModelColors = Service<RenderSystem>::Get().ModelColors;
									for (auto i = 0; i < modelObj.m_Model->colours.size(); i++)
									{
										ModelColors[selected].push_back(modelObj.m_Model->colours[i]);
									}
									*/
									auto& transform = instance.ecs.get<tc::Transform>(entity);
									
									//auto& model = instance.ecs.get<tc::Model>(entity);

									/* ===========================================
									* NOTE TO UI!!!!
									* ADD CHARACTER SHEET HERE TO THE CHARACTER!!
									* JUST ASSIGN THE COMPONENT
									* instance.ecs.get<tc::Character>(entity) = instance.ecs.get<tc::Character>(char_sheet_id)
									* ============================================
									*/
									if (random_char_id)
										instance.ecs.get<tc::Character>(entity) = entities[selected] ? instance.ecs.get<tc::Character>(entities[selected]) : instance.ecs.get<tc::Character>(random_char_id);
									instance.ecs.get<tc::Character>(entity).isInCombat = true;
									transform.position = inter;

									EmitterSystem_3D::GetInstance().CharacterSpawnEmitter(m_characterSpawnEmitter, transform.position);

									// tutorial stuff oh dears
									if (instance.tutorial_enable)
									{
										if (selected == 0 && tutorial_index == 0)
											tutorial_index = 1;

										else if (selected == 1 && tutorial_index == 1)
											tutorial_index = 2;
									}
								}
							}
						}
					}
				}
				// highlight stuff on viewport

				// draw stuff on units
				{
					auto view = instance.ecs.view<tc::Unit, tc::Transform>();
					for (auto id : view)
					{
						auto position = instance.ecs.get<tc::Transform>(id).position;
						position.y += 2;
						auto ss = cam.WorldspaceToScreenspace(position);
						auto vp = cam.GetViewport();
						ss.x = (1 + ss.x) / 2 * vp.z;
						ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
						auto temp = ImGui::GetCursorPos();

						// Draw whatever thing on their head
						{

							/*ImGui::PushFont(FONT_BOLD);
							auto text_size = ImGui::CalcTextSize(ICON_FA_ICE_CREAM);
							auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
							if (cursor_pos.x < viewport->WorkSize.x && ss.y + text_size.y / 2 < viewport->WorkSize.y) {
								ImGui::SetCursorPos(cursor_pos);
								ImGui::Text(ICON_FA_ICE_CREAM);
							}
							ImGui::PopFont();*/

							draw_indicator(instance, position);
						}


						ImGui::SetCursorPos(temp);
					}

				}
				// draw stuff on unit heads
			}

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(2);
		}
	}

	//void PlaceUnitsOverlay::show(Instance& instance [[maybe_unused]] )
	//{
	//	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	//	ImGui::SetNextWindowPos(viewport->Pos);
	//	ImGui::SetNextWindowSize(viewport->Size);

	//	if (OverlayOpen)
	//	{
	//		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	//		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

	//		if (ImGui::Begin("Place Units Overlay", nullptr, window_flags))
	//		{
	//			auto window = ImGui::GetWindowDrawList();

	//			auto sideBg = tex_map["Assets/FadingSideBG.png"];
	//			auto imgWidth = tex_map["Assets/SmallTurnSelectedBtn.png"]->GetWidth();
	//			ImVec2 sideBarSize = { imgWidth + 20.f, ImGui::GetWindowHeight() * 0.6f };
	//			ImVec2 sideBgMin = ImGui::GetCurrentWindow()->Pos;
	//			ImVec2 sideBgMax = { sideBgMin.x + (float)sideBg->GetWidth(), sideBgMin.x + (float)ImGui::GetWindowHeight() };
	//			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
	//			ImGui::PushFont(FONT_TURN);
	//			string str = "PLACING UNIT";
	//			auto strSize = ImGui::CalcTextSize(str.c_str());
	//			ImGui::PopFont();

	//			//ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
	//			window->AddImage((void*)static_cast<size_t>(sideBg->GetID()), sideBgMin, sideBgMax);
	//			window->AddRectFilled({ sideBgMin.x, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, { sideBgMin.x + imgWidth - 20.f,  sideBgMin.y + ImGui::GetWindowHeight() * 0.1f + strSize.y  }, ImGui::GetColorU32(col));
	//			
	//			ImGui::PushFont(FONT_TURN);
	//			window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, ImGui::GetColorU32({ 0,0,0,1 }), str.c_str());
	//			ImGui::PopFont();

	//			ImGui::PushFont(FONT_BODY);
	//			string str2 = "Place your selected units into the level";
	//			window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.15f }, ImGui::GetColorU32({ 1,1,1,1 }), str2.c_str());
	//			ImGui::PopFont();

	//			ImGui::SetCursorPosY(sideBgMin.y + ImGui::GetWindowHeight() * 0.2f);
	//			DrawSideBar(instance, sideBarSize);
	//			ImVec2 buttonPos = { sideBgMin.x + 100.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.87f };
	//			if (UI::UIButton_2("Turn Order", "Turn Order", buttonPos, { -40.f,20.f }, FONT_BODY))
	//			{

	//			}
	//			if (UI::UIButton_2("Save", "Save", { buttonPos.x + 200.f, buttonPos.y }, { -40.f,20.f }, FONT_BODY))
	//			{

	//			}
	//		}
	//		ImGui::End();
	//		ImGui::PopStyleVar();
	//		ImGui::PopStyleColor();
	//	}
	//}
	void PlaceUnitsOverlay::DrawSideBar(Instance& instance, ImVec2 size)
	{
		
		if (ImGui::BeginChild("##PlaceUnitSideBar", size))
		{
			for (int i = 0; i < (int)entities.size(); ++i)
			{
				auto& character = instance.ecs.get<tc::Character>(entities[i]);

				auto unitIcon = tex_map["Assets/Unit_Black.dds"];
				ImGui::SetCursorPosX(-10.f);
				if (UI::UICharTurnButton((void*)static_cast<size_t>(unitIcon->GetID()), { (float)unitIcon->GetWidth(), (float)unitIcon->GetHeight() }, character.name.c_str(), "##PlaceUnit" + std::to_string(i), i == selected , 
					chars[i] != INVALID, false, ImVec2{ 0,0 }, ImVec2{ 1,1 }, -1, ImVec4{ 0,0,0,0 }, ImVec4{ character.color.x,character.color.y,character.color.z,1 }))
				{
					selected = i;
				}
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			}
		}
		
		
		ImGui::EndChild();

	}
}