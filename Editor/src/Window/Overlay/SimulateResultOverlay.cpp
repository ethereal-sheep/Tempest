#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/OpenGL/RenderSystem.h"
#include "SimulateResultOverlay.h"

namespace Tempest
{
	void SimulateResultOverlay::open_popup(const Event&)
	{
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	}

	void SimulateResultOverlay::show(Instance&)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			if (ImGui::Begin("SimResult", nullptr, window_flags))
			{
				if (ImGui::Button("CLOSE"))
					OverlayOpen = false;
			}
			ImGui::End();
		}
	}

}