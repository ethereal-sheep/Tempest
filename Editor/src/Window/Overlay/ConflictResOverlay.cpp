/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "ConflictResOverlay.h"
#include <Editor/src/InstanceManager/InstanceConfig.h>
#include <Tempest/src/Instance/EditTimeInstance.h>

namespace Tempest
{
	void ConflictResOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		SelectedConflictRes = 0;
		SelectedSequences.clear();
	}
	void ConflictResOverlay::show([[maybe_unused]]Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
			window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
			if (ImGui::Begin("ConflictResolution", nullptr, window_flags))
			{
				// render the select map image
				auto image = tex_map["Assets/SelectCR_BG.png"];
				ImVec2 point = ImGui::GetCursorScreenPos();
				ImVec2 Min{ point.x, point.y };
				ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
				ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(image->GetID()), Min, Max);

				// render title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Select Conflict Resolution");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });


				// render back button
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				image = tex_map["Assets/BackMenuBtn.png"];

				if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
					OverlayOpen = false;

				ImGui::PopStyleColor(3);

				// draw the child
				const ImVec2 child_size{ viewport->Size.x * 0.25f, viewport->Size.y * 0.55f };
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - child_size.x * 0.5f, viewport->Size.y * 0.55f - child_size.y * 0.5f });
				if (ImGui::BeginChild("##LoadConflictRes", child_size, true))
				{
					const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };
					// TODO: load the conflict stuff here

					for (int i = 0; i < 4; i++)
					{
						ImGui::PushID(std::string{ "ConflictRes" + std::to_string(i) }.c_str());
						if (UI::UIButton_2("Sample_Conflict", "Sample_Conflict", ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50,20 }, FONT_BTN, SelectedConflictRes == i))
						{
							SelectedConflictRes = i;
						}
						ImGui::PopID();
					}
				}

				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.8f - child_size.x * 0.5f, viewport->Size.y * 0.55f - child_size.y * 0.5f });
				if (ImGui::BeginChild("##LoadSequenceConflictRes", child_size, true))
				{
					const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };

					// TODO: render all the sequences from selected conflict
					// TODO: make a popup menu
					for (int i = 0; i < 8; i++)
					{
						std::string seq_name = "Sample_Seq" + std::to_string(i);
						ImGui::PushID(seq_name.c_str());
						bool selected = std::find(SelectedSequences.begin(), SelectedSequences.end(), i) != SelectedSequences.end();
						if (UI::UIButton_2(seq_name.c_str(), seq_name.c_str(), ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50,20 }, FONT_BTN, selected))
						{
							Service<EventManager>::Get().instant_dispatch< MainMenuSequencePopupTrigger>(SelectedSequences);
						}
						ImGui::PopID();
					}
				}

				ImGui::EndChild();

				ImGui::PushID("conflictresnext");
				if (UI::UIButton_2("Next", "Next", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.95f }, { -20,20 }, FONT_BTN))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
						dynamic_cast<EditTimeInstance&>(instance).get_full_path(),
						MemoryStrategy{},
						InstanceType::RUN_TIME);
				}
				ImGui::PopID();
			}
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			
		}
	}
}