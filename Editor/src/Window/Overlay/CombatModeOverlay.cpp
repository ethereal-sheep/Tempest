#include "CombatModeOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	void CombatModeOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
	}

	void CombatModeOverlay::show(Instance& instance)
	{
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f,0.0f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
			{

				if (ImGui::Button("X"))
					OverlayOpen = false;


                
                auto drawlist = ImGui::GetWindowDrawList();
                //auto jankImg = Service<RenderSystem>::Get().jank2->GetID();
				//GLuint tex_id = static_cast<GLuint>(jankImg);
                
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + viewport->Size.y * 0.95f);
				ImVec2 point = ImGui::GetCursorScreenPos();
                // Setting character info bg
                //drawlist->AddImage((void*)static_cast<size_t>(tex_id), ImVec2(point.x - viewport->Size.x * 0.2f, point.y - viewport->Size.y * 0.2f), ImVec2(point.x + viewport->Size.x * 0.4f, point.y ));
                //ImGui::Image((void*)static_cast<size_t>(tex_id), ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.25f));

				// Setting bootiful elmo
                //jankImg = Service<RenderSystem>::Get().tex->GetID();
				//tex_id = static_cast<GLuint>(jankImg);
				
				//drawlist->AddImage((void*)static_cast<size_t>(tex_id), ImVec2(point.x, point.y - viewport->Size.y * 0.4f), ImVec2(point.x + viewport->Size.x * 0.2f, point.y));

				// Shift position
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + viewport->Size.x * 0.65f);
				point = ImGui::GetCursorPos();

				// Setting actions info bg
				//jankImg = Service<RenderSystem>::Get().jank1->GetID();
				//tex_id = static_cast<GLuint>(jankImg);

				//drawlist->AddImage((void*)static_cast<size_t>(tex_id), ImVec2(point.x, point.y - viewport->Size.y * 0.2f), ImVec2(point.x + viewport->Size.x * 0.4f, point.y));
			}

            ImGui::End();

            ImGui::PopStyleColor(2);
            
		}
	}
}