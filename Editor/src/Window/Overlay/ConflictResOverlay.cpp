#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "ConflictResOverlay.h"

namespace Tempest
{
	void ConflictResOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
	}
	void ConflictResOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
			window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
			if (ImGui::Begin("Conflict Resolution", &visible, window_flags))
			{
				static ImVec4 active{ 0.2f, 0.2f, 0.2f, 1.f };
				static ImVec4 inactive{ 0.062f, 0.062f, 0.062f, 1.f };
				static const ImVec2 buttonSize{ 70, 7.5 };
				const ImVec2 button2Size{ 70.f, 20.0f };
				bool border = false;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
				Tempest::UI::SubHeader("Conflict Resolutions");

				ImGui::BeginChild("##ContentSection", ImVec2(ImGui::GetContentRegionAvailWidth() * 0.85f, ImGui::GetContentRegionAvail().y), border, ImGuiWindowFlags_NoScrollWithMouse);

				const auto regoinAvailWidth = ImGui::GetContentRegionAvailWidth() / 3.0f - padding;
				const auto regoinAvailHeight = ImGui::GetContentRegionAvail().y;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - regoinAvailWidth * 3 - padding) * 0.25f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + halfPadding);

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##LeftSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), border);
					if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
					{
						bgColor = IM_COL32(0, 0, 0, 100);
						AddUnitTex = tex_map["Assets/EditUnitsUnlit.png"];
					}	
					else
					{
						bgColor = IM_COL32(20, 20, 20, 100);
						AddUnitTex = tex_map["Assets/EditUnitsLit.png"];
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
					ImGui::Image((void*)static_cast<size_t>(AddUnitTex->GetID()), ImVec2(static_cast<float>(AddUnitTex->GetWidth()), static_cast<float>(AddUnitTex->GetHeight())));

					{
						ImGui::BeginChild("ChildUnit", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y / 1.2f), border);

						const ImVec2 cursor{ ImGui::GetCursorPosX() + 180, ImGui::GetCursorPosY() + 30 };
						auto view = instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>());

						unsigned i = 0;
						for (auto id : view)
						{
							auto& Charac = instance.ecs.get<tc::Character>(id);
							if (UI::UIButton_2(Charac.name.c_str(), Charac.name.c_str(), { cursor.x , cursor.y + i++ * 80 }, button2Size, FONT_PARA))
							{
								Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false, id);								
							}
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Units", "Add Units", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, buttonSize, FONT_PARA))
					{
						Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(true);
					}

					ImGui::EndChild();
					ImGui::PopStyleColor();

				}

				ImGui::SameLine();
				ImGui::Dummy({ halfPadding, 0 });
				ImGui::SameLine();

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##MiddleSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), border);

					if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
					{
						bgColor = IM_COL32(0, 0, 0, 100);
						AddActionTex = tex_map["Assets/EditActionsUnlit.png"];
					}	
					else
					{
						bgColor = IM_COL32(20, 20, 20, 100);
						AddActionTex = tex_map["Assets/EditActionsLit.png"];
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
					ImGui::Image((void*)static_cast<size_t>(AddActionTex->GetID()), ImVec2(static_cast<float>(AddActionTex->GetWidth()), static_cast<float>(AddActionTex->GetHeight())));

					{
						ImGui::BeginChild("ChildAction", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y / 1.2f), border);

						const ImVec2 cursor{ ImGui::GetCursorPosX() + 180, ImGui::GetCursorPosY() + 30 };
						for (unsigned i = 0; i < numOfButtons; i++)
						{
							if (UI::UIButton_2("Test Action" + std::to_string(i), "Test Action" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, button2Size, FONT_PARA)) {}
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Actions", "Add Actions", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, buttonSize, FONT_PARA)) {}

					ImGui::EndChild();
					ImGui::PopStyleColor();

				}


				ImGui::SameLine();
				ImGui::Dummy({ halfPadding, 0 });
				ImGui::SameLine();

				{
					static auto bgColor = IM_COL32(0, 0, 0, 100);
					ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
					ImGui::BeginChild("##RightSide", ImVec2(regoinAvailWidth, regoinAvailHeight - padding), border);
					if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
					{
						bgColor = IM_COL32(0, 0, 0, 100);
						AddLinkTex = tex_map["Assets/EditLinksUnlit.png"];
					}
					else
					{
						bgColor = IM_COL32(20, 20, 20, 100);
						AddLinkTex = tex_map["Assets/EditLinksLit.png"];
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
					ImGui::Image((void*)static_cast<size_t>(AddLinkTex->GetID()), ImVec2(static_cast<float>(AddLinkTex->GetWidth()), static_cast<float>(AddLinkTex->GetHeight())));

					{
						ImGui::BeginChild("ChildLink", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y / 1.2f), border);

						const ImVec2 cursor{ ImGui::GetCursorPosX() + 180, ImGui::GetCursorPosY() + 30 };
						for (unsigned i = 0; i < numOfButtons; i++)
						{
							if (UI::UIButton_2("Test Link" + std::to_string(i), "Test Link" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, button2Size, FONT_PARA)) {}
						}

						ImGui::EndChild();
					}

					if (UI::UIButton_1("Add Link", "Add Links", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, buttonSize, FONT_PARA)) {}

					ImGui::EndChild();
					ImGui::PopStyleColor();
				}


				ImGui::EndChild();

				ImGui::SameLine();

				const ImVec2 buttonPos{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f,  ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.75f };
				
				

				//// other buttons 
				const ImVec2 button3Size{ 0.f, 8.0f };
				if (UI::UIButton_2("Define stats", "Define stats", buttonPos, button3Size, FONT_BODY))
				{
					Service<EventManager>::Get().instant_dispatch<DefineStatsTrigger>();
				}

				if (UI::UIButton_2("Simulate", "Simulate", { buttonPos.x,buttonPos.y + padding * 1.5f }, button3Size, FONT_BODY))
				{
					Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
				}

				if (UI::UIButton_2("Cancel", "Cancel", { buttonPos.x,buttonPos.y + padding * 3.0f}, button3Size, FONT_BODY))
				{
					OverlayOpen = false;
				}


			}

			ImGui::End();
		}
	}
}