#pragma once
#include "Instance/Instance.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

namespace Tempest
{
	class ViewportWindow : public Window
	{
		const char* window_name() override
		{
			return "Viewport";
		}

		void init() override
		{
			window_flags |=
				//ImGuiWindowFlags_NoCollapse |
				//ImGuiWindowFlags_NoResize |
				//ImGuiWindowFlags_NoMove |
				//ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoDecoration |
				//ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoSavedSettings;
		}

		void show(Instance&) override
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Main", nullptr, window_flags))
			{
				ImGui::PopStyleVar(3);
				//ImGui::Image(ImGui::GetIO().Fonts->TexID, {1600, 900}, { 0,1 }, { 1,0 });
			}
			ImGui::End();
		}
	};
}