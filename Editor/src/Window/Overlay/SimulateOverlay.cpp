#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	void SimulateOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	}

	void SimulateOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			if (ImGui::Begin("Select Unit Sheet", nullptr, window_flags))
			{
				float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;

				UI::SubHeader(ImVec2{ center_x - 100.f,50 }, "Simulate");
                ImGui::Dummy(ImVec2{ center_x - 100.f,0.f});
                ImGui::SameLine();
				ImGui::Text("Simulate your resolution here with up to two selected units.");
				ImGui::BeginChild("##ContentSectionSimulate", ImVec2(ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetContentRegionAvail().y * 0.5f), true, ImGuiWindowFlags_NoScrollWithMouse);
				const auto regoinAvailWidth = ImGui::GetContentRegionAvailWidth() / 3.0f - Padding;
				const auto regoinAvailHeight = ImGui::GetContentRegionAvail().y;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - regoinAvailWidth * 3 - Padding) * 0.25f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + HalfPadding);

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##LeftSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding), true, window_flags);
					if (!ImGui::IsWindowHovered())
						bgColor = IM_COL32(0, 0, 0, 100);
					else
						bgColor = IM_COL32(20, 20, 20, 100);
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Padding);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + regoinAvailHeight / 3.0f);

						ImGui::BeginChild("ChildUnitSimulate", ImVec2(ImGui::GetContentRegionAvailWidth() - Padding, ImGui::GetContentRegionAvail().y * 0.5f), true, ImGuiWindowFlags_HorizontalScrollbar);

						const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
						for (unsigned i = 0; i < NumOfButtons; i++)
						{
							if (UI::UIButton_1("Unit" + std::to_string(i), "Unit" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, { 140, 15 }, FONT_PARA))
								Tab = i;
						}

						ImGui::EndChild();
					}

					ImGui::EndChild();
					ImGui::PopStyleColor();

				}

				ImGui::SameLine();
				ImGui::Dummy({ HalfPadding, 0 });
				ImGui::SameLine();

				// Add more things ehre


				ImGui::EndChild();
			}

			ImGui::End();

		}
	}
}