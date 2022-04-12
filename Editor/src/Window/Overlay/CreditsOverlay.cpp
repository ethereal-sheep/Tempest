/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "CreditsOverlay.h"
#include <Editor/src/InstanceManager/InstanceConfig.h>
#include <Tempest/src/Audio/AudioEngine.h>

namespace Tempest
{
	void CreditsOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<CreditsTrigger>(e);
		OverlayOpen = true;
		credits = tex_map["Assets/Credits.dds"];
		inter.start(0, credits->GetHeight() * 1.3f, 20);
	}


	void CreditsOverlay::show(Instance&)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			inter.update(ImGui::GetIO().DeltaTime);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.9f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
			if (ImGui::Begin("CombatResults", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/CreditsBG.dds"];
				{
					ImVec2 Min{ 0,0 };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - credits->GetWidth() * 0.5f, viewport->Size.y -inter.get() });
				ImGui::Image((void*)static_cast<size_t>(credits->GetID()), ImVec2{ credits->GetWidth() * 1.0f, credits->GetHeight() * 1.0f });

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.02f,viewport->Size.y * 0.03f });

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
				tex = tex_map["Assets/BackMenuBtn.dds"];

				if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
				{
					auto fn = [&]()
					{
						OverlayOpen = false;
					};

					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, 0.f, fn);
				}

				ImGui::PopStyleColor(3);

				if (inter.is_finished())
				{
					auto fn = [&]()
					{
						OverlayOpen = false;
					};

					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, 0.f, fn);
				}
			}
			ImGui::End();

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