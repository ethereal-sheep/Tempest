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
				auto image = tex_map["Assets/SelectCR_BG.dds"];
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
				image = tex_map["Assets/BackMenuBtn.dds"];

				if (ImGui::ImageButton((void*)static_cast<size_t>(image->GetID()), ImVec2{ image->GetWidth() * 0.7f, image->GetHeight() * 0.7f }))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<OpenBuildModeOverlay>();
				}
				ImGui::PopStyleColor(3);
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0,0,0,0 });

				// draw the child
				const ImVec2 child_size{ viewport->Size.x * 0.25f, viewport->Size.y * 0.55f };
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - child_size.x * 0.5f, viewport->Size.y * 0.55f - child_size.y * 0.5f });
				if (ImGui::BeginChild("##LoadConflictRes", child_size, true))
				{
					const ImVec2 cusor{ ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + 40.0f };
					// TODO: load the conflict stuff here

					//for (int i = 0; i < SelectedConflictRes; i++)
					{
						ImGui::PushID(std::string{ "Conflict Res " + std::to_string(0) }.c_str());
						if (UI::UIButton_2("Sample_Conflict", "Sample_Conflict", ImVec2{ cusor.x, cusor.y + 0 * 90.0f }, { 50,20 }, FONT_BTN, SelectedConflictRes == 0))
						{
							SelectedConflictRes = 0;
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

					int i = 0;
					std::string seq_name = "Unit vs Unit";
					ImGui::PushID(seq_name.c_str());
					//bool selected = SelectedSequences.size();
					if (UI::UIButton_2(seq_name.c_str(), seq_name.c_str(), ImVec2{ cusor.x, cusor.y + i * 90.0f }, { 50, 20 }, FONT_BTN, false))
					{
						Service<EventManager>::Get().instant_dispatch<MainMenuSequencePopupTrigger>(SelectedSequences);
					}
					ImGui::PopID();

					++i;
				}

				ImGui::EndChild();
				ImGui::PopStyleColor(1);

				ImGui::PushID("conflictresnext");
				if (UI::UIButton_2("Next", "Next", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.95f }, { -20,20 }, FONT_BTN))
				{
					OverlayOpen = false;
					//Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Saving...");
					dynamic_cast<EditTimeInstance&>(instance).save();
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