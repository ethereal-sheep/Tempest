/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "Triggers/SimulationTriggers.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	void SimulateOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;
		attacker.Reset();
		defender.Reset();
		sequence = UNDEFINED;

		padding = ImGui::GetMainViewport()->Size.y * 0.02f;
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
				// title
				const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.9f;
				string text = "Simulate";

				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f});

				// simulate backdrop
				const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

				auto tex = tex_map["Assets/SimulationBackdrop.png"];
				//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
				ImVec2 point = ImGui::GetCursorPos();
				{

					ImVec2 Min{ point.x, point.y };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y * 0.73f };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// sequence display
				ImGui::PushFont(FONT_HEAD);
				const std::string seq_title{ "Sequence List" };
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - (ImGui::CalcTextSize(seq_title.c_str()).x  + ImGui::GetFontSize() * 0.5f) * 0.5f, viewport->Size.y * 0.2f });
				ImGui::Text("Sequence List");
				ImGui::PopFont();

				const ImVec2 childSize{ viewport->Size.x * 0.2f, viewport->Size.y * 0.5f }; //SHIFT to local
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - childSize.x * 0.5f,viewport->Size.y * 0.5f - childSize.y * 0.5f});
				ImGui::BeginChild("##SequenceSection", childSize, true);
				{
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 180, ImGui::GetCursorPosY() + 30 };

					unsigned i = 0;
					for (auto id : instance.ecs.view<tc::ConflictGraph>())
					{
						ImGui::PushID(id);
						//ImGui::BeginGroup();
						const ImVec2 pos{ cursor.x , cursor.y + i++ * 80 };

						auto& conflict = instance.ecs.get<tc::Graph>(id);

						if (UI::UIButton_2(conflict.g.name + ": " + std::to_string(i), conflict.g.name + ": " + std::to_string(i), pos, {70.0f,20.0f}, FONT_PARA))
						{
							sequence = id;
						}

						ImGui::PopID();
					}
				}
				ImGui::EndChild();


				// attack section
				DisplayUnitSection({ viewport->Size.x * 0.18f,viewport->Size.y * 0.5f }, true);
				DisplayUnitSection({ viewport->Size.x * 0.82f,viewport->Size.y * 0.5f }, false);


				if (UI::UIButton_2("Back", "Back", { viewport->Size.x * 0.1f, viewport->Size.y * 0.9f }, { 0,0 }, FONT_PARA))
				{
					OverlayOpen = false;
				}

				if (UI::UIButton_2("Next", "Next", { viewport->Size.x * 0.9f, viewport->Size.y * 0.9f }, { 0,0 }, FONT_PARA))
				{
					// for testing here
					//auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

					//// for testing here
					//for (auto id : view)
					//{
					//	if (attacker.unit_id == UNDEFINED)
					//		attacker.unit_id = id;
					//	else if (defender.unit_id == UNDEFINED)
					//		defender.unit_id = id;
					//}
					Service<EventManager>::Get().instant_dispatch<OpenSimulateResultTrigger>(attacker.unit_id, defender.unit_id, sequence);
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
	void SimulateOverlay::DisplayUnitSection(const ImVec2 start_pos, bool is_attacker)
	{
		ImGui::SetCursorPos(start_pos);

		if (UI::UIButton_2("CHARA", "CHARA", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::UNIT, is_attacker, is_attacker ? attacker.unit_id : defender.unit_id);
		}

		// character mame
		const std::string char_name{ "Character Name" };
		ImGui::SetCursorPos({ start_pos.x - (ImGui::CalcTextSize(char_name.c_str()).x + ImGui::GetFontSize()) * 0.5f, start_pos.y + padding });
		ImGui::Text(char_name.c_str());

		ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 4.0f });
		if (UI::UIButton_2("SELECT WEAPON", "SELECT WEAPON", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::WEAPON, is_attacker, is_attacker ? attacker.weapon : defender.weapon);
		}
		ImGui::SetCursorPos({ start_pos.x, start_pos.y + padding * 7.0f });
		if (UI::UIButton_2("SELECT ACTION", "SELECT ACTION", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::ACTION, is_attacker, is_attacker ? attacker.action : defender.action);
		}
	}
}