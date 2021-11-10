/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "TurnOrderOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	void TurnOrderOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;

		turn_order_state = TURN_ORDER_STATE::ORDER_ADD_UNITS;
	}

	void TurnOrderOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.0f,0.0f,0.0f,0.8f});
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Turn Order Overlay", nullptr, window_flags))
			{
				// title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Add Units");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// draw the divider
				ImVec2 winMin = { viewport->Size.x * 0.43f, viewport->Size.y * 0.25f };
				ImVec2 winMax = { viewport->Size.x * 0.4305f,viewport->Size.y * 0.85f };
				ImVec4 col = { 0.8f, 0.8f, 0.8f, 1.f };

				ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

				// text displays
				std::string text = "Select and add the units from the list that you want you in the game.";
				ImGui::PushFont(FONT_BODY);
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.2f - ImGui::CalcTextSize(text.c_str()).x * 0.5f, viewport->Size.y * 0.25f });
				ImGui::Text(text.c_str());

				text = "Currently selected units";
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.695f - ImGui::CalcTextSize(text.c_str()).x * 0.5f, viewport->Size.y * 0.25f });
				ImGui::Text(text.c_str());
				ImGui::PopFont();

				// character selection section
				ImGui::SetCursorPos(ImVec2{0, viewport->Size.y * 0.3f});
				if (ImGui::BeginChild("Character adding", ImVec2{viewport->Size.x * 0.35f, viewport->Size.y * 0.5f }, true))
				{
					for (auto id : instance.ecs.view<Components::Character>())
					{
						// render the character buttons
						// add it to a list please
					}
				}
				ImGui::EndChild();

				// character added section
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.47f, viewport->Size.y * 0.3f });
				if (ImGui::BeginChild("Character added", ImVec2{ viewport->Size.x * 0.45f, viewport->Size.y * 0.5f }, true))
				{
					// TODO: loop through the added units here
				}
				ImGui::EndChild();

				// back button
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				auto tex = tex_map["Assets/BackMenuBtn.png"];

				if (ImGui::ImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(true);
				}

				ImGui::PopStyleColor(3);

				// save
				if (UI::UIButton_2("Next", "Next", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.9f }, {0,0}, FONT_PARA))
				{

					// change the state
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
}