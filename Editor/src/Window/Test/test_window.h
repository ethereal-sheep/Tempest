#pragma once
#include "Instance/Instance.h"
#include "Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	class test_window : public Window
	{
		const char* window_name() override
		{
			return "test_window";
		}

		bool button(
			const string& default_text,
			const string& hover_text,
			ImVec2 pos,
			float padding_x,
			float padding_y,
			float rounding
		) 
		{

		}


		void show(Instance& instance) override
		{
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

				}


				//ImPlot::SetNextPlotLimits(0, dt.count() / defaultDivisor, 0, 1.0 * maxRow);


				{

					static int  bins = 50;
					static bool cumulative = false;
					static bool density = true;
					static bool outliers = true;
					static double mu = 5;
					static double sigma = 2;

					ImGui::SetNextItemWidth(200);
					if (ImGui::RadioButton("Sqrt", bins == ImPlotBin_Sqrt)) { bins = ImPlotBin_Sqrt; } ImGui::SameLine();
					if (ImGui::RadioButton("Sturges", bins == ImPlotBin_Sturges)) { bins = ImPlotBin_Sturges; } ImGui::SameLine();
					if (ImGui::RadioButton("Rice", bins == ImPlotBin_Rice)) { bins = ImPlotBin_Rice; } ImGui::SameLine();
					if (ImGui::RadioButton("Scott", bins == ImPlotBin_Scott)) { bins = ImPlotBin_Scott; } ImGui::SameLine();
					if (ImGui::RadioButton("N Bins", bins >= 0))                       bins = 50;
					if (bins >= 0) {
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						ImGui::SliderInt("##Bins", &bins, 1, 100);
					}
					if (ImGui::Checkbox("Density", &density))
						ImPlot::FitNextPlotAxes();
					ImGui::SameLine();
					if (ImGui::Checkbox("Cumulative", &cumulative))
						ImPlot::FitNextPlotAxes();
					ImGui::SameLine();
					static bool range = false;
					ImGui::Checkbox("Range", &range);
					static float rmin = -3;
					static float rmax = 13;
					if (range) {
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						ImGui::DragFloat2("##Range", &rmin, 0.1f, -3, 13);
						ImGui::SameLine();
						ImGui::Checkbox("Outliers", &outliers);
					}

					// example 2d6
					static tvector<int> dist(20, 0);

					if (ImGui::Button("Generate"))
					{
						for (auto& i : dist) i = 0;

						auto d6 = []() {
							return els::random::uniform_rand(1, 6);
						};

						for (int i = 0; i < 1000; ++i)
						{
							++dist[d6() + d6()];
						}

					}




					if (ImPlot::BeginPlot("##Histograms")) {
						ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
						ImPlot::PlotBars("Test", dist.data(), 20);
						ImPlot::EndPlot();
					}
				}

			}

			ImGui::End();
		}
	};
}