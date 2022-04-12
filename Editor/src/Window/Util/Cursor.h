/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Util/interpolater.h"

namespace Tempest
{
    class Cursor : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {

        }



        void show(Instance&) override
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
            auto cursor_id = tex_map["Assets/Cursor.png"];

            auto size = ImVec2{ (float)cursor_id->GetWidth(), (float)cursor_id->GetHeight() };
            auto pos = ImGui::GetMousePos();
            ImGui::GetForegroundDrawList()->AddImage((ImTextureID)cursor_id->GetID(), pos, pos + size);
        }



    };
}