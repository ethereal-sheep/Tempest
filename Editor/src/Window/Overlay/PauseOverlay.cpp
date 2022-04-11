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
		FromCombatMode = a.fromCombatMode;
	}

	void PauseOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			override_dt(0.f);

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

					if (UI::UIButton_2("Resume Game", "Resume Game", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * (CanOpenGraph ? 0.25f : 0.35f) }, { 50,20 }, FONT_BTN))
					{
						OverlayOpen = false;
						EscDuringPause = false;
					}

					if (CanOpenGraph)
					{
						if (UI::UIButton_2("Edit Action", "Edit Action", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.35f }, { 50,20 }, FONT_BTN))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
							Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION, true);
							EscDuringPause = false;
						}

						if (UI::UIButton_2("Edit Sequence", "Edit Sequence", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.45f }, { 50,20 }, FONT_BTN))
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
							Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(UNDEFINED, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE, true);
							EscDuringPause = false;
						}
					}

					if (UI::UIButton_2("How To Play", "How To Play", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * (CanOpenGraph ? 0.55f : 0.45f) }, { 50,20 }, FONT_BTN))
					{
					}

					if (UI::UIButton_2("Settings", "Settings", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * (CanOpenGraph ? 0.65f : 0.55f) }, { 50,20 }, FONT_BTN))
					{
						Service<EventManager>::Get().instant_dispatch<SettingsTrigger>();
					}

					// where does this go to?
					if (UI::UIButton_2("Quit Game", "Quit Game", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * (CanOpenGraph ? 0.75f : 0.65f) }, { 50,20 }, FONT_BTN))
					{
						if (FromCombatMode)
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<CombatResultsTrigger>();
						}

						else
						{
							auto fn = [&]() {

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

							};

							auto second_fn = [passed = 0.0]() mutable
							{
								/*auto start = std::chrono::system_clock::now();

								for (auto it : fs::directory_iterator(fs::path("Models")))
								{
									auto end = std::chrono::system_clock::now();
									std::chrono::duration<double> diff = end - start;

									if (diff.count() > 1.0)
									{
										passed += diff.count();
										return false;
									}

									if (it.path().extension() != ".a")
										continue;

									Service<RenderSystem>::Get().LoadModel(it.path().string());
								}*/

								return true;
							};

							LoadTrigger t;
							t.do_at_end_fn = fn;
							t.do_until_true_fn = second_fn;

							Service<EventManager>::Get().instant_dispatch<LoadTrigger>(t);
						}
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