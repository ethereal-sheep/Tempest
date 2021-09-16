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
				UI::SubHeader("Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f});

				const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
				// Drag drop section
				{
					const float availRegion = ImGui::GetContentRegionAvail().y * 0.3f;
					ImGui::BeginChild("##DragDropSectionSimulate", ImVec2(contentSize, availRegion), true, window_flags | ImGuiWindowFlags_NoScrollbar);

					// Attacker section
					{
						ImGui::BeginChild("##DragDropAttackerSimulate", ImVec2(contentSize * 0.5f, availRegion - 2.0f), false, window_flags | ImGuiWindowFlags_NoScrollbar);

						if (Attacker != UNDEFINED)
						{
							auto& character = instance.ecs.get<tc::Character>(Attacker);
							if (UI::UIButton_1(character.name.c_str(), character.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 200.0f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { 180, 15 }, FONT_PARA))
								Attacker = UNDEFINED;
						}

						ImGui::EndChild();

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
							{
								Attacker = *(const Entity*)payload->Data;
							}

							ImGui::EndDragDropTarget();
						}
					}
				

					ImGui::SameLine();

					// Defender section
					{
						ImGui::BeginChild("##DragDropDefenderSimulate", ImVec2(contentSize * 0.5f, availRegion - 2.0f), false, window_flags | ImGuiWindowFlags_NoScrollbar);
						if (Defender != UNDEFINED)
						{
							auto& character = instance.ecs.get<tc::Character>(Defender);
							if (UI::UIButton_1(character.name.c_str(), character.name.c_str(), { ImGui::GetCursorPosX() + 200.0f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { 180, 15 }, FONT_PARA))
								Defender = UNDEFINED;
						}
						ImGui::EndChild();


						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
							{
								Defender = *(const Entity*)payload->Data;
							}

							ImGui::EndDragDropTarget();
						}
					}
					

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
						ImGui::PushID(id);
						ImGui::BeginGroup();
						const ImVec2 pos{ cursor.x , cursor.y + i++ * 80 };

						auto& Charac = instance.ecs.get<tc::Character>(id);
						if (UI::UIButton_1(Charac.name.c_str(), Charac.name.c_str(), pos, { 180, 15 }, FONT_PARA)) {}

						ImGui::EndGroup();
				
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("UNIT_CONTENT_ITEM", &id, sizeof(Entity));
							UI::UIButton_1(Charac.name.c_str(), Charac.name.c_str(), { cursor.x , cursor.y }, { 180, 15 }, FONT_PARA);
							ImGui::EndDragDropSource();
						}

						ImGui::PopID();
				
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
					int index = 0;
					for (auto id : instance.ecs.view<tc::ActionGraph>())
					{
						auto& g = instance.ecs.get<tc::Graph>(id);
						g.g.get_name();
						if (UI::UIButton_1(g.g.get_name() + std::to_string(id), g.g.get_name() + std::to_string(id), { cursor.x , cursor.y + index * 80 }, { 180, 15 }, FONT_PARA))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<OpenActionGraphTrigger>(id);
						}
						index++;
							
					}

					/*const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
					for (unsigned i = 0; i < NumOfButtons; i++)
					{
						if (UI::UIButton_1("System" + std::to_string(i), "System" + std::to_string(i), { cursor.x , cursor.y + i * 80 }, { 180, 15 }, FONT_PARA))
							Tab = i + NumOfButtons;
					}*/

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