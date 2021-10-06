#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	void CombatModeOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
	}

	void CombatModeOverlay::show(Instance&)
	{
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.f});

			if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
			{

				if (ImGui::Button("X"))
					OverlayOpen = false;
                
                auto drawlist = ImGui::GetWindowDrawList();
				const ImVec2 cursorPosition{ ImGui::GetCursorPos() };

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + viewport->Size.y);
				ImVec2 point = ImGui::GetCursorScreenPos();

                // Setting character info bg
				{
					auto tex = tex_map["Assets/CharacterBackdrop.png"];
					ImVec2 Min {point.x, point.y - viewport->Size.y * 0.32f};
					ImVec2 Max {Min.x + tex->GetWidth(), Min.y + tex->GetHeight()};
					drawlist->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}
                

				// Setting bootiful elmo
				{
					auto tex = tex_map["Assets/test_photo2.png"];
					ImVec2 Min {point.x, point.y - tex->GetHeight() * 0.7f };
					ImVec2 Max {Min.x + tex->GetWidth() * 0.7f, Min.y + tex->GetHeight() * 0.7f };
					drawlist->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// Shift position
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + viewport->Size.x * 0.65f);
				point = ImGui::GetCursorPos();

				// Setting actions info bg
				{
					auto tex = tex_map["Assets/ActionBackdrop.png"];
					ImVec2 Min{ point.x, point.y - viewport->Size.y * 0.32f };
					ImVec2 Max{ Min.x + tex->GetWidth(), Min.y + tex->GetHeight() };
					drawlist->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// Addding of buttons
				if (UI::UIButton_2("More Information >", "More Information >", { cursorPosition.x + viewport->Size.x * 0.3f, cursorPosition.y + viewport->Size.y * 0.65f }, { 0.f, 8.0f }, FONT_BODY))
				{
				}

				if (UI::UIButton_1("End Turn", "End Turn", { cursorPosition.x + viewport->Size.x * 0.92f, cursorPosition.y + viewport->Size.y * 0.65f }, { 60.f, 1.0f }, FONT_BODY))
				{
				}
			}

            ImGui::End();

			ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
            
		}
	}
}