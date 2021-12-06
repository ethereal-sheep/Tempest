/**********************************************************************************
* \author		_ (_@digipen.edu)
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
			
	}

	void UnitSheetOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::UNITS)
			OverlayOpen = false;
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
		
		if (OverlayOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Edit Unit Sheet", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/UnitsBG.png"];
				
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
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f, viewport->Size.y * 0.15f });
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
						auto CharIcon = tex_map["Assets/CharacterIcon.png"];
						std::pair<bool,bool> PairResult = UI::UICharButton_WithDelete((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth(), (float)CharIcon->GetHeight() }, charac.name.c_str(), "##" + std::to_string(i), SelectedID == id, { 0,0 }, { 1,1 });
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

				// display unit picture here
				//push_button_style();
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.12f, viewport->Size.y * 0.15f });
				auto UnitImg = tex_map["Assets/UnitIdle.png"];
				if (UI::UIImageButton((void*)static_cast<size_t>(UnitImg->GetID()), ImVec2{ UnitImg->GetWidth()*1.0f,UnitImg->GetHeight() * 1.0f }))
				{

				}
				//pop_button_style();
			
				// tabs 
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.35f, viewport->Size.y * 0.15f });

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

				// display top buttons
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.png"];

					if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
					{
						OverlayOpen = false;
						ImGui::CloseCurrentPopup();
						Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/QuickMenuBtn.png"];

					if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::UNITS);
					}

					ImGui::PopStyleColor(3);
				}

				// define stats button
				if (UI::UIButton_2("Define stats", "Define stats", { viewport->Size.x * 0.92f, viewport->Size.y * 0.05f }, { 0,0 }, FONT_PARA))
				{
					Service<EventManager>::Get().instant_dispatch<DefineStatsTrigger>();
				}

				{
					//ImGui::Columns(4, "UnitCreation", true);
					//ImGuiWindow* window = ImGui::GetCurrentWindow();
					//ImGuiOldColumnFlags ColFlags = ImGuiOldColumnFlags_NoResize;
					//ImGuiOldColumns* columns = window->DC.CurrentColumns;
					//columns->Flags = ColFlags;

					//auto tex = tex_map["Assets/test_photo2.png"];
					//ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2(ImGui::GetColumnWidth(0), ImGui::GetColumnWidth(0)));
					///*==================================================================
					//					CHARACTER INFOMATION COLUMN
					//==================================================================*/
					//ImGui::NextColumn();
					//ImGui::PushFont(FONT_BODY);
					//string text = "Character Infomation";
					//float FirstColWidth = ImGui::GetColumnWidth(1) / 2;
					//float font_size = ImGui::GetFontSize() * text.size() / 2;
					//float offsetX = FirstColWidth - font_size + (font_size / 2);
					//float frontPadding = 5.f;


					//ImGui::Dummy({ offsetX, 0 });
					//ImGui::SameLine();

					//ImGui::Text(text.c_str());
					//ImGui::PopFont();
					//ImGui::Dummy({ 0, 10.f });
					//ImGui::BeginChild("##CharacterInfo", ImVec2(ImGui::GetColumnWidth(1) - 10.f, viewport->Size.y * 0.5f));
					////Name
					//ImGui::PushFont(FONT_BODY);
					//ImGui::Dummy({ frontPadding, 0 });
					//ImGui::SameLine();
					//ImGui::Text("Name");

					//ImGui::Dummy({ frontPadding, 0 });
					//ImGui::SameLine();
					//ImGui::InputText("##CharacterName", &cs->name);
					//bool NameDisabled = cs->name.size() > 15;
					//ImGui::SameLine();
					//if (NameDisabled)
					//{
					//	ImGui::Text("15 Char only");
					//}
					//else
					//{
					//	ImGui::Dummy({ 100.f, 10.f });
					//}
					//ImGui::Dummy({ 0, 10.f });

					//for (auto i = 0; i < sl->size(); i++)
					//{
					//	if ((*sl)(i))
					//	{
					//		string stat = sl->operator[](i) + " :";
					//		string label = "##" + stat;
					//		//auto data = std::to_string(cs->get_stat(i));
					//		string WeaponData = "";
					//		if (cs->weapon != UNDEFINED)
					//		{
					//			auto weap = instance.ecs.get_if<tc::Weapon>(cs->weapon);

					//			if (weap->get_stat(i) > 0)
					//			{
					//				string data = std::to_string(weap->get_stat(i));
					//				WeaponData = "( +" + data + " )";
					//			}
					//			else if (weap->get_stat(i) < 0)
					//			{
					//				string data = std::to_string(weap->get_stat(i));
					//				WeaponData = "( " + data + " )";
					//			}

					//		}


					//		ImGui::Dummy({ frontPadding, 0 });
					//		ImGui::SameLine();
					//		ImGui::Text(stat.c_str());
					//		ImGui::Dummy({ frontPadding, 0 });
					//		ImGui::SameLine();
					//		ImGui::PushItemWidth(100.f);
					//		ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					//		ImGui::PopItemWidth();
					//		ImGui::SameLine();
					//		ImGui::Text(WeaponData.c_str());
					//		ImGui::Dummy({ 0, 10.f });
					//	}

					//}
					//ImGui::PopFont();
					//ImGui::EndChild();

					///*==================================================================
					//					WEAPON AND ITEMS COLUMN
					//==================================================================*/
					//ImGui::NextColumn();
					//ImGui::PushFont(FONT_BODY);
					//string WeapText = "Inventory";
					//float SecondColWidth = ImGui::GetColumnWidth(2) / 2;
					//font_size = ImGui::GetFontSize() * WeapText.size() / 2;
					//float SecondoffsetX = SecondColWidth - font_size + (font_size / 2);

					//ImGui::Dummy({ SecondoffsetX, 0 });
					//ImGui::SameLine();

					//ImGui::Text(WeapText.c_str());
					//ImGui::PopFont();
					//ImGui::Dummy({ 0, 10.f });
					//ImGui::BeginChild("##WeapAndItem", ImVec2(ImGui::GetColumnWidth(2) - 10.f, viewport->Size.y * 0.5f));


					//if (cs->weapon != UNDEFINED)
					//{
					//	auto& weap = instance.ecs.get<tc::Weapon>(cs->weapon);
					//	if (UI::UIButton_2(weap.name.c_str(), weap.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + 30.0f }, { 40.f, 20.f }, FONT_BODY))
					//	{
					//		EditWeaponPopup = true;
					//		EditWeap = weap;
					//	}
					//	if (EditWeaponPopup)
					//	{
					//		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
					//			ImGuiWindowFlags_NoScrollbar;
					//		ImGui::OpenPopup("Edit Weapon");
					//		ImGui::SetNextWindowSize(ImVec2(800, 300));
					//		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					//		if (ImGui::BeginPopupModal("Edit Weapon", NULL, flags))
					//		{
					//			//float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
					//			string Editsub = "Edit Weapon/Item";
					//			font_size = ImGui::GetFontSize() * Editsub.size() / 2;
					//			//float center = RegionWidth - font_size;
					//			UI::SubHeader(Editsub.c_str());

					//			ImGui::Dummy({ 0, 20.f });
					//			ImGui::Columns(2, "Edit Weapon", true);
					//			window = ImGui::GetCurrentWindow();
					//			columns = window->DC.CurrentColumns;
					//			columns->Flags = ColFlags;
					//			ImGui::SetColumnWidth(0, 332.f);

					//			ImGui::NextColumn();
					//			ImGui::PushFont(FONT_PARA);
					//			//static tc::Weapon newWeap;
					//			float Padding_x = 60.f;
					//			ImGui::Text("Name");
					//			ImGui::SameLine();
					//			ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
					//			ImGui::SameLine();
					//			ImGui::InputText("##EditWeapName", &EditWeap.name);
					//			bool disabled = EditWeap.name.size() > 15;
					//			ImGui::SameLine();
					//			if (disabled)
					//			{
					//				ImGui::Text("15 Char only");
					//			}
					//			else
					//			{
					//				ImGui::Dummy({ 100.f, 10.f });
					//			}
					//			ImGui::Dummy({ 0, 10.f });
					//			ImGui::BeginChild("##EditWeapStats", { ImGui::GetColumnWidth(1) - 10.f, 100.f });
					//			for (auto i = 0, j = 0; i < sl->size(); i++)
					//			{
					//				if ((*sl)(i))
					//				{
					//					ImGui::Text(sl->operator[](i).c_str());
					//					ImGui::SameLine();
					//					ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
					//					ImGui::SameLine();
					//					ImGui::SetNextItemWidth(80.f);
					//					string WeapStats = "##WeapStats" + std::to_string(i);
					//					ImGui::InputInt(WeapStats.c_str(), &EditWeap.get_stat(i), 0);

					//					if (j % 2 == 0)
					//					{
					//						ImGui::SameLine();
					//						ImGui::Dummy({ 10.f, 0 });
					//						ImGui::SameLine();
					//					}
					//					j++;
					//				}
					//			}

					//			ImGui::EndChild();
					//			ImGui::PopFont();
					//			ImGui::EndColumns();
					//			if (disabled)
					//			{
					//				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					//				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					//				//ImGui::PushDisabled();
					//			}
					//			if (UI::UIButton_1("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					//			{
					//				ImGui::CloseCurrentPopup();
					//				weap = EditWeap;
					//				EditWeaponPopup = false;
					//			}
					//			if (disabled)
					//			{
					//				ImGui::PopItemFlag();
					//				ImGui::PopStyleVar();
					//				//ImGui::PopDisabled();
					//			}
					//			if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					//			{
					//				ImGui::CloseCurrentPopup();
					//				EditWeap = tc::Weapon();
					//				EditWeaponPopup = false;
					//			}
					//			ImGui::EndPopup();
					//		}
					//	}
					//}
					//ImGui::EndChild();
					//if (UI::UIButton_1("Add Weapon/Item", "Add Weapon/Item", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
					//{
					//	AddWeaponPopup = true;
					//	LOG("ADDING WEAPON");
					//}
					///*==================================================================
					//							ADD WEAPON POPUP
					//==================================================================*/
					//if (AddWeaponPopup)
					//{
					//	ImGui::OpenPopup("Add Weapon");
					//	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					//	ImGui::SetNextWindowSize(ImVec2(700, 550));
					//	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
					//		ImGuiWindowFlags_NoScrollbar;
					//	if (ImGui::BeginPopupModal("Add Weapon", NULL, flags))
					//	{
					//		float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
					//		string sub = "Add Weapon";
					//		font_size = ImGui::GetFontSize() * sub.size() / 2;
					//		float center = RegionWidth - font_size;



					//		UI::SubHeader("Add Weapon");
					//		ImGui::Dummy({ 0, 20.f });
					//		ImGui::Columns(2, "Weapons", true);
					//		window = ImGui::GetCurrentWindow();

					//		columns = window->DC.CurrentColumns;
					//		columns->Flags = ColFlags;

					//		/*==================================================================
					//							ADD WEAPON COLUMN
					//		==================================================================*/
					//		ImGui::BeginChild("##weap", ImVec2(ImGui::GetColumnWidth(0) - 10.f, 300.0f));
					//		auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());
					//		float index = 1;

					//		const ImVec2 button_pos{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.1f };
					//		for (auto id : view)
					//		{
					//			auto& weap = instance.ecs.get<Components::Weapon>(id);
					//			bool isSelected = false;
					//			if (CurSelection == id)
					//				isSelected = true;

					//			if (UI::UIButton_2(weap.name.c_str(), weap.name.c_str(), { button_pos.x, button_pos.y + (index - 1) * 70 }, { 30.f, 20.f }, FONT_PARA, isSelected))
					//			{
					//				CurSelection = id;
					//				//LOG("CURRENT SELECTED: {0}", CurSelection);
					//			}
					//			index++;
					//		}
					//		ImGui::EndChild();
					//		/*==================================================================
					//							Create WEAPON COLUMN
					//		==================================================================*/
					//		ImGui::NextColumn();

					//		//ImGuiSelectableFlags flags = ImGuiSelectableFlags_
					//		ImGui::PushFont(FONT_BODY);
					//		//float CreateItmCol = ImGui::GetContentRegionAvail().x * 0.5f;
					//		string CreateItmStr = "Create New Item";
					//		/*font_size = ImGui::GetFontSize() * CreateItmStr.size() / 2;
					//		float CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
					//		ImGui::Dummy({ CreateItmCenter, 0 });
					//		ImGui::SameLine();*/
					//		if (UI::UISelectable(CreateItmStr.c_str()))
					//		{
					//			CreateOpen = true;
					//		}
					//		ImGui::Dummy({ 0, 10.f });

					//		string CreateWeapStr = "Create New Weapon";
					//		font_size = ImGui::GetFontSize() * CreateWeapStr.size() / 2;
					//		/*CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
					//		ImGui::Dummy({ CreateItmCenter, 0 });
					//		ImGui::SameLine();*/

					//		if (UI::UISelectable(CreateWeapStr.c_str()))
					//		{
					//			CreateOpen = true;
					//		}

					//		/*==================================================================
					//							Create New WEAPON POPUP
					//		==================================================================*/
					//		if (CreateOpen)
					//		{
					//			ImGui::OpenPopup("Add New Weapon");
					//			ImGui::SetNextWindowSize(ImVec2(800, 300));
					//			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					//			if (ImGui::BeginPopupModal("Add New Weapon", NULL, flags))
					//			{
					//				RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
					//				sub = "New Weapon/Item";
					//				font_size = ImGui::GetFontSize() * sub.size() / 2;
					//				center = RegionWidth - font_size;
					//				UI::SubHeader(sub.c_str());

					//				ImGui::Dummy({ 0, 20.f });

					//				ImGui::Columns(2, "Add New Weapon", true);
					//				window = ImGui::GetCurrentWindow();
					//				columns = window->DC.CurrentColumns;
					//				columns->Flags = ColFlags;
					//				ImGui::SetColumnWidth(0, 332.f);

					//				ImGui::NextColumn();
					//				ImGui::PushFont(FONT_PARA);
					//				float Padding_x = 60.f;
					//				ImGui::Text("Name");
					//				ImGui::SameLine();
					//				ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
					//				ImGui::SameLine();
					//				ImGui::InputText("##NewWeapName", &NewWeap.name);
					//				bool disabled = NewWeap.name.size() > 15;
					//				ImGui::SameLine();
					//				if (disabled)
					//				{
					//					ImGui::Text("15 Char only");
					//				}
					//				else
					//				{
					//					ImGui::Dummy({ 100.f, 10.f });
					//				}
					//				ImGui::Dummy({ 0, 10.f });
					//				ImGui::BeginChild("##NewWeapStats", { ImGui::GetColumnWidth(1) - 10.f, 100.f });
					//				for (auto i = 0, j = 0; i < sl->size(); i++)
					//				{
					//					if ((*sl)(i))
					//					{
					//						ImGui::Text(sl->operator[](i).c_str());
					//						ImGui::SameLine();
					//						ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
					//						ImGui::SameLine();
					//						ImGui::SetNextItemWidth(80.f);
					//						string WeapStats = "##WeapStats" + std::to_string(i);
					//						ImGui::InputInt(WeapStats.c_str(), &NewWeap.get_stat(i), 0);

					//						if (j % 2 == 0)
					//						{
					//							ImGui::SameLine();
					//							ImGui::Dummy({ 10.f, 0 });
					//							ImGui::SameLine();
					//						}
					//						j++;
					//					}
					//				}

					//				ImGui::EndChild();
					//				ImGui::PopFont();
					//				ImGui::EndColumns();
					//				if (disabled)
					//				{
					//					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					//					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					//				}
					//				if (UI::UIButton_1("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					//				{
					//					auto entity = instance.ecs.create();
					//					auto meta = instance.ecs.emplace<tc::Meta>(entity);
					//					meta->name = NewWeap.name;
					//					auto weapon = instance.ecs.emplace<tc::Weapon>(entity);
					//					weapon->name = NewWeap.name;
					//					for (auto i = 0; i < sl->size(); i++)
					//					{
					//						weapon->set_stat(i, NewWeap.get_stat(i));
					//					}
					//					ImGui::CloseCurrentPopup();
					//					NewWeap = tc::Weapon();
					//					CreateOpen = false;
					//				}
					//				if (disabled)
					//				{
					//					ImGui::PopItemFlag();
					//					ImGui::PopStyleVar();
					//				}
					//				if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
					//				{
					//					NewWeap = tc::Weapon();
					//					ImGui::CloseCurrentPopup();
					//					CreateOpen = false;
					//				}
					//				ImGui::EndPopup();
					//			}
					//		}
					//		ImGui::Dummy({ 0, 10.f });

					//		string DeleteWeapStr = "Delete Weapon";
					//		//font_size = ImGui::GetFontSize() * DeleteWeapStr.size() / 2;
					//		//CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
					//		/*ImGui::Dummy({ CreateItmCenter, 0 });
					//		ImGui::SameLine();*/
					//		if (UI::UISelectable(DeleteWeapStr.c_str()))
					//		{
					//			if (cs->weapon == CurSelection)
					//			{
					//				cs->weapon = UNDEFINED;
					//			}
					//			instance.ecs.emplace<tc::Destroyed>(CurSelection);
					//			// remove rb from scene
					//			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(CurSelection))
					//			{
					//				instance.po.RemoveActorFromScene(rb->internal_rb.get());
					//			}
					//			CurSelection = UNDEFINED;

					//			ImGui::CloseCurrentPopup();
					//			AddWeaponPopup = false;
					//		}
					//		ImGui::PopFont();
					//		ImGui::EndColumns();
					//		ImVec2 ConfirmPos = { 195, 400.f };
					//		if (UI::UIButton_2("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.75f }, { 0,10 }, FONT_PARA, false))
					//		{
					//			cs->weapon = CurSelection;
					//			ImGui::CloseCurrentPopup();
					//			AddWeaponPopup = false;
					//		}

					//		ImGui::EndPopup();
					//	}
					//}

					///*==================================================================
					//						SKILL COLUMN
					//==================================================================*/
					//ImGui::NextColumn();
					//ImGui::PushFont(FONT_BODY);
					//string SkillText = "Skill";
					//font_size = ImGui::GetFontSize() * SkillText.size() / 2;
					//float ThirdColWidth = ImGui::GetColumnWidth(2) / 2;
					//float ThirdoffsetX = ThirdColWidth - font_size + (font_size / 2);

					//ImGui::Dummy({ ThirdoffsetX, 0 });
					//ImGui::SameLine();
					//ImGui::Text(SkillText.c_str());
					//ImGui::PopFont();
					//ImGui::Dummy({ 0, 10.f });
					//ImGui::BeginChild("##SKILL", ImVec2(ImGui::GetColumnWidth(2) - 10.f, viewport->Size.y * 0.5f));


					//ImGui::EndChild();
					//if (UI::UIButton_1("Add Skill", "Add Skill", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
					//{

					//}
					//ImGui::EndColumns();

					//if (NameDisabled)
					//{
					//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

					//}
					//if (IsUnitCreation)
					//{
					//	if (UI::UIButton_2("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.80f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 0.f, 10.f }, FONT_PARA))
					//	{
					//		LOG("SAVED");
					//		if (IsUnitCreation)
					//		{
					//			auto entity = instance.ecs.create();
					//			auto meta = instance.ecs.emplace<tc::Meta>(entity);
					//			meta->name = cs->name;
					//			auto character = instance.ecs.emplace<tc::Character>(entity);
					//			character->name = cs->name;

					//			for (auto i = 0; i < tc::STAT_TOTAL; i++)
					//			{

					//				character->set_stat(i, cs->get_stat(i));
					//			}
					//		}
					//		OverlayOpen = false;
					//		NewCharacter = tc::Character();
					//	}
					//	if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.92f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 0.f, 10.f }, FONT_PARA))
					//	{
					//		OverlayOpen = false;
					//		NewCharacter = tc::Character();
					//	}
					//}
					//else
					//{
					//	if (UI::UIButton_2("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.92f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 0.f, 10.f }, FONT_PARA))
					//	{
					//		OverlayOpen = false;
					//	}
					//}
					//if (NameDisabled)
					//{
					//	ImGui::PopItemFlag();
					//	ImGui::PopStyleVar();
					//}

				}
				
			}
			
			ImGui::PopStyleVar();
			ImGui::End();
		}
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
		Tabs[TABS_TYPE::UNIT].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/UnitTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::UNIT].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/UnitTabLit.png"]->GetID());
		Tabs[TABS_TYPE::UNIT].size = ImVec2{ static_cast<float>(tex_map["Assets/UnitTabUnlit.png"]->GetWidth()), 
											 static_cast<float>(tex_map["Assets/UnitTabUnlit.png"]->GetHeight()) };

		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponTabLit.png"]->GetID());
		Tabs[TABS_TYPE::WEAPON].size = ImVec2{ static_cast<float>(tex_map["Assets/WeaponTabUnlit.png"]->GetWidth()), 
											   static_cast<float>(tex_map["Assets/WeaponTabUnlit.png"]->GetHeight()) };

		Tabs[TABS_TYPE::ITEM].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/ItemTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::ITEM].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/ItemTabLit.png"]->GetID());
		Tabs[TABS_TYPE::ITEM].size = ImVec2{ static_cast<float>(tex_map["Assets/ItemTabUnlit.png"]->GetWidth()),
											 static_cast<float>(tex_map["Assets/ItemTabUnlit.png"]->GetHeight()) };

		Tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/ActionTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/ActionTabLit.png"]->GetID());
		Tabs[TABS_TYPE::ACTION].size = ImVec2{ static_cast<float>(tex_map["Assets/ActionTabUnlit.png"]->GetWidth()),
											   static_cast<float>(tex_map["Assets/ActionTabUnlit.png"]->GetHeight()) };
	}

	void UnitSheetOverlay::display_unit_stats(const ImGuiViewport& viewport, Instance& instance) const
	{
		if (!cs)
			return; 

		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });
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
				string label = "##" + stat;
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
		ImGui::ColorEdit3("##color", glm::value_ptr(cs->color), ImGuiColorEditFlags_AlphaBar | 0);
		ImGui::EndChild();
	}

	void UnitSheetOverlay::display_weapon_stats(const ImGuiViewport& viewport, Instance& instance) const
	{
		if (!cs)
			return;

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });
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
	}

	void UnitSheetOverlay::display_items(const ImGuiViewport& viewport, Instance& instance) const
	{
		if (!cs)
			return;

		(void)viewport;
		(void)instance;
	}

	void UnitSheetOverlay::display_actions(const ImGuiViewport& viewport, Instance& instance) const
	{
		if (!cs)
			return;

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });
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
	}
}