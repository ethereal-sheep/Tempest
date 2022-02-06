/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "WeaponSheetOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"

#include "../../Tempest/src/Particles/ParticleSystem_2D.h"

namespace Tempest
{
	// pass in instance 
	void WeaponSheetOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		Tabs[TABS_TYPE::WEAPON].current_state = TabImageData::STATE::UNHOVER;
		Tabs[TABS_TYPE::USE].current_state = TabImageData::STATE::UNHOVER;

		auto a = event_cast<OpenWeaponSheetTrigger>(e);

		if (a.addUnit)
		{
			create_new_weapon(a.instance);
		}

		// get the first weapon
		else if (a.entity != UNDEFINED)
		{
			SelectedID = a.entity;
		}

		else
		{
			SelectedID = a.instance.ecs.view_first<Components::Weapon>(exclude_t<tc::Destroyed>());
		}

		if (SelectedID != INVALID && SelectedID != UNDEFINED)
		{
			weap = a.instance.ecs.get_if<tc::Weapon>(SelectedID);

			if (weap)
				Tabs[TABS_TYPE::WEAPON].is_active = true;
		}

		tutorial_index = 0;
		particle_0 = false;
		particle_1 = false;
		particle_2 = false;
		particle_3 = false;

		inter.start(-0.1f, 0.02f, .25f, 0, [](float x) { return glm::cubicEaseOut(x); }); // back
		inter_nest[0].start(0.5f, .15f, .4f, 0, [](float x) { return glm::cubicEaseOut(x); }); // weapons 
	}

	void WeaponSheetOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::WEAPONS)
		{
			OverlayOpen = false;
			weap = nullptr;
		}
	}

	void WeaponSheetOverlay::force_close(const Event& e)
	{
		OverlayOpen = false;
		weap = nullptr;
	}

	void WeaponSheetOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		{
			float dt = ImGui::GetIO().DeltaTime;
			for (auto& i : inter_nest)
				i.update(dt);

			inter.update(dt);
		}
		if (OverlayOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Edit Weapon Sheet", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/WeaponsBG.dds"];

				ImVec2 point{ 0,0 };
				{
					ImVec2 Min{ point.x, point.y };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Editing Weapon");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// Display the created units
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f, viewport->Size.y * inter_nest[0].get() });

				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				ImGui::BeginChild("##WeaponsDisplay", { viewport->Size.x * 0.12f, viewport->Size.y * 0.7f }, true);

				{
					unsigned i = 0;
					auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());

					// TODO: store selected item
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() + 60 };
					for (auto id : view)
					{
						auto& weapon = instance.ecs.get<tc::Weapon>(id);
						auto PairResult = UI::UIButtonWithDelete(weapon.name.c_str(), string("##"+std::to_string(i)), {cursor.x , cursor.y + i++ * 90}, {10,20}, FONT_PARA, SelectedID == id);
						if (PairResult.first)
						{
							SelectedID = id;
							weap = &weapon;
						}
						if (PairResult.second)
						{
							ImGui::OpenPopup(string("DeleteWeapon##" + std::to_string(i)).c_str() );
						}

						if (UI::ConfirmDeletePopup(string("DeleteWeapon##" + std::to_string(i)).c_str(), "Delete this weapon?"))
						{
							auto charac_view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());
							for (auto charac_id : charac_view)
							{
								auto& charac = instance.ecs.get<tc::Character>(charac_id);
								charac.weapons.erase(std::remove(charac.weapons.begin(), charac.weapons.end(), id), charac.weapons.end());
							}

							instance.ecs.emplace<tc::Destroyed>(id);
							SelectedID = UNDEFINED;
							weap = nullptr;

							Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
							Tabs[CurrentTab].is_active = false;
						}
					}
					ImGui::PushFont(FONT_HEAD);
					auto pSize = ImGui::CalcTextSize("+");
					ImGui::PopFont();
					if (UI::UIButton_1("+", "+", { ImGui::GetCursorPosX() + 99,  ImGui::GetCursorPosY() + 80 }, { 140,-5 }, FONT_HEAD))
					{
						create_new_weapon(instance);
						weap = instance.ecs.get_if<tc::Weapon>(SelectedID);
						Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
						Tabs[CurrentTab].is_active = false;
						Tabs[TABS_TYPE::WEAPON].is_active = true;

						CurrentTab = TABS_TYPE::WEAPON;

						//Tutorial progression
						if (instance.tutorial_enable && tutorial_index == 0)
							tutorial_index = 1;
					}
					if (instance.tutorial_enable && tutorial_index == 0)
					{
						if (particle_0 == false)
						{
							particle_0 = true;

							glm::vec2 real_buttonSize;
							real_buttonSize.x = pSize.x + 140 + 8;
							real_buttonSize.y = pSize.y + 20.f;

							glm::vec2 real_mousePosition;
							real_mousePosition.x = ImGui::GetCursorPos().x + real_buttonSize.x*0.5f - 37.f;
							real_mousePosition.y = ImGui::GetCursorPos().y + 80 + pSize.y * 0.45f;

							if (!m_waypointEmitter)
								m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
							else
								ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);
						}
					}
				}

				ImGui::EndChild();
				ImGui::PopStyleColor();

				// display weapon picture here
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.12f, viewport->Size.y * 0.35f });
				auto UnitImg = tex_map["Assets/WeaponImage.dds"];
				if (UI::UIImageButton((void*)static_cast<size_t>(UnitImg->GetID()), ImVec2{ UnitImg->GetWidth() * 1.0f,UnitImg->GetHeight() * 1.0f }))
				{

				}
				ImGui::PopStyleColor(3);

				// display information
				// tabs 
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.35f, viewport->Size.y * 0.15f });

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

					if (weap)
					{
						render_tabs(TABS_TYPE::WEAPON, [&]() {
							WeaponSheetOverlay::display_weapon_stats(*viewport, instance);
						});
					}
				
			/*		render_tabs(TABS_TYPE::USE, [&]() {
						WeaponSheetOverlay::display_use_stats(*viewport, instance);
					});*/

					ImGui::PopStyleColor(3);
				}

				// display top buttons
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * inter.get(), viewport->Size.y * 0.03f });
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }))
					{
						OverlayOpen = false;
						ImGui::CloseCurrentPopup();
						//Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(3);
						Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);

						
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();
					auto quickMenuPos = ImGui::GetCursorPos();
					tex = tex_map["Assets/QuickMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::WEAPONS);
						//Tutorial progression
						if (instance.tutorial_enable && tutorial_index == 2)
							tutorial_index = 3;
					}
					if (instance.tutorial_enable && tutorial_index == 2 && inter.is_finished())
					{
						if (particle_2 == false)
						{
							particle_2 = true;

							glm::vec2 real_buttonSize;
							real_buttonSize.x = tex->GetWidth() * 0.7f;
							real_buttonSize.y = tex->GetHeight() * 0.7f;

							glm::vec2 real_mousePosition;
							real_mousePosition.x = quickMenuPos.x ;
							real_mousePosition.y = quickMenuPos.y ;

							if (!m_waypointEmitter)
								m_waypointEmitter = ParticleSystem_2D::GetInstance().ButtonEmitter(real_mousePosition, real_buttonSize);
							else
								ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);
						}
					}

					ImGui::PopStyleColor(3);
				}

				if (instance.tutorial_enable)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					switch (tutorial_index)
					{
						case 0:
						{
							ImVec2 pos = { viewport->Size.x * 0.02f, viewport->Size.y * 0.19f };
							ImVec2 size = { 200.f, 70.f };
							UI::TutArea(pos, size, false);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to create a new weapon.";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							
						}
						break;

						case 1:
						{
							if (!weap)
								break;

							ImVec2 pos = { viewport->Size.x * 0.35f, viewport->Size.y * 0.22f };
							ImVec2 size = { viewport->Size.x * 0.45f, viewport->Size.y * 0.7f };
							auto selected = tex_map["Assets/Selected.dds"];
							auto unselected = tex_map["Assets/Unselected.dds"];
							bool taskCompleted = true;
							UI::TutArea(pos, size, false);

							// render the tasks
							//Task List
							string str = "";
							str = string(ICON_FK_EXCLAMATION_CIRCLE);
							ImGui::PushFont(FONT_HEAD);
							drawlist->AddText({ viewport->Size.x * 0.8f, viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 1.f,1.f,1.f,1 }), str.c_str());
							str = " Tasks";
							drawlist->AddText({ viewport->Size.x * 0.8f + ImGui::GetFontSize(), viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							drawlist->AddLine({ viewport->Size.x * 0.8f, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, { viewport->Size.x, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
							ImGui::PopFont();


							ImGui::PushFont(FONT_BODY);
							ImVec2 min = { viewport->Size.x * 0.8f, viewport->Size.y * 0.45f };
							str = "Rename the weapon";
							if (weap->name != "Weapon")
							{
								drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
								taskCompleted &= true;
							}
							else
							{
								drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
								taskCompleted &= false;
							}
							drawlist->AddText({ viewport->Size.x * 0.8f + selected->GetWidth() * 0.7f , min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							min = { min.x, min.y + unselected->GetWidth() * 0.9f };
							str = "Assign 3 ATK to the weapon";
							if (weap->get_stat(1) == 3)
							{
								drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
								taskCompleted &= true;
							}
							else
							{
								drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
								taskCompleted &= false;
							}
							drawlist->AddText({ viewport->Size.x * 0.8f + selected->GetWidth() * 0.7f, min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
							ImGui::PopFont();

							auto nextBtn = tex_map["Assets/NextBtn.dds"];
							ImVec2 tut_min = { min.x, min.y + unselected->GetWidth() * 0.9f };
							ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

							if (taskCompleted)
							{
								drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);

								if (UI::MouseIsWithin(tut_min, tut_max))
								{
									ImGui::SetMouseCursor(7);
									if (ImGui::IsMouseClicked(0))
										tutorial_index = 2;
								}
							}
							else
								drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max, { 0,0 }, { 1,1 }, ImGui::GetColorU32({ 1,1,1,0.4f }));

							m_waypointEmitter->m_GM.m_active = false;
						}
						break;

						case 2:
						{
							ImVec2 pos = { viewport->Size.x * 0.1052f, viewport->Size.y * 0.0261f };
							ImVec2 size = { 200.f, 50.f };
							UI::TutArea(pos, size, false);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access the quick menu.";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							
						}
						break;

						case 3:
						{
							ImVec2 pos = { viewport->Size.x * 0.345f, viewport->Size.y * 0.1f };
							ImVec2 size = { 310.f, 140.f };
							UI::TutArea(pos, size, false);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access the actions page.";
							drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							if (particle_3 == false)
							{
								glm::vec2 real_buttonSize;
								real_buttonSize.x = size.x;
								real_buttonSize.y = size.y;

								glm::vec2 real_mousePosition;
								real_mousePosition.x = pos.x;
								real_mousePosition.y = pos.y;

								ParticleSystem_2D::GetInstance().ReuseButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);

								particle_3 = true;
							}
						}
						break;

					}

					//Tutorial Exit Button
					auto exitBtn = tex_map["Assets/Tutorial_exit.dds"];
					ImVec2 tut_min = { viewport->Size.x * 0.85f, viewport->Size.y * 0.05f };
					ImVec2 tut_max = { tut_min.x + exitBtn->GetWidth() * 0.7f, tut_min.y + exitBtn->GetHeight() * 0.7f };
					drawlist->AddImage((void*)static_cast<size_t>(exitBtn->GetID()), tut_min, tut_max);

					if (UI::MouseIsWithin(tut_min, tut_max))
					{
						ImGui::SetMouseCursor(7);
						if (ImGui::IsMouseClicked(0))
							instance.tutorial_enable = false;
					}

					UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 2);
				}
			}

			ImGui::PopStyleVar();
			ImGui::End();
		}
		if (m_waypointEmitter && (!OverlayOpen || !instance.tutorial_enable))
			m_waypointEmitter->m_GM.m_active = false;
	}

	void WeaponSheetOverlay::create_new_weapon(Instance &instance)
	{
		tc::Weapon* temp_weap = &NewWeapon;
		auto entity = instance.ecs.create();
		auto meta = instance.ecs.emplace<tc::Meta>(entity);
		meta->name = temp_weap->name;
		auto character = instance.ecs.emplace<tc::Weapon>(entity);
		character->name = temp_weap->name;

		for (auto i = 0; i < tc::STAT_TOTAL; i++)
			character->set_stat(i, temp_weap->get_stat(i));

		NewWeapon = tc::Weapon();
		SelectedID = entity;
	}

	void WeaponSheetOverlay::initialise_tabs()
	{
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/NewWeaponInfoTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/NewWeaponInfoTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::WEAPON].size = ImVec2{ static_cast<float>(tex_map["Assets/NewWeaponInfoTabLit.dds"]->GetWidth()),
											 static_cast<float>(tex_map["Assets/NewWeaponInfoTabLit.dds"]->GetHeight()) };

		Tabs[TABS_TYPE::USE].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/UseInfoTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::USE].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/UseInfoTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::USE].size = ImVec2{ static_cast<float>(tex_map["Assets/UseInfoTabUnlit.dds"]->GetWidth()),
											   static_cast<float>(tex_map["Assets/UseInfoTabUnlit.dds"]->GetHeight()) };
	}

	template<typename F>
	void WeaponSheetOverlay::render_tabs(TABS_TYPE type, F&& func)
	{
		ImVec2 prev_cursor_pos{ 0.f,0.f };

		if (ImGui::ImageButton(Tabs[type].image_id[Tabs[type].current_state], Tabs[type].size))
		{
			Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
			Tabs[CurrentTab].is_active = false;
			Tabs[type].is_active = true;

			CurrentTab = type;
		}

		if (ImGui::IsItemHovered() || Tabs[type].is_active)
			Tabs[type].current_state = TabImageData::STATE::HOVER;
		else
			Tabs[type].current_state = TabImageData::STATE::UNHOVER;

		if (type != TABS_TYPE::USE)
		{
			ImGui::SameLine();
			prev_cursor_pos = ImGui::GetCursorPos();
		}

		if (Tabs[type].is_active)
			func();

		// Get ready for next render of tab
		ImGui::SetCursorPos(prev_cursor_pos);
	}

	void WeaponSheetOverlay::display_weapon_stats(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!weap)
			return; 

		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });

		ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
		ImGui::BeginChild("##WeaponsInformationDisplay", { viewport.Size.x * 0.6f, viewport.Size.y * 0.55f }, true);

		// display the weapon info
		float frontPadding = 5.f;
		ImGui::PushFont(FONT_BODY);
		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::InputText("##WeaponName", &weap->name);
		bool NameDisabled = weap->name.size() > 15;
		ImGui::SameLine();
		if (NameDisabled)
		{
			ImGui::Text("15 Char only");
		}
		else
		{
			ImGui::Dummy({ 100.f, 10.f });
		}
		ImGui::Dummy({ 0, 10.f });

		bool NextLine = false;
		ImVec2 PrevPos{ 0.f ,0.f };
		for (auto i = 0; i < sl->size(); i++)
		{
			if ((*sl)(i))
			{
				string stat = sl->operator[](i) + " :";
				string label = "##" + stat;

				if (!NextLine)
				{
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					PrevPos = ImGui::GetCursorPos();
					ImGui::Text(stat.c_str());
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &weap->get_stat(i), 0);
					ImGui::PopItemWidth();
				}

				else
				{
					ImGui::SetCursorPos(PrevPos);
					ImGui::Dummy({ 250.f - frontPadding, 0.f });
					ImGui::SameLine();

					ImGui::Text(stat.c_str());
					ImGui::Dummy({ 250 + frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &weap->get_stat(i), 0);
					ImGui::PopItemWidth();

					ImGui::SetCursorPos(PrevPos);
					ImGui::Dummy({ 0, 70.f });
				}
			

				NextLine = !NextLine;
			}

		}
		ImGui::PopFont();

		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	void WeaponSheetOverlay::display_use_stats(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!weap)
			return;

		(void)viewport;
		(void)instance;
	}
}