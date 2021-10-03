#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "Triggers/SimulationTriggers.h"

namespace Tempest
{
	void SimulateOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		Entity Attacker = UNDEFINED;
		Entity Defender = UNDEFINED;
		Entity ActionID = UNDEFINED;
		Entity LinkID = UNDEFINED;
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
				//float center_x = ImGui::GetContentRegionAvailWidth() * 0.5f;
				const float contentSize = ImGui::GetContentRegionAvailWidth() * 0.9f;
				string text = "Simulate";
				//float text_center = center_x - (ImGui::CalcTextSize(text.c_str()).x * 0.5f);

				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f});

				const float offsetX = (ImGui::GetContentRegionAvailWidth() - contentSize) * 0.5f;

				auto tex = tex_map["Assets/SimulationBackdrop.png"];
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
				ImVec2 point = ImGui::GetCursorPos();
				{

					ImVec2 Min{ point.x, point.y };
					ImVec2 Max{ Min.x + viewport->Size.x * 0.91f, Min.y + viewport->Size.y * 0.73f };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// Drag drop section
				{
					const float availRegion = ImGui::GetContentRegionAvail().y * 0.3f;
					auto curr_tex = tex_map["Assets/ButtonSlot.png"];
					const ImVec2 button_slot_size{ curr_tex->GetWidth() * 0.9f, curr_tex->GetHeight() * 0.9f };

					ImGui::BeginChild("##DragDropSectionSimulate", ImVec2(contentSize, availRegion), true, window_flags | ImGuiWindowFlags_NoScrollbar);

					// Attacker section
					{
						ImGui::BeginChild("##DragDropAttackerSimulate", ImVec2(contentSize * 0.5f, availRegion - 2.0f), true, window_flags );

						// Attacker image
						ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.05f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.11f });
						ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Attacker.png"]->GetID()), {tex_map["Assets/Attacker.png"]->GetWidth() * 0.9f, tex_map["Assets/Attacker.png"]->GetHeight() * 0.9f });

						ImGui::SameLine();

						// Button slot
						ImVec2 buttonPos{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.45f - button_slot_size.x * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f - button_slot_size.y * 0.5f};
						
						ImGui::SetCursorPos(buttonPos);
						ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), button_slot_size); // Image draws from top left
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
							{
								Attacker = *(const Entity*)payload->Data;
							}

							ImGui::EndDragDropTarget();
						}

						if (Attacker != UNDEFINED)
						{
							auto& character = instance.ecs.get<tc::Character>(Attacker);
							if (UI::UIButton_1(character.name.c_str(), character.name.c_str(), ImVec2{ buttonPos.x + button_slot_size.x * 0.5f, buttonPos.y + button_slot_size.y * 0.5f}, { 180, 15 }, FONT_PARA))
								Attacker = UNDEFINED;
						}

						ImGui::EndChild();
					
					}
				
					ImGui::SameLine(0,0);

					// Defender section
					{
						ImGui::BeginChild("##DragDropDefenderSimulate", ImVec2(contentSize * 0.5f, availRegion - 2.0f), true, window_flags );

						ImGui::SameLine();

						// Button slot
						ImVec2 buttonPos{ ImGui::GetCursorPosX() - 20.0f * 0.5f + ImGui::GetContentRegionAvailWidth() * 0.35f - button_slot_size.x * 0.5f,
										  ImGui::GetCursorPosY() + 15.0f * 0.5f + ImGui::GetContentRegionAvail().y * 0.5f - button_slot_size.y * 0.5f};

						ImGui::SetCursorPos(buttonPos);
						ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), button_slot_size); // Image draws from top left
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("UNIT_CONTENT_ITEM"))
							{
								Defender = *(const Entity*)payload->Data;
							}

							ImGui::EndDragDropTarget();
						}

						ImGui::SameLine();

						// Defender image
						ImGui::SetCursorPos({ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.56f - (tex_map["Assets/Defender.png"]->GetWidth() * 0.9f) * 0.5f , ImGui::GetCursorPosY() - (tex_map["Assets/Defender.png"]->GetHeight() * 0.9f) * 0.5f + 15.0f});
						ImGui::Image((void*)static_cast<size_t>(tex_map["Assets/Defender.png"]->GetID()), { tex_map["Assets/Defender.png"]->GetWidth() * 0.9f, tex_map["Assets/Defender.png"]->GetHeight() * 0.9f });

						if (Defender != UNDEFINED)
						{
							auto& character = instance.ecs.get<tc::Character>(Defender);
							if (UI::UIButton_1(character.name.c_str(), character.name.c_str(), ImVec2{ buttonPos.x + button_slot_size.x * 0.5f, buttonPos.y + button_slot_size.y * 0.5f }, { 180, 15 }, FONT_PARA))
								Defender = UNDEFINED;
						}
						ImGui::EndChild();
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
						bool selected = false;
						if (id == ActionID)
							selected = true;
						if (UI::UIButton_2(g.g.get_name() + std::to_string(id), g.g.get_name() + std::to_string(id), { cursor.x , cursor.y + index * 80 }, { 70.f, 15.f }, FONT_PARA, selected))
						{
							//OverlayOpen = false;
							//Service<EventManager>::Get().instant_dispatch<OpenActionGraphTrigger>(id, instance);
							
							if (selected == true)
								ActionID = UNDEFINED;
							else
								ActionID = id;

						}
						index++;
					}

					ImGui::EndChild();

				}

				ImGui::SameLine();
				ImGui::Dummy({ HalfPadding, 0 });
				ImGui::SameLine();

				{
					ImGui::BeginChild("##RightSideSimulate", ImVec2(regoinAvailWidth, regoinAvailHeight - Padding));

					const ImVec2 cursor{ ImGui::GetCursorPosX() + 120, ImGui::GetCursorPosY() + 30 };
					int index = 0;
					for (auto id : instance.ecs.view<tc::ConflictGraph>())
					{
						auto& g = instance.ecs.get<tc::Graph>(id);
						bool selected = false;
						if (id == LinkID)
							selected = true;
						g.g.get_name();
						if (UI::UIButton_2(g.g.get_name() + std::to_string(id), g.g.get_name() + std::to_string(id), { cursor.x + 10.f , cursor.y + index * 80 }, { 70.f, 15.f}, FONT_PARA, selected))
						{
							// make sure i can simulate
							// 1. have atk and def 
							// 2. atk and def have characters
							// 3. have conflict and conflict graph
							

							if (selected == true)
								LinkID = UNDEFINED;
							else
								LinkID = id;
							if (Defender != INVALID &&
								Attacker != INVALID &&
								instance.ecs.has<tc::Character>(Defender) &&
								instance.ecs.has<tc::Character>(Attacker)
							)
							{
								
								//Service<EventManager>::Get().instant_dispatch<OpenSimulateResultTrigger>(Attacker, Defender, id);
								//OverlayOpen = false;
							}

						}
						index++;
					}
					ImGui::EndChild();
				}

				ImGui::EndChild();

				// Cancel button
				

				// Simulate button
				bool disable = false;
				if (ActionID == UNDEFINED || LinkID == UNDEFINED)
					disable = true;
				
				if (disable)
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				if (UI::UIButton_2("Simulate", "Simulate", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
				{
					//Service<EventManager>::Get().instant_dispatch<SimulationTrigger>();
					//OverlayOpen = false;
				}
				if (UI::UIButton_2("Custom Map", "Custom Map", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.75f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
				{
					//Service<EventManager>::Get().instant_dispatch<SimulationTrigger>();
					//OverlayOpen = false;
				}
				if (disable)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
				if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.895f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 10.f, 10.f }, FONT_PARA))
				{
					OverlayOpen = false;
				}
			}

			ImGui::PopStyleVar();

			ImGui::End();

		}
	}
}