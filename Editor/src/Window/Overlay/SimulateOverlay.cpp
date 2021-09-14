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
				float center_x = ImGui::GetContentRegionAvailWidth() * 0.5f;
				const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.85f;
				string text = "Simulate";
				float text_center = center_x - (ImGui::CalcTextSize(text.c_str()).x * 0.5f);

				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader(ImVec2{ text_center - 100.f,50 }, "Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f});

				const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
				// Drag drop section
				{
					ImGui::BeginChild("##DragDropSectionSimulate", ImVec2(contentSize, ImGui::GetContentRegionAvail().y * 0.3f), true, ImGuiWindowFlags_NoScrollWithMouse);

					// VS word (please bring y-axis down to centre
					center_x = ImGui::GetContentRegionAvailWidth() * 0.5f;
					text = "VS";
					text_center = center_x - (ImGui::CalcTextSize(text.c_str()).x * 0.5f);
					ImGui::Dummy(ImVec2{ text_center, 0.f });
					ImGui::SameLine();
					ImGui::PushFont(FONT_SHEAD);
					ImGui::Text(text.c_str());
					ImGui::PopFont();

				//	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() / 3.0f);



					ImGui::EndChild();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
				// Content selection section
				ImGui::BeginChild("##ContentSectionSimulate", ImVec2(contentSize, ImGui::GetContentRegionAvail().y * 0.8f), true, ImGuiWindowFlags_NoScrollWithMouse);
				
				const auto regoinAvailWidth = ImGui::GetContentRegionAvailWidth() / 3.0f - Padding;
				const auto regoinAvailHeight = ImGui::GetContentRegionAvail().y;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - regoinAvailWidth * 3 - Padding) * 0.4f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + HalfPadding);

				{
					ImGui::BeginChild("##LeftSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
					auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

					unsigned i = 0;
					for (auto id : view)
					{
						auto& Charac = instance.ecs.get<tc::Character>(id);
						if (UI::UIButton_1(Charac.name.c_str(), Charac.name.c_str(), { cursor.x , cursor.y + i++ * 80 }, { 180, 15 }, FONT_PARA))
							Tab = i;
					}

					ImGui::EndChild();

				}

				ImGui::SameLine();
				ImGui::Dummy({ HalfPadding, 0 });
				ImGui::SameLine();

				// Add more things ehre
				{
					ImGui::BeginChild("##MiddleSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));
			
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
					for (unsigned i = 0; i < NumOfButtons; i++)
					{
						if (UI::UIButton_1("System" + std::to_string(i), "System" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, { 180, 15 }, FONT_PARA))
							Tab = i + NumOfButtons;
					}

					ImGui::EndChild();

				}

				ImGui::SameLine();
				ImGui::Dummy({ HalfPadding, 0 });
				ImGui::SameLine();

				{
					ImGui::BeginChild("##RightSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));

					const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
					for (unsigned i = 0; i < NumOfButtons; i++)
					{
						if (UI::UIButton_1("Link" + std::to_string(i), "Link" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, { 180, 15 }, FONT_PARA))
							Tab = i + NumOfButtons * 2;
					}
					ImGui::EndChild();
				}

				ImGui::EndChild();

				// Cancel button
				if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 100.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
				{
					OverlayOpen = false;
				}

				// Simulate button
				if (UI::UIButton_1("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 350.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
				{
					OverlayOpen = false;
				}
			}

			ImGui::End();

		}
	}
}