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

#include "UnitSheetOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"
#include "Instance/EditTimeInstance.h"

namespace Tempest
{
	// pass in instance 
	void UnitSheetOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;

		Tabs[TABS_TYPE::UNIT].current_state = TabImageData::STATE::UNHOVER;
		Tabs[TABS_TYPE::WEAPON].current_state = TabImageData::STATE::UNHOVER;
		Tabs[TABS_TYPE::ITEM].current_state = TabImageData::STATE::UNHOVER;
		Tabs[TABS_TYPE::ACTION].current_state = TabImageData::STATE::UNHOVER;

		auto a = event_cast<OpenUnitSheetTrigger>(e);

		if (a.addUnit)
		{
			create_new_unit(a.instance);
		}
		
		else if (a.entityID != UNDEFINED)
			SelectedID = a.entityID;

		else
			SelectedID = a.instance.ecs.view_first<Components::Character>(exclude_t<tc::Destroyed>());

		TempAction = UNDEFINED;
		TempWeapon = UNDEFINED;

		if (SelectedID != INVALID && SelectedID != UNDEFINED)
		{
			cs = a.instance.ecs.get_if<tc::Character>(SelectedID);
			CurrentTab = TABS_TYPE::UNIT;
			Tabs[TABS_TYPE::UNIT].is_active = true;
			Tabs[TABS_TYPE::WEAPON].is_active = false;
			Tabs[TABS_TYPE::ITEM].is_active = false;
			Tabs[TABS_TYPE::ACTION].is_active = false;
		}

		/*for (int i = 0; i < inter_nest.size(); ++i)
		{
			inter_nest[i].start(-.18 * ImGui::GetMainViewport()->Size.x, .0f, .4f, i * .05f, [](float x) { return glm::cubicEaseOut(x); });
		}*/
		inter_nest[0].start(1.f, .35f, .4f, 0, [](float x) { return glm::cubicEaseOut(x); }); // tabs
		inter_nest[1].start(0.5f, .15f, .4f, 0, [](float x) { return glm::cubicEaseOut(x); }); // units 
		inter_nest[2].start(0.f, .15f, .4f, 0, [](float x) { return glm::cubicEaseOut(x); }); // define stats
		inter.start(-0.1f, 0.02f, .25f, 0, [](float x) { return glm::cubicEaseOut(x); }); // back

		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.set_orbit_camera(cam, vec3(0.f, 0.f, 0.f));
		cam_ctrl.update(cam);
	}

	void UnitSheetOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::UNITS)
		{
			OverlayOpen = false;
			cs = nullptr;
		}
	}

	void UnitSheetOverlay::force_close(const Event& e)
	{
		OverlayOpen = false;
		cs = nullptr;
	}

	void UnitSheetOverlay::confirm_data(const Event& e)
	{
		auto a = event_cast<SimulateSelectionConfirm>(e);
		if (!a.for_unitpage || a.data == UNDEFINED)
			return; 

		switch (a.type)
		{
		case SIMULATE_POPUP_TYPE::ACTION:
			TempAction = a.data;
			cs->actions.emplace_back(TempAction);
			TempAction = UNDEFINED;
			break;
		case SIMULATE_POPUP_TYPE::WEAPON:
			TempWeapon = a.data;
			cs->weapons.emplace_back(TempWeapon);
			TempWeapon = UNDEFINED;
			break;
		default:
			break;
		}
		
	}

	void UnitSheetOverlay::show(Instance& instance)
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

			if (ImGui::Begin("Edit Unit Sheet", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/UnitsBG.dds"];
				
				ImVec2 point{ 0,0 };
				{
					ImVec2 Min{ point.x, point.y };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				ImGui::SetCursorPos(ImVec2{0,0});
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Editing Unit");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// Display the created units
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f, viewport->Size.y * inter_nest[1].get() });
				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				ImGui::BeginChild("##UnitsDisplay", { viewport->Size.x * 0.1f, viewport->Size.y * 0.7f }, true);
				{
					unsigned i = 0;
					auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

					// TODO: store selected item
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 30, ImGui::GetCursorPosY() + 20 };
					for (auto id : view)
					{
						auto& charac = instance.ecs.get<tc::Character>(id);
						ImGui::SetCursorPos(ImVec2{ cursor.x , cursor.y + i++ * 165 });
						auto CharIcon = tex_map["Assets/CharacterIcon.dds"];
						std::pair<bool,bool> PairResult = UI::UICharButton_WithDelete((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth(), (float)CharIcon->GetHeight() }, charac.name.c_str(), "##" + std::to_string(i), SelectedID == id,
							{ 0,0 }, { 1,1 }, 2, ImVec4{ 0,0,0,0 }, ImVec4{ charac.color.x, charac.color.y,charac.color.z,1 });
						if (PairResult.first)
						{
							SelectedID = id;
							cs = &charac;
						}
						if (PairResult.second)
						{
							ImGui::OpenPopup(string("DeleteCharacter##" + std::to_string(i)).c_str());
						}

						if (UI::ConfirmDeletePopup(string("DeleteCharacter##" + std::to_string(i)).c_str(), "Delete this character?"))
						{
							instance.ecs.emplace<tc::Destroyed>(id);
							SelectedID = UNDEFINED;
							cs = nullptr;
						}
					}

					// just try with get cursor pos
					if (UI::UIButton_1("+", "+", ImVec2{ ImGui::GetCursorPos().x + 80, ImGui::GetCursorPos().y + 60 }, { 45,20 }, FONT_HEAD))
					{
						create_new_unit(instance);
						cs = instance.ecs.get_if<tc::Character>(SelectedID);
						Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
						Tabs[CurrentTab].is_active = false;
						Tabs[TABS_TYPE::UNIT].is_active = true;

						CurrentTab = TABS_TYPE::UNIT;
					}
				}
				ImGui::EndChild();
				ImGui::PopStyleColor();

				// display unit picture here
				if (cs)
				{
					//auto newEnt = instance.ecs.create();
					//
					//instance.ecs.emplace<tc::Model>(newEnt, "UnitBlack_Idle.a");

					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.12f, viewport->Size.y * 0.15f });
					auto UnitImg = tex_map["Assets/UnitIdle.dds"];
					if (ImGui::BeginChild("RenderUnit##", ImVec2{ UnitImg->GetWidth() * 1.0f,UnitImg->GetHeight() * 1.0f }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
					{
						auto& cam = Service<RenderSystem>::Get().GetCamera();
						if (ImGui::IsWindowHovered())
						{
							cam_ctrl.preview_controls(cam);
						}
						cam_ctrl.update(cam);
						const ImVec4 color{ cs->color.x, cs->color.y, cs->color.z, 1 };
						Service<RenderSystem>::Get().USOcolor.x = cs->color.x;
						Service<RenderSystem>::Get().USOcolor.y = cs->color.y;
						Service<RenderSystem>::Get().USOcolor.z = cs->color.z;
						//ImGui::Image((void*)static_cast<size_t>(UnitImg->GetID()), ImVec2{ UnitImg->GetWidth() * 1.0f,UnitImg->GetHeight() * 1.0f }, ImVec2{ 0,0 }, ImVec2{ 1,1 }, color);
						ImGuiViewport* viewport = ImGui::GetMainViewport();
						vec2 size = vec2(viewport->Size.x, viewport->Size.y);
						ImGui::Image((ImTextureID)(Service<RenderSystem>::Get().postprocessBuffer), ImVec2(viewport->Size.x, viewport->Size.y), ImVec2(0, 1), ImVec2(1, 0));
						//ImGui::Image((ImTextureID)(Service<RenderSystem>::Get().USObuffer), ImVec2(Service<RenderSystem>::Get().getWidth(), Service<RenderSystem>::Get().getHeight()), ImVec2(0, .2), ImVec2(.2, 0));
					}
					ImGui::EndChild();
					
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.33f, viewport->Size.y * 0.85f });
					ImGui::ColorEdit4("##colorbuttonunit", glm::value_ptr(cs->color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
				}

				// tabs 
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * inter_nest[0].get(), viewport->Size.y * 0.15f });

					push_button_style();

					render_tabs(TABS_TYPE::UNIT, [&]() {
						UnitSheetOverlay::display_unit_stats(*viewport, instance);
					});

					render_tabs(TABS_TYPE::WEAPON, [&]() {
						UnitSheetOverlay::display_weapon_stats(*viewport, instance);
					});

					render_tabs(TABS_TYPE::ITEM, [&]() {
						// not using, i no on-hover item picture :(
						UnitSheetOverlay::display_items(*viewport, instance);
					});

					render_tabs(TABS_TYPE::ACTION, [&]() {
						UnitSheetOverlay::display_actions(*viewport, instance);
					});

					pop_button_style();
				}

				ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
				ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
				// display top buttons
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * inter.get(),viewport->Size.y * 0.03f });
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0, 0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
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

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0, 0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::UNITS);
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/TutorialBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::UNITS_TUT);
					}

					ImGui::PopStyleColor(3);
				}

				// define stats button
				auto image = tex_map["Assets/DefineStat.dds"];
				ImGui::SetCursorPos({ viewport->Size.x * (1 - inter_nest[2].get()), viewport->Size.y * 0.04f });
				if (UI::UIImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }, { 0, 0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
				{
				//if (UI::UIButton_2("Define stats", "Define stats", { viewport->Size.x * 0.92f, viewport->Size.y * 0.05f }, { 0,0 }, FONT_PARA))
				//{
					Service<EventManager>::Get().instant_dispatch<DefineStatsTrigger>();
				}
			}
			
			ImGui::PopStyleVar();
			ImGui::End();
			
			//ImGui::Begin("GameWindow", true);
			//{
			//	// Using a Child allow to fill all the space of the window.
			//	// It also alows customization
			//	ImGui::BeginChild("GameRender");
			//	// Get the size of the child (i.e. the whole draw size of the windows).
			//	ImVec2 wsize = ImGui::GetWindowSize();
			//	// Because I use the texture from OpenGL, I need to invert the V from the UV.
			//	ImGui::Image((ImTextureID)(Service<RenderSystem>::Get().gBuffer), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			//	ImGui::EndChild();
			//}
			//ImGui::End();
			
			Service<RenderSystem>::Get().USO = true;

		}
		else
			Service<RenderSystem>::Get().USO = false;
	}

	void UnitSheetOverlay::push_button_style() const
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
	}

	void UnitSheetOverlay::pop_button_style() const
	{
		ImGui::PopStyleColor(3);
	}

	void UnitSheetOverlay::create_new_unit(Instance& instance)
	{
		tc::Character* tempcs = &NewCharacter;
		auto entity = instance.ecs.create();
		auto meta = instance.ecs.emplace<tc::Meta>(entity);
		meta->name = tempcs->name;
		auto character = instance.ecs.emplace<tc::Character>(entity);
		character->name = tempcs->name;

		for (auto i = 0; i < tc::STAT_TOTAL; i++)
		{
			character->set_stat(i, tempcs->get_stat(i));
			character->set_statDelta(i, 0);
		}
			

		NewCharacter = tc::Character();
		SelectedID = entity;
	}

	template<typename F>
	void UnitSheetOverlay::render_tabs(TABS_TYPE type, F&& func)
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

		if (type != TABS_TYPE::ACTION)
		{
			ImGui::SameLine();
			prev_cursor_pos = ImGui::GetCursorPos();
		}

		if (Tabs[type].is_active)
		{
			func();
		}
			

		// Get ready for next render of tab
		ImGui::SetCursorPos(prev_cursor_pos);
	}

	void UnitSheetOverlay::initialise_tabs()
	{
		Tabs[TABS_TYPE::UNIT].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/UnitTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::UNIT].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/UnitTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::UNIT].size = ImVec2{ static_cast<float>(tex_map["Assets/UnitTabUnlit.dds"]->GetWidth()), 
											 static_cast<float>(tex_map["Assets/UnitTabUnlit.dds"]->GetHeight()) };

		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::WEAPON].size = ImVec2{ static_cast<float>(tex_map["Assets/WeaponTabUnlit.dds"]->GetWidth()), 
											   static_cast<float>(tex_map["Assets/WeaponTabUnlit.dds"]->GetHeight()) };

		Tabs[TABS_TYPE::ITEM].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/ItemTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::ITEM].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/ItemTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::ITEM].size = ImVec2{ static_cast<float>(tex_map["Assets/ItemTabUnlit.dds"]->GetWidth()),
											 static_cast<float>(tex_map["Assets/ItemTabUnlit.dds"]->GetHeight()) };

		Tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/ActionTabUnlit.dds"]->GetID());
		Tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/ActionTabLit.dds"]->GetID());
		Tabs[TABS_TYPE::ACTION].size = ImVec2{ static_cast<float>(tex_map["Assets/ActionTabUnlit.dds"]->GetWidth()),
											   static_cast<float>(tex_map["Assets/ActionTabUnlit.dds"]->GetHeight()) };
	}

	void UnitSheetOverlay::display_unit_stats(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!cs)
			return; 

		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });

		ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
		ImGui::BeginChild("##UnitsInformationDisplay", { viewport.Size.x * 0.6f, viewport.Size.y * 0.55f }, true);

		// display the character info
		float frontPadding = 5.f;
		ImGui::PushFont(FONT_BODY);
		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::InputText("##CharacterName", &cs->name);
		bool NameDisabled = cs->name.size() > 15;
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
				string label = "##" + stat + std::to_string(i);
				string WeaponData = "";
		/*		if (cs.weapon != UNDEFINED)
				{
					auto weap = instance.ecs.get_if<tc::Weapon>(cs.weapon);

					if (weap->get_stat(i) > 0)
					{
						string data = std::to_string(weap->get_stat(i));
						WeaponData = "( +" + data + " )";
					}
					else if (weap->get_stat(i) < 0)
					{
						string data = std::to_string(weap->get_stat(i));
						WeaponData = "( " + data + " )";
					}

				}*/

				if (!NextLine)
				{
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					PrevPos = ImGui::GetCursorPos();
					ImGui::Text(stat.c_str());
					if (i < 5)
					{
						ImGui::SameLine();
						UI::HelpMarker(sl->get_statsDesription(i).c_str());
					}
					
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::Text(WeaponData.c_str());
				}

				else
				{
					ImGui::SetCursorPos(PrevPos);
					ImGui::Dummy({ 250.f - frontPadding, 0.f });
					ImGui::SameLine();

					ImGui::Text(stat.c_str());
					if (i < 5)
					{
						ImGui::SameLine();
						UI::HelpMarker(sl->get_statsDesription(i).c_str());
					}
					ImGui::Dummy({ 250 + frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::Text(WeaponData.c_str());

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

	void UnitSheetOverlay::display_weapon_stats(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!cs)
			return;

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });

		ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
		ImGui::BeginChild("##WeaponsInformationDisplay", { viewport.Size.x * 0.6f, viewport.Size.y * 0.55f }, true);
		unsigned i = 0;
		unsigned j = 0;
		const ImVec2 cursor{ ImGui::GetCursorPosX() + 130, ImGui::GetCursorPosY() + 60 };

		tset<Entity> destroyed;
		for (auto weap_id : cs->weapons)
		{
			if (auto weap = instance.ecs.get_if<tc::Weapon>(weap_id))
			{
				auto PairResult = UI::UIButtonWithDelete(weap->name.c_str(), string("##weapweap" + std::to_string(i + j * 5)), { cursor.x + i++ * 300.0f, cursor.y + j * 100.0f }, { 40,20 }, FONT_BODY, false);
				if (PairResult.first)
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
					Service<EventManager>::Get().instant_dispatch<OpenWeaponSheetTrigger>(false, instance, weap_id);
				}
				if (PairResult.second)
				{
					ImGui::OpenPopup(string("RemoveWeap##" + std::to_string(i)).c_str());
				}

				if (UI::ConfirmDeletePopup(string("RemoveWeap##" + std::to_string(i)).c_str(), "Remove this weapon?"))
				{
					cs->weapons.erase(std::remove(cs->weapons.begin(), cs->weapons.end(), weap_id), cs->weapons.end());
				}

				// display in rows of 2
				if (i / 3)
				{
					i = 0;
					j++;
				}
			}
			else
			{
				destroyed.insert(weap_id);
			}
		}
		std::_Erase_remove_if(cs->actions, [&destroyed](Entity e) { return destroyed.count(e); });

		if (UI::UIButton_2("+", "+", ImVec2{ cursor.x + i * 300.0f, cursor.y + j * 100.0f }, {10,0}, FONT_BODY))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::WEAPON, false, TempWeapon, true);
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	void UnitSheetOverlay::display_items(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!cs)
			return;

		(void)viewport;
		(void)instance;
	}

	void UnitSheetOverlay::display_actions(const ImGuiViewport& viewport, Instance& instance)
	{
		if (!cs)
			return;

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });
		ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
		ImGui::BeginChild("##ActionsInformationDisplay", { viewport.Size.x * 0.6f, viewport.Size.y * 0.55f }, true);
		unsigned i = 0;
		unsigned j = 0;
		const ImVec2 cursor{ ImGui::GetCursorPosX() + 130, ImGui::GetCursorPosY() + 60 };

		tset<Entity> destroyed;
		for (auto id : cs->actions)
		{
			if (auto action = instance.ecs.get_if<tc::Graph>(id))
			{
				auto PairResult = UI::UIButtonWithDelete(action->g.name.c_str(), string("##actionn" + std::to_string(i + j * 5)), { cursor.x + i++ * 300.0f, cursor.y + j * 100.0f }, { 40,20 }, FONT_BODY, false);
				if (PairResult.first)
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
					Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(id, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION);
				}
				if (PairResult.second)
				{
					ImGui::OpenPopup(string("RemoveAction##" + std::to_string(i)).c_str());
				}

				if (UI::ConfirmDeletePopup(string("RemoveAction##" + std::to_string(i)).c_str(), "Remove this action?"))
				{
					cs->actions.erase(std::remove(cs->actions.begin(), cs->actions.end(), id), cs->actions.end());
				}

				// display in rows of 2
				if (i / 3)
				{
					i = 0;
					j++;
				}
			}
			else
			{
				destroyed.insert(id);
			}
		}

		std::_Erase_remove_if(cs->actions, [&destroyed](Entity e) { return destroyed.count(e); });

		if (UI::UIButton_2("+", "+", ImVec2{ cursor.x + i * 300.0f, cursor.y + j * 100.0f }, { 10,0 }, FONT_BODY))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::ACTION, false, TempWeapon, true);
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}