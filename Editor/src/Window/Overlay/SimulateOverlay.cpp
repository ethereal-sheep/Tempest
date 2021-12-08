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

#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Triggers/SimulationTriggers.h"
#include "Triggers/Triggers.h"
#include "Instance/EditTimeInstance.h"
#include "InstanceManager/InstanceConfig.h"

namespace Tempest
{
	void SimulateOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenSimulateTrigger>(e);
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;
		attacker.Reset(a.instance);
		defender.Reset(a.instance);

		if (!a.instance.ecs.view<Components::Graph>(exclude_t<tc::Destroyed>()).contains(sequence))
		sequence = UNDEFINED;

		padding = ImGui::GetMainViewport()->Size.y * 0.02f;
		win = 0;
		lose = 0;
		enter_button = tex_map["Assets/EnterButton.dds"];
	}

	void SimulateOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::SIMULATE)
			OverlayOpen = false;
	}

	void SimulateOverlay::confirm_data(const Event& e)
	{
		auto a = event_cast<SimulateSelectionConfirm>(e);

		UnitData& owner = a.is_attacker ? attacker : defender;
		switch (a.type)
		{
		case SIMULATE_POPUP_TYPE::UNIT:
			owner.unit_id = a.data;
			break;
		case SIMULATE_POPUP_TYPE::WEAPON:
			owner.weapon = a.data;
			break;
		case SIMULATE_POPUP_TYPE::ACTION:
			owner.action = a.data;
			break;
		case SIMULATE_POPUP_TYPE::SEQUENCE:
			sequence = a.data;
			break;
		default:
			break;
		}
	}

	void SimulateOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Select Unit Sheet", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/SimulationBG.dds"];
				{

					ImVec2 Min{ 0,0 };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y};
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// sequence display
				ImGui::PushFont(FONT_BODY);
				const std::string seq_title{ "Sequence" };
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - (ImGui::CalcTextSize(seq_title.c_str()).x  + ImGui::GetFontSize() * 0.5f) * 0.5f, viewport->Size.y * 0.2f });
				ImGui::Text(seq_title.c_str());
				ImGui::PopFont();

				// display chosen sequence
				std::string seq_name{ "SELECT SEQUENCE" };
				if (sequence != UNDEFINED)
					seq_name = instance.ecs.get<tc::Graph>(sequence).g.name;

				if (UI::UIButton_Sequence(instance, sequence, seq_name.c_str(), seq_name.c_str(), { viewport->Size.x * 0.5f, viewport->Size.x * 0.15f }, { 0,0 }, FONT_PARA))
				{
					Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
						SIMULATE_POPUP_TYPE::SEQUENCE, false, sequence);
				}
				push_button_style();
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.545f, viewport->Size.y * 0.24f });
				ImGui::PushID("seq");
				if (sequence != UNDEFINED && ImGui::ImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
				{
					Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
					Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(sequence, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE);
				}
				ImGui::PopID();
				pop_button_style();

				// success title
				tex = tex_map["Assets/CoSTitle.dds"];
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.4f - tex->GetHeight() * 0.5f });
				ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2((float)tex->GetWidth(), (float)tex->GetHeight()));

				// display number percentage
				float result = 0.0f;

				// display number percentage
				if (win + lose)
				{
					result = 100.f * win / (win + lose);
					ImGui::SetCursorPos({ viewport->Size.x * 0.465f, viewport->Size.y * 0.55f });
					ImGui::PushFont(FONT_HEAD);
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(232, 137, 64, 255));
					ImGui::Text("%.1f %%", result);
					ImGui::PopStyleColor();
					ImGui::PopFont();
				}

				// render progress bar
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - 350.0f * 0.5f, viewport->Size.y * 0.45f });
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(232, 137, 64, 255));
				ImGui::ProgressBar(result * 0.01f, { 350.0f, 50.0f });
				ImGui::PopStyleColor();

				ImGui::SetCursorPos({ viewport->Size.x * 0.45f, viewport->Size.y * 0.65f });
				ImGui::PushFont(FONT_PARA);
				ImGui::Text("Frequency ");
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::PushItemWidth(100.f);
				ImGui::InputScalar("", ImGuiDataType_U32, &freq, 0);
				freq = std::clamp(freq, 100u, 50000u);
				ImGui::PopItemWidth();
				
				// attack section
				display_unit_section(instance, { viewport->Size.x * 0.18f,viewport->Size.y * 0.5f }, true);
				display_unit_section(instance, { viewport->Size.x * 0.82f,viewport->Size.y * 0.5f }, false);

				if (UI::UIButton_2("Simulate", "Simulate", { viewport->Size.x * 0.43f, viewport->Size.y * 0.72f }, { 10.f, 10.f }, FONT_PARA))
				{
					// TODO: check for invalid inputs
					freq = std::clamp(freq, 1u, 100000u);

					// equip your stuff here
					attacker.unit_id;
					defender.unit_id;

					if (auto character = instance.ecs.get_if<tc::Character>(attacker.unit_id))
					{
						character->chosen_weapon = attacker.weapon;
					}
					if (auto character = instance.ecs.get_if<tc::Character>(defender.unit_id))
					{
						character->chosen_weapon = defender.weapon;
					}

					Service<EventManager>::Get().instant_dispatch<SimulateConflict>(attacker.unit_id, defender.unit_id, attacker.action, defender.action, sequence, freq, win, lose, attack, defend, finish);
				}

				if (UI::UIButton_2("Custom Map", "Custom Map", { viewport->Size.x * 0.57f, viewport->Size.y * 0.72f }, { 10.f, 10.f }, FONT_PARA))
				{
					// open testing combat in map
					auto& edit = dynamic_cast<EditTimeInstance&>(instance);
					edit.save();

					if (instance.ecs.view_first<tc::Character>() && instance.ecs.view_first<tc::ConflictGraph>())
					{
						Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(5);
						OverlayOpen = false;
					}
					else if (!instance.ecs.view_first<tc::Character>() && !instance.ecs.view_first<tc::ConflictGraph>())
					{
						Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
					}
					else if (!instance.ecs.view_first<tc::ConflictGraph>())
					{
						Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Sequence found!");
					}
					else
					{
						Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit found!");
					}

					/*Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
						edit.get_full_path(),
						MemoryStrategy{},
						InstanceType::RUN_TIME);*/
				}

				// display top buttons
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						OverlayOpen = false;
						Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(3);
						if (auto edit_instance = dynamic_cast<EditTimeInstance*>(&instance))
						{
							Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Saving...");
							Service<EventManager>::Get().instant_dispatch<SaveProjectTrigger>();
						}
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/QuickMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
					}
					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/TutorialBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>();
					}
					ImGui::PopStyleColor(3);
				}
				

				{
					//// Drag drop section
			//{
			//	const float availRegion = ImGui::GetContentRegionAvail().y * 0.3f;
			//	auto curr_tex = tex_map["Assets/ButtonSlot.png"];
			//	const ImVec2 button_slot_size{ curr_tex->GetWidth() * 0.68f, curr_tex->GetHeight() * 0.67f };

			//	ImGui::BeginChild("##DragDropSectionSimulate", ImVec2(contentSize, availRegion), false, window_flags | ImGuiWindowFlags_NoScrollbar);

			//	// Attacker section
			//	{
			//		ImGui::BeginChild("##DragDropAttackerSimulate", ImVec2(contentSize * 0.45f, availRegion - 2.0f), false, window_flags );

			//		// Attacker image
			//		ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.11f });
			//		ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Attacker.png"]->GetID()), {tex_map["Assets/Attacker.png"]->GetWidth() * 0.9f, tex_map["Assets/Attacker.png"]->GetHeight() * 0.9f });

			//		ImGui::SameLine();

			//		// Button slot
			//		ImVec2 buttonPos{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.45f - button_slot_size.x * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f - button_slot_size.y * 0.5f};
			//		
			//		ImGui::SetCursorPos(buttonPos);
			//		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), button_slot_size); // Image draws from top left
			//		if (ImGui::BeginDragDropTarget())
			//		{
			//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
			//			{
			//				Attacker = *(const Entity*)payload->Data;
			//			}

			//			ImGui::EndDragDropTarget();
			//		}

			//		if (Attacker != UNDEFINED)
			//		{
			//			auto& character = instance.ecs.get<tc::Character>(Attacker);
			//			if (UI::UIButton_2(character.name.c_str(), character.name.c_str(), ImVec2{ buttonPos.x + button_slot_size.x * 0.5f, buttonPos.y + button_slot_size.y * 0.5f}, { 60, 15 }, FONT_PARA))
			//				Attacker = UNDEFINED;
			//		}

			//		ImGui::EndChild();
			//	
			//	}
			//	ImGui::SameLine();
			//	ImGui::BeginChild("##VSChild", ImVec2(contentSize * 0.1f, availRegion - 2.0f), false, window_flags);
			//	ImGui::PushFont(FONT_HEAD);
			//	ImGui::Dummy({ 0.f, ImGui::GetContentRegionAvail().y * 0.41f });
			//	ImGui::Dummy({ ImGui::GetContentRegionAvail().x * 0.25f, 0.f });
			//	ImGui::SameLine();
			//	ImGui::Text("VS");
			//	ImGui::PopFont();
			//	ImGui::EndChild();
			//	ImGui::SameLine(0,0);

			//	// Defender section
			//	{
			//		ImGui::BeginChild("##DragDropDefenderSimulate", ImVec2(contentSize * 0.45f, availRegion - 2.0f), false, window_flags );

			//		ImGui::SameLine();

			//		// Button slot
			//		ImVec2 buttonPos{ ImGui::GetCursorPosX() - 20.0f * 0.5f + ImGui::GetContentRegionAvailWidth() * 0.35f - button_slot_size.x * 0.5f,
			//						  ImGui::GetCursorPosY() + 15.0f * 0.5f + ImGui::GetContentRegionAvail().y * 0.5f - button_slot_size.y * 0.5f};

			//		ImGui::SetCursorPos(buttonPos);
			//		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), button_slot_size); // Image draws from top left
			//		if (ImGui::BeginDragDropTarget())
			//		{
			//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
			//			{
			//				Defender = *(const Entity*)payload->Data;
			//			}

			//			ImGui::EndDragDropTarget();
			//		}

			//		
			//		ImGui::SameLine();

			//		// Defender image
			//		ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.56f - (tex_map["Assets/Defender.png"]->GetWidth() * 0.9f) * 0.5f , ImGui::GetCursorPosY() - (tex_map["Assets/Defender.png"]->GetHeight() * 0.9f) * 0.5f + 15.0f});
			//		ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Defender.png"]->GetID()), { tex_map["Assets/Defender.png"]->GetWidth() * 0.9f, tex_map["Assets/Defender.png"]->GetHeight() * 0.9f });

			//		if (Defender != UNDEFINED)
			//		{
			//			auto& character = instance.ecs.get<tc::Character>(Defender);
			//			if (UI::UIButton_2(character.name.c_str(), character.name.c_str(), ImVec2{ buttonPos.x + button_slot_size.x * 0.5f, buttonPos.y + button_slot_size.y * 0.5f }, { 60, 15 }, FONT_PARA))
			//				Defender = UNDEFINED;
			//		}
			//		ImGui::EndChild();
			//	}
			//	ImGui::EndChild();
			//}

			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

			//// Content selection section
			//ImGui::BeginChild("##ContentSectionSimulate", ImVec2(contentSize, ImGui::GetContentRegionAvail().y * 0.8f), false, ImGuiWindowFlags_NoScrollWithMouse);
			//
			//const auto regoinAvailWidth = ImGui::GetContentRegionAvailWidth() / 3.0f - Padding;
			//const auto regoinAvailHeight = ImGui::GetContentRegionAvail().y;

			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - regoinAvailWidth * 3 - Padding) * 0.4f);
			//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + HalfPadding);

			//{
			//	ImGui::BeginChild("##LeftSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));
			//	const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
			//	auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

			//	unsigned i = 0;
			//	for (auto id : view)
			//	{
			//		ImGui::PushID(id);
			//		ImGui::BeginGroup();
			//		const ImVec2 pos{ cursor.x , cursor.y + i++ * 80 };

			//		auto& Charac = instance.ecs.get<tc::Character>(id);
			//		if (UI::UIButton_2(Charac.name.c_str(), Charac.name.c_str(), pos, { 60, 15 }, FONT_PARA)) {}

			//		ImGui::EndGroup();
			//
			//		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			//		{
			//			ImGui::SetDragDropPayload("UNIT_CONTENT_ITEM", &id, sizeof(Entity));
			//			UI::UIButton_2(Charac.name.c_str(), Charac.name.c_str(), { cursor.x , cursor.y }, { 60, 15 }, FONT_PARA);
			//			ImGui::EndDragDropSource();
			//		}

			//		ImGui::PopID();
			//
			//	}

			//	ImGui::EndChild();

			//}

			//ImGui::SameLine();
			//ImGui::Dummy({ HalfPadding, 0 });
			//ImGui::SameLine();

			//// Add more things ehre
			//{
			//	ImGui::BeginChild("##MiddleSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));

			//	const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
			//	int index = 0;
			//	for (auto id : instance.ecs.view<tc::ActionGraph>())
			//	{
			//		auto& g = instance.ecs.get<tc::Graph>(id);
			//		g.g.get_name();
			//		bool selected = false;
			//		if (id == ActionID)
			//			selected = true;
			//		if (UI::UIButton_2(g.g.get_name() + std::to_string(id), g.g.get_name() + std::to_string(id), { cursor.x , cursor.y + index * 80 }, { 60.f, 15.f }, FONT_PARA, selected))
			//		{
			//			//OverlayOpen = false;
			//			//Service<EventManager>::Get().instant_dispatch<OpenActionGraphTrigger>(id, instance);
			//			
			//			if (selected == true)
			//				ActionID = UNDEFINED;
			//			else
			//				ActionID = id;

			//		}
			//		index++;
			//	}

			//	ImGui::EndChild();

			//}

			//ImGui::SameLine();
			//ImGui::Dummy({ HalfPadding, 0 });
			//ImGui::SameLine();

			//{
			//	ImGui::BeginChild("##RightSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));

			//	const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
			//	int index = 0;
			//	for (auto id : instance.ecs.view<tc::ConflictGraph>())
			//	{
			//		auto& g = instance.ecs.get<tc::Graph>(id);
			//		bool selected = false;
			//		if (id == LinkID)
			//			selected = true;
			//		g.g.get_name();
			//		if (UI::UIButton_2(g.g.get_name() + std::to_string(id), g.g.get_name() + std::to_string(id), { cursor.x + 10.f , cursor.y + index * 80 }, { 60.f, 15.f}, FONT_PARA, selected))
			//		{
			//			// make sure i can simulate
			//			// 1. have atk and def 
			//			// 2. atk and def have characters
			//			// 3. have conflict and conflict graph
			//			

			//			if (selected == true)
			//				LinkID = UNDEFINED;
			//			else
			//				LinkID = id;
			//			//if (Defender != INVALID &&
			//			//	Attacker != INVALID &&
			//			//	instance.ecs.has<tc::Character>(Defender) &&
			//			//	instance.ecs.has<tc::Character>(Attacker)
			//			//)
			//			//{

			//			//}

			//		}
			//		index++;
			//	}
			//	ImGui::EndChild();
			//}

			//ImGui::EndChild();

			//// Cancel button
			//

			//// Simulate button
			//bool disable = false;
			//if (LinkID == UNDEFINED || Attacker == UNDEFINED || Defender == UNDEFINED)
			//	disable = true;
			//
			//if (disable)
			//{
			//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			//}
			//if (UI::UIButton_2("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
			//{
			//	Service<EventManager>::Get().instant_dispatch<OpenSimulateResultTrigger>(Attacker, Defender, LinkID);
			//	OverlayOpen = false;
			//}
			//if (UI::UIButton_2("Custom Map", "Custom Map", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.75f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
			//{

			//}
			//if (disable)
			//{
			//	ImGui::PopItemFlag();
			//	ImGui::PopStyleVar();
			//}
			//if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.895f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
			//{
			//	OverlayOpen = false;
			//}
				}
			
			}

			ImGui::PopStyleVar();

			ImGui::End();

		}
	}
	void SimulateOverlay::display_unit_section(Instance& instance, const ImVec2 start_pos, bool is_attacker)
	{
		Entity* temp = is_attacker ? &attacker.unit_id : &defender.unit_id;
		ImVec4 tint{ 1,1,1,1 };

		// character display
		ImGui::SetCursorPos(ImVec2{start_pos.x - 35.0f, start_pos.y - 60.0f });
		auto tex = tex_map["Assets/CharacterIcon.dds"];
		std::string chara_name{ "CHARACTER" };
		if (*temp != UNDEFINED)
		{
			auto charac = instance.ecs.get<tc::Character>(*temp);
			chara_name = charac.name;
			tint = ImVec4{ charac.color.x, charac.color.y,charac.color.z,1 };
		}
			
		
		if (UI::UICharButton_Toggle((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f },
			chara_name, "##charaname" + is_attacker, * temp != UNDEFINED, { 0, 0 }, { 1,1 }, 2, ImVec4{ 0,0,0,0 }, tint))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::UNIT, is_attacker, *temp);
		}

		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() - 15.0f, ImGui::GetCursorPosY() + 15.0f});
		push_button_style();
		ImGui::PushID("chara" + is_attacker);
		if (*temp != UNDEFINED && ImGui::ImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button ->GetHeight() * 1.0f}))
		{
			OverlayOpen = false;
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::EDIT_UNIT, is_attacker, *temp);
		}
		ImGui::PopID();
		pop_button_style();

		// weapon
		temp = is_attacker ? &attacker.weapon : &defender.weapon;
		ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 4.0f });
		if (UI::UIButton_Weapon(instance, *temp, "SELECT WEAPON", "SELECT WEAPON", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::WEAPON, is_attacker, *temp);
		}
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 10.0f });
		push_button_style();
		ImGui::PushID("weapon" + is_attacker);
		if (*temp != UNDEFINED && ImGui::ImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
		{
			OverlayOpen = false;
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::EDIT_WEAPON, is_attacker, *temp);
		}
		ImGui::PopID();
		pop_button_style();


		// action
		temp =  is_attacker ? &attacker.action : &defender.action;
		ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 7.0f });
		ImGui::PushID("action" + is_attacker);
		if (UI::UIButton_Action(instance, *temp,"SELECT ACTION", "SELECT ACTION", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::ACTION, is_attacker, *temp);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 10.0f });
		push_button_style();
		if (*temp != UNDEFINED && ImGui::ImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button ->GetHeight() * 1.0f}))
		{
			OverlayOpen = false;
			Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
			Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(*temp, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION);
		}
		pop_button_style();
	}

	void SimulateOverlay::push_button_style() const
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
	}

	void SimulateOverlay::pop_button_style() const
	{
		ImGui::PopStyleColor(3);
	}

	void SimulateOverlay::UnitData::Reset(Instance& instance)
	{
		if (!instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()).contains(unit_id))
			unit_id = UNDEFINED;

		if (!instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>()).contains(weapon))
			weapon = UNDEFINED;

		if (!instance.ecs.view<Components::Graph>(exclude_t<tc::Destroyed>()).contains(action))
			action = UNDEFINED;
	}
}