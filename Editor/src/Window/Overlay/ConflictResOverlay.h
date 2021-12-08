/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
                Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>

namespace Tempest
{
    class ConflictResOverlay
 : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

            Service<EventManager>::Get().register_listener<OpenConflictResTrigger>(&ConflictResOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = false;
        const unsigned numOfButtons = 10;
        const float padding = 50.0f;
        const float halfPadding = padding * 0.5f;
        ImVec2 initViewPortSize = ImGui::GetMainViewport()->Size;
        int SelectedConflictRes{ 0 };
        std::vector<int> SelectedSequences; // will this be an entity id?
        ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
        ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
    };
}