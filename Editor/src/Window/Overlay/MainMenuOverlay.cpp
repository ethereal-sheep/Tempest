#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "MainMenuOverlay.h"

namespace Tempest
{
	void MainMenuOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	}

	void MainMenuOverlay::show(Instance&)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			if (ImGui::Begin("MainMenu", nullptr, window_flags))
			{
				//auto bgImg = tex_map["Assets/StartScreenBG.png"];
			}
			ImGui::End();
		}
	}

}