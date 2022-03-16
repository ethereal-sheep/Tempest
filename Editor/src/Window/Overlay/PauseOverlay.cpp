/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "PauseOverlay.h"
#include <Editor/src/InstanceManager/InstanceConfig.h>
#include <Tempest/src/Audio/AudioEngine.h>

namespace Tempest
{
	void PauseOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<PauseOverlayTrigger>(e);
		OverlayOpen = true;
		CanOpenGraph = a.canOpenGraph;
	}

	void PauseOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			
			if (EscDuringPause && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			{
				OverlayOpen = false;
				EscDuringPause = false;
			}
			else
			{

				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
				if (ImGui::Begin("PauseMenu", nullptr, window_flags))
				{
				
					EscDuringPause = true;
					// render title
					ImGui::SetCursorPos(ImVec2{ 0,0 });
					ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
					UI::SubHeader("Paused");
					ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

					if (CanOpenGraph)
					{
						if (UI::UIButton_2("Edit Action Graph", "Edit Action Graph", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.3f }, { 50,20 }, FONT_BTN))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
							Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION, true);
							EscDuringPause = false;
						}

						if (UI::UIButton_2("Edit Sequence Graph", "Edit Sequence Graph", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.4f }, { 50,20 }, FONT_BTN))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
							Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE, true);
							EscDuringPause = false;
						}
					}

					if (UI::UIButton_2("Return", "Return", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f }, { 50,20 }, FONT_BTN))
					{
						OverlayOpen = false;
						EscDuringPause = false;
					}

					// where does this go to?
					if (UI::UIButton_2("Quit", "Quit", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.6f }, { 50,20 }, FONT_BTN))
					{
						AudioEngine ae;
						ae.StopAllChannels();
						OverlayOpen = false;
						EscDuringPause = false;

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
			}

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