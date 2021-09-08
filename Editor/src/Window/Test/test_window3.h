#pragma once
#include "Instance/Instance.h"
#include "Graphics/OpenGL/RenderSystem.h"
#include "Util/UIElements.h"

namespace Tempest
{
	class test_window3 : public Window
	{
		unsigned tab = 0;
		const unsigned numOfButtons = 10;
		const float padding = 50.0f;
		const float halfPadding = padding * 0.5f;

		const char* window_name() override
		{
			return "test_window3";
		}

		void init(Instance&) override
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			style->FramePadding = ImVec2(8, 6);
			style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonActive] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonHovered] = ImColor(30, 30, 30, 255);
		}

		void show(Instance& instance [[maybe_unused]] ) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				static ImVec4 active{ 0.2f, 0.2f, 0.2f, 1.f };
				static ImVec4 inactive{ 0.062f, 0.062f, 0.062f, 1.f };
				static const ImVec2 buttonSize{ 70, 7.5 };

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
				Tempest::UI::SubHeader({ ImGui::GetWindowWidth() / 3.f , padding }, "Conflict Resolutions");

				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar;
				windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

				ImGui::BeginChild("##ContentSection", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y), true, ImGuiWindowFlags_NoScrollWithMouse);

				const auto regoinAvailWidth = ImGui::GetContentRegionAvailWidth() / 3.0f - padding;
				const auto regoinAvailHeight = ImGui::GetContentRegionAvail().y;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - regoinAvailWidth * 3 - padding) * 0.25f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + halfPadding);

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##LeftSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), true, windowFlags);
					if (!ImGui::IsWindowHovered())
						bgColor = IM_COL32(0, 0, 0, 100);
					else
						bgColor = IM_COL32(20, 20, 20, 100);
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + regoinAvailHeight / 3.0f);

						ImGui::BeginChild("ChildUnit", ImVec2(ImGui::GetContentRegionAvailWidth() - padding, ImGui::GetContentRegionAvail().y / 1.2f), true, ImGuiWindowFlags_HorizontalScrollbar);
						
						const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30};
						for (unsigned i = 0; i < numOfButtons; i++)
						{
							if (UI::UIButton_1("Test Unit" + std::to_string(i), "Test Unit" + std::to_string(i), { cursor.x , cursor.y + i * 80}, { 140, 15 }, FONT_PARA))
								tab = i;
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Units", "Add Units", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + halfPadding }, buttonSize, FONT_PARA)) {}

					ImGui::EndChild();
					ImGui::PopStyleColor();
				
				}
				
				ImGui::SameLine();
				ImGui::Dummy({ halfPadding, 0 });
				ImGui::SameLine();

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##MiddleSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), true, windowFlags);
					if (!ImGui::IsWindowHovered())
						bgColor = IM_COL32(0, 0, 0, 100);
					else
						bgColor = IM_COL32(20, 20, 20, 100);
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + regoinAvailHeight / 3.0f);

						ImGui::BeginChild("ChildAction", ImVec2(ImGui::GetContentRegionAvailWidth() - padding, ImGui::GetContentRegionAvail().y / 1.2f), true, ImGuiWindowFlags_HorizontalScrollbar);
						
						const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30};
						for (unsigned i = 0; i < numOfButtons; i++)
						{
							if (UI::UIButton_1("Test Action" + std::to_string(i), "Test Action" + std::to_string(i), { cursor.x , cursor.y + i * 80}, { 140, 15 }, FONT_PARA))
								tab = i + numOfButtons;
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Actions", "Add Actions", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + halfPadding }, buttonSize, FONT_PARA)) {}
					
					ImGui::EndChild();
					ImGui::PopStyleColor();
					
				}
				

				ImGui::SameLine();
				ImGui::Dummy({ halfPadding, 0 });
				ImGui::SameLine();

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##RightSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), true, windowFlags);
					if (!ImGui::IsWindowHovered())
						bgColor = IM_COL32(0, 0, 0, 100);
					else
						bgColor = IM_COL32(20, 20, 20, 100);
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + regoinAvailHeight / 3.0f);

						ImGui::BeginChild("ChildLink", ImVec2(ImGui::GetContentRegionAvailWidth() - padding, ImGui::GetContentRegionAvail().y / 1.2f), true, ImGuiWindowFlags_HorizontalScrollbar);

						const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
						for (unsigned i = 0; i < numOfButtons; i++)
						{
							if (UI::UIButton_1("Test Link" + std::to_string(i), "Test Link" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, { 140, 15 }, FONT_PARA))
								tab = i + numOfButtons * 2;
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Link", "Add Links", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + halfPadding }, buttonSize, FONT_PARA)) {}
					
					ImGui::EndChild();
					ImGui::PopStyleColor();
				}
				

				ImGui::EndChild();
			}

			ImGui::End();
	
		}
	};
}