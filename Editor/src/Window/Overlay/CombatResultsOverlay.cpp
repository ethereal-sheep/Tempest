/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "CombatResultsOverlay.h"
#include <Editor/src/InstanceManager/InstanceConfig.h>
#include <Tempest/src/Audio/AudioEngine.h>

namespace Tempest
{
	void CombatResultsOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<CombatResultsTrigger>(e);
		OverlayOpen = true;
	}

	void CombatResultsOverlay::get_results(const Event& e)
	{
		auto a = event_cast<SendCombatResults>(e);
		units_results.clear();
		units_results = a.units_results;
	}

	void CombatResultsOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		banner.update(ImGui::GetIO().DeltaTime);
		if (banner.is_finished())
			banner.start(1, 0, 10);

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.9f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
			if (ImGui::Begin("CombatResults", nullptr, window_flags))
			{
				auto bannerTex = tex_map["Assets/CombatResultBanner.dds"];
				ImVec2 min_pos = { viewport->WorkPos.x, viewport->WorkPos.y + 30.0f };
				ImVec2 max_pos = { viewport->WorkPos.x + bannerTex->GetWidth(),viewport->WorkPos.y + 30.0f + bannerTex->GetHeight() };

				ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bannerTex->GetID()), min_pos, max_pos, { banner.get(), 0 }, { 1 + banner.get(), 1 });

				// make a child!!
				const ImVec2 tableSize{ viewport->Size.x * 0.8f, viewport->Size.y * 0.7f };

				ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.5f - tableSize.x * 0.5f, viewport->Size.y * 0.55f - tableSize.y * 0.5f });
				if (ImGui::BeginChild("TableParent", tableSize, true)) 
				{
					ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 10.f, 10.f });
					if (ImGui::BeginTable("TableResults", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame))  //ImGuiTableFlags_BordersOuter
					{
						ImGui::TableSetupScrollFreeze(0, 1);

						ImGui::PushFont(FONT_SHEAD);
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.f,1.f,1.f,1.0f });

						ImGui::TableSetupColumn("Unit");
						ImGui::TableSetupColumn("Damage done");
						ImGui::TableSetupColumn("Damage received");
						ImGui::TableSetupColumn("Units defeated");
						ImGui::TableSetupColumn("Avg. attack success rate");
						ImGui::TableSetupColumn("Deaths");
						ImGui::TableHeadersRow();

						ImGui::PopFont();
						ImGui::PopStyleColor();


						ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4{ 0.254f,0.254f,0.254f, 1.f });
						ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4{ 0.168f,0.168f,0.168f, 1.f });
						unsigned i = 0;
						const float rowHeight = (tableSize.y - 30.f) / 4.0f;
						for (const auto this_unit : units_results)
						{
							ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);
							ImGui::PushID(i);
							ImGui::TableSetColumnIndex(0);

							// render character
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

							ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - character_icon->GetWidth() * 1.0f) * 0.5f, 0.0f });
							ImGui::SameLine();
							auto cs = instance.ecs.get_if<tc::Character>(this_unit.first);
							UI::UIImageButton((void*)static_cast<size_t>(character_icon->GetID()), ImVec2{ character_icon->GetWidth() * 1.0f, character_icon->GetHeight() * 1.0f },
								ImVec2{ 0,0 }, ImVec2{ 1,1 }, -1, ImVec4{ 0,0,0,0 }, ImVec4{ cs->color.x, cs->color.y,cs->color.z,1 },
								ImVec4{ cs->color.x, cs->color.y,cs->color.z,1 }, ImVec4{ cs->color.x, cs->color.y,cs->color.z,1 });

							if (cs)
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.f,1.f,1.f,1.0f });
								ImGui::PushFont(FONT_BODY);
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(cs->name.c_str()).x) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text(cs->name.c_str());
								ImGui::PopFont();
								ImGui::PopStyleColor();
							}

							ImGui::PopStyleColor(3);
							ImGui::PushFont(FONT_SHEAD);

							std::string text{ "" };
							ImVec2 textSize{};

							// damage done
							if (ImGui::TableNextColumn())
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.f,1.f,0.f,1.0f });
								text = std::to_string(this_unit.second.dmg_done);
								textSize = ImGui::CalcTextSize(text.c_str());
								ImGui::Dummy(ImVec2{ 0.f, (rowHeight - textSize.y * 2.0f) * 0.5f });
								ImGui::Text("");
								ImGui::SameLine();
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - textSize.x * 2.0f) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text(text.c_str());
								ImGui::PopStyleColor();
							}

							// damage received
							if (ImGui::TableNextColumn())
							{
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.f,0.f,0.f,1.0f });
								text = std::to_string(this_unit.second.dmg_taken);
								textSize = ImGui::CalcTextSize(text.c_str());
								ImGui::Dummy(ImVec2{ 0.f, (rowHeight - textSize.y * 2.0f) * 0.5f });
								ImGui::Text("");
								ImGui::SameLine();
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - textSize.x * 2.0f) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text(text.c_str());
								ImGui::PopStyleColor();
							}

							// units defeated
							if (ImGui::TableNextColumn())
							{
								text = std::to_string(this_unit.second.defeated);
								textSize = ImGui::CalcTextSize(text.c_str());
								ImGui::Dummy(ImVec2{ 0.f, (rowHeight - textSize.y * 2.0f) * 0.5f });
								ImGui::Text("");
								ImGui::SameLine();
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - textSize.x * 2.0f) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text(text.c_str());
							}

							// success rate
							if (ImGui::TableNextColumn())
							{
								char buffer[7];
								float rate{ 0.f };
								if (this_unit.second.total_attacks != 0)
									rate = static_cast<float>(this_unit.second.successful_attacks) / static_cast<float>(this_unit.second.total_attacks) * 100.f;

								sprintf(buffer, "%.1f%%", rate);
								text = std::string{ buffer };
								textSize = ImGui::CalcTextSize(text.c_str());
								ImGui::Dummy(ImVec2{ 0.f, (rowHeight - textSize.y * 2.0f) * 0.5f });
								ImGui::Text("");
								ImGui::SameLine();
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - textSize.x * 2.0f) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text("%.1f%%", rate);
							}

							// deaths
							if (ImGui::TableNextColumn())
							{
								text = std::to_string(this_unit.second.deaths);
								textSize = ImGui::CalcTextSize(text.c_str());
								ImGui::Dummy(ImVec2{ 0.f, (rowHeight - textSize.y * 2.0f) * 0.5f });
								ImGui::Text("");
								ImGui::SameLine();
								ImGui::Dummy(ImVec2{ (ImGui::GetContentRegionAvail().x - textSize.x * 2.0f) * 0.5f, 0.f });
								ImGui::SameLine();
								ImGui::Text(text.c_str());
							}

							ImGui::PopFont();
							ImGui::PopID();
						}

						ImGui::PopStyleColor(2);

					}
					ImGui::EndTable();
					ImGui::PopStyleVar();
				}
				
				ImGui::EndChild();

				if (UI::UIButton_2("Quit", "Quit", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.1f }, { 50,20 }, FONT_BTN))
				{
					AudioEngine ae;
					ae.StopAllChannels();
					OverlayOpen = false;

					InstanceConfig config{ instance.get_full_path(),
						MemoryStrategy{},
						InstanceType::EDIT_TIME };
					config.enable_tutorial = instance.tutorial_enable;

					Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(config);

					ae.Play("Sounds2D/CoReSyS_BGM1.wav", "BGM", 0.7f, true);
					Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>();
				}
			}
			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (!ImGui::GetHoveredID())
				HoveredID = 0;
			else if (HoveredID != ImGui::GetHoveredID())
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Highlight.wav", "SFX", 0.8f);
				HoveredID = ImGui::GetHoveredID();
			}
		}
	}
}