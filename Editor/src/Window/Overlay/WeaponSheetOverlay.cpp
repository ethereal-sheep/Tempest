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
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f, viewport->Size.y * 0.15f });

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

					if (UI::UIButton_1("+", "+", { ImGui::GetCursorPosX() + 99,  ImGui::GetCursorPosY() + 80 }, { 140,-5 }, FONT_HEAD))
					{
						create_new_weapon(instance);
						weap = instance.ecs.get_if<tc::Weapon>(SelectedID);
						Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
						Tabs[CurrentTab].is_active = false;
						Tabs[TABS_TYPE::WEAPON].is_active = true;

						CurrentTab = TABS_TYPE::WEAPON;
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
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.dds"];

					if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
					{
						OverlayOpen = false;
						ImGui::CloseCurrentPopup();
						//Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(3);
						Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);

						
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/QuickMenuBtn.dds"];

					if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::WEAPONS);
					}

					ImGui::PopStyleColor(3);
				}
			}

			ImGui::PopStyleVar();
			ImGui::End();
		}
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