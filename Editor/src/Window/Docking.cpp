#include "Docking.h"

#include "imgui/imgui.h"

namespace UI
{
	void DockingWindow::Show() const
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			[[maybe_unused]]ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			//ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);
		}
	}
}

