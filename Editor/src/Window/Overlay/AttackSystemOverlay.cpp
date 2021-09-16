#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "AttackSystemOverlay.h"
#include "Util/UIElements.h"

namespace Tempest
{
	void AttackSystemOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		auto a = event_cast<OpenActionGraphTrigger>(e);
		id = a.id;
	}
	void AttackSystemOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
			if (ImGui::Begin("Attack System", nullptr, window_flags))
			{
				auto& g = instance.ecs.get<tc::Graph>(id);
				ImGui::PushFont(FONT_HEAD);
				ImGui::Text(g.g.get_name().c_str());
				ImGui::PopFont();
				ImGui::SameLine();
				UI::Header_1("Attack System");

				ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });
				draw_context(instance);
				ImGui::PopStyleColor();


				if (UI::UIButton_1("Save & Close", "Save & Close", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
				{
					OverlayOpen = false;
					Service<EventManager>::Get().instant_dispatch<OpenSimulateTrigger>();
				}

				ImGui::SameLine();
				if (UI::UIButton_1("Navigate to Content", "Navigate to Content", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
				{
					ax::NodeEditor::NavigateToContent();
				}
			}
			ImGui::End();
		}
	}
}