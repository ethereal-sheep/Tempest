/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "RNG/dice_bag.h"
#include "RNG/dice_distribution.h"

namespace Tempest
{
	class test_window : public Window
	{
		const char* window_name() override
		{
			return "test_window";
		}
		std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);
		interpolater<float> inter{};
		/*bool button(
			const string& default_text,
			const string& hover_text,
			ImVec2 pos,
			float padding_x,
			float padding_y,
			float rounding
		) 
		{

		}*/

		void show(Instance& instance [[maybe_unused]] ) override
		{
			{
				float dt = ImGui::GetIO().DeltaTime;
				for (auto& i : inter_nest)
					i.update(dt);

				inter.update(dt);
			}

			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				string unselected = "Unselected";
				string hover = "Hover";

				const float default_padding_x = 8.f;
				const float default_padding_y = 8.f;
				const float border_size = 1.5f;

				const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
				const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
				const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };

				static float padding_x = 50.f;
				static float padding_y = 50.f;
				static float rounding = 0.f;
				static ImVec2 pos = { 200,500 };
				
				//UI::Header(ImVec2{ center_x - 100.f,0 }, "HEADER");
				UI::SubHeader("SUBHEADER");
				static bool test;
				UI::UICheckBox_1("TS", &test);
				/*if(ImGui::Button("COMBAT MODE"))
				{
					Service<EventManager>::Get().instant_dispatch<OpenCombatModeTrigger>(instance);
				}*/

				if (ImGui::Button("CONFLICT RES"))
				{
					Service<EventManager>::Get().instant_dispatch<OpenConflictResTrigger>();
				}
				if (ImGui::Button("DEFINE STATS"))
				{
					Service<EventManager>::Get().instant_dispatch<DefineStatsTrigger>();
				}
				if (ImGui::Button("MAIN MENU"))
				{
					Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>();
				}
				if (ImGui::Button("BUILD MODE"))
				{
					Service<EventManager>::Get().instant_dispatch<OpenBuildModeOverlay>(instance);
				}

				// funky stuff put here first cuz laze, mirgrating it out to somewhere else
				// i'll just assume that it's max 2 numbers
				static bool startAnim = false;
				static int randNum = 0;
				static string numToString = "";
				if (ImGui::Button("ANIMATE NUM") && !startAnim)
				{
					startAnim = true;
					randNum = std::rand() % 100;
					numToString = std::to_string(randNum);
					inter_nest[0].start(2.0f, 1.0f, .5f, 0.f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					inter_nest[1].start(2.0f, 1.0f, .5f, 0.2f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					inter_nest[2].start(0.5f, 2.f, .15f, 0.0f,
						[](float x) { return glm::backEaseInOut(x);
					});
				}

				ImGui::SameLine();
				if (ImGui::BeginChild("##SomeWeirdTest", ImVec2{ 300.0f,200.0f }, true))
				{
					const ImVec2 renderPos{ ImGui::GetCursorPos() + ImVec2{ 150.0f, 100.0f } };
					FONT_HEAD->Scale = 2.0f;
					ImGui::PushFont(FONT_HEAD);
					const ImVec2 textSize{ImGui::CalcTextSize(numToString.c_str())};
					ImGui::PopFont();

					// draw the image bg
					// maybe have some tint color
					auto tex = tex_map["Assets/Selected.dds"];
					ImGui::SetCursorPos(ImVec2{ renderPos.x - textSize.x * inter_nest[2].get() * 0.6f, renderPos.y - textSize.y * (inter_nest[2].get() * 0.5f) * 0.7f});
					ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2{ textSize.x * inter_nest[2].get(),textSize.y * (inter_nest[2].get() *0.5f) },
						ImVec2{ 0,0 }, ImVec2{ 1,1 }, ImVec4{1.0f, 0.64f, 0.0f, 1.0f});

					// render the funky text
					ImGui::SetCursorPos(ImVec2{ renderPos.x - textSize.x * 0.5f, renderPos.y - textSize.y * 0.5f });
					for (int i = 0; i < numToString.size(); i++)
					{
						FONT_HEAD->Scale = inter_nest[i].get();
						ImGui::PushFont(FONT_HEAD);
						ImGui::Text(std::string{ numToString[i] }.c_str());
						ImGui::PopFont();
						ImGui::SameLine();
					}

					// reset animation
					if (!numToString.empty() && inter_nest[numToString.size() - 1].is_finished())
						startAnim = false;
				}
				ImGui::EndChild();
				FONT_HEAD->Scale = 1.0f; // reset to original scale


				UI::UISelectable("TEST", false);
				
				
			
				// sliders for adjustment
				ImVec2 slider_padding = { 100,0 };
				UI::DragFloat("Padding X", "123", slider_padding, &padding_x);
				UI::DragFloat("Padding y", "321", slider_padding, &padding_y);
				UI::DragFloat("Rounding", "2341", slider_padding, &rounding);
				UI::DragFloat2("Pos ", "5466", slider_padding, &pos.x);

				// button shit
				ImGui::PushFont(FONT_BTN);
				ImVec2 text_size = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
				ImVec2 alt_text_size = ImGui::CalcTextSize(hover.c_str(), nullptr, true);
				ImVec2 act_text_size = {
					std::max(text_size.x, alt_text_size.x),
					std::max(text_size.y, alt_text_size.y)
				};
				ImGui::PopFont();

				ImVec2 button_size = {
					act_text_size.x + default_padding_x + padding_x,
					act_text_size.y + default_padding_y + padding_y
				};

				auto old_pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(pos);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
				bool clicked = ImGui::InvisibleButton("##NiceButton", button_size);
				ImGui::PopStyleVar(1);
				ImGui::SetCursorPos(pos);

				if (!ImGui::IsItemHovered())
				{
					// default
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
					ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
					ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
					ImGui::Button("##NiceButton_Dummy", button_size);
					ImGui::PopStyleVar(2);
					ImGui::PopStyleColor(4);
					ImGui::SetCursorPos(
						{
							pos.x + button_size.x / 2.f - text_size.x / 2.f,
							pos.y + button_size.y / 2.f - text_size.y / 2.f
						});
					ImGui::PushFont(FONT_BTN);
					ImGui::Text(unselected.c_str());
					ImGui::PopFont();
				}
				else
				{
					// hovered
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
					ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
					ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
					ImGui::Button("##NiceButton_Dummy", button_size);
					ImGui::PopStyleVar(2);
					ImGui::PopStyleColor(4);

					ImGui::SetCursorPos(
						{
							pos.x + button_size.x / 2.f - alt_text_size.x / 2.f,
							pos.y + button_size.y / 2.f - alt_text_size.y / 2.f
						});
					ImGui::PushFont(FONT_BTN);
					ImGui::Text(hover.c_str());
					ImGui::PopFont();
				}

				ImGui::SetCursorPos(old_pos);

				ImGui::Button("Default Button Style");

				if (clicked)
				{
					LOG("TRUE");
					Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>(instance);
				}


				//ImPlot::SetNextPlotLimits(0, dt.count() / defaultDivisor, 0, 1.0 * maxRow);


				{
					// example 2d6
					static dice_bag bag;

					auto dice_input = [](auto d) {
						string label = "D";
						label += std::to_string(d);
						auto count = bag.count(d);
						int uone = 1;
						if (ImGui::InputScalar(label.c_str(), ImGuiDataType_U64, &count, &uone))
						{
							if (count < bag.count(d))
								bag.remove(1, d);
							else
								bag.add(1, d);
						}

					};

					uint64_t uone = 1u;
					static uint64_t freq = 3000u;
					static uint64_t difficulty = 0u;

					ImGui::InputScalar("Frequency", ImGuiDataType_U64, &freq);
					ImGui::InputScalar("Difficulty", ImGuiDataType_U64, &difficulty, &uone);


					//dice_input(2);
					dice_input(4);
					dice_input(6);
					dice_input(8);
					dice_input(10);
					dice_input(12);
					dice_input(20);
					//dice_input(100);


					static size_t output = 0;
					auto expected = bag.expected();

					if (ImGui::Button("Roll"))
					{
						output = bag();
					}

					ImGui::Text("Roll:      %u", output);
					ImGui::Text("Expected:  %.1f", expected);


					static auto win = 0;
					static auto lose = 0;
					static float chance = 0.f;
					static size_t max_bucket = 0;
					static tvector<size_t> dist;
					static tvector<double> dist2;
					static double max_pro = 0.0;

					if (ImGui::Button("Generate"))
					{
						win = 0;
						lose = 0;
						max_bucket = 0;
						auto dd = bag.get_distribution();
						dist2 = dd.get_probability_win();


						max_pro = dd.get_max_probability();
						dist.clear();
						dist.resize(bag.spread() + 2, 0);
						for (int i = 0; i < freq; ++i)
						{
							auto val = bag();

							max_bucket = std::max(++dist[val - bag.min()+1], max_bucket);

							if (val >= difficulty) ++win; else ++lose;
						}
					}

					ImGui::Text("Win:   %d", win);
					ImGui::Text("Lose:  %d", lose);


					chance = (difficulty <= bag.min()) ? 100.f : ((difficulty > bag.max() ? 0.f : static_cast<float>(dist2[difficulty - bag.min()] * 100)));

					ImGui::Text("Chance:  %.3f%", chance);


					//static const char* labels[] = { "1","2","3","4","5","6","7","8","9","10","11","12","13" };
					//static const double positions[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };


					ImPlot::SetNextPlotLimits(bag.min() - 1., bag.max() + 1., 0., 1., ImGuiCond_Always);
					
					if (ImPlot::BeginPlot("##Line Plot", "", "", ImVec2{ -1, 0 },
						ImPlotFlags_NoLegend | ImPlotFlags_NoMousePos, ImPlotAxisFlags_None,
						ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_LockMax |
						ImPlotAxisFlags_LockMin))
					{
						ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
						ImPlot::PlotLine("", dist2.data(), (int)dist2.size(), 1.0, (int)bag.min());
						ImPlot::EndPlot();
					}

					ImPlot::SetNextPlotLimits(bag.min()-1., bag.max()+1., 0., static_cast<double>(std::max<size_t>(5u, max_bucket*3/2)), ImGuiCond_Always);

					if (ImPlot::BeginPlot("##Histograms", "", "", ImVec2{ -1, 0 }, 
						ImPlotFlags_NoLegend | ImPlotFlags_NoMousePos, ImPlotAxisFlags_None,
						ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_LockMax |
						ImPlotAxisFlags_LockMin | ImPlotAxisFlags_NoDecorations))
					{
						ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, .5f);
						ImPlot::PlotBars("", dist.data(), static_cast<int>(dist.size() + bag.min()), 0.70, 0.0, 0 - int(bag.min() - 1));
						
						auto [dx, dy] = ImPlot::GetPlotMousePos();

						int x = (int)std::round(dx);
						int y = (int)std::round(dy);

						if (ImPlot::IsPlotHovered())
						{
							if (abs(x - dx) <= 0.35 && x >= bag.min() && x < bag.min() + dist.size() && y <= dist[x-bag.min()+1])
							{
								ImGui::BeginTooltip();
								//ImGui::PushStyleColor(ImGuiCol_Text, ConvertUint32TOVec4(profile.GetZoneColor(bar.key->m_Zone)));
								ImGui::Text(ICON_FA_MARKER);
								ImGui::SameLine();
								ImGui::Text("%d, %d", x, dist[x - bag.min()+1]);

								//ImGui::Text("Call time: %f%s", (bar.duration - bar.start) / 1000000000.f, "ms");

								ImGui::EndTooltip();

							}
						}


						ImPlot::PlotVLines("Expected", &expected, 1);
						ImPlot::EndPlot();
					}
				}

			}

			ImGui::End();
		}
	};
}