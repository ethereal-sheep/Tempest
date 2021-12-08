/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu)
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
#include "Triggers/Triggers.h"
namespace Tempest
{

    class TutorialPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
            Service<EventManager>::Get().register_listener<TutorialPopupTrigger>(&TutorialPopup::open_popup, this);
            
        }

        void open_popup(const Event&)
        {
            currentIndex = 0;
            tutorial_Arr[0] = tex_map["Assets/Tutorial_0.dds"];
            tutorial_Arr[1] = tex_map["Assets/Tutorial_1.dds"];
            tutorial_Arr[2] = tex_map["Assets/Tutorial_2.dds"];
            tutorial_Arr[3] = tex_map["Assets/Tutorial_3.dds"];
            tutorial_Arr[4] = tex_map["Assets/Tutorial_4.dds"];
            enable_popup = true;
        }

        void show(Instance& instance) override
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            if (enable_popup)
            {
                
                if (ImGui::Begin("Tutorial", &visible, window_flags))
                {
                    auto tut_Arrow = tex_map["Assets/Tutorial_Arrow.dds"];
                    ImVec2 arrow_Size = { (float)tut_Arrow->GetWidth(), (float)tut_Arrow->GetHeight()};
                    
                    
                    auto pos = ImGui::GetCursorPos();
                    ImVec2 max = { pos.x + viewport->Size.x,pos.y + viewport->Size.y };
                   // ImVec2 max = { pos.x + tutorial_Arr[0]->GetWidth() *0.83f,pos.y + tutorial_Arr[0]->GetHeight() * 0.83f };

                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tutorial_Arr[currentIndex]->GetID()), pos, max);

                    ImGui::PushStyleColor(ImGuiCol_Button,{ 0,0,0,0 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,{ 0,0,0,0 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,{ 0,0,0,0 });
                    ImGui::Dummy({ 0.f, (viewport->Size.y * 0.5f) - (arrow_Size.y * 0.5f) });
                    ImGui::Dummy({ (viewport->Size.x * 0.01f), 0.f });
                    ImGui::SameLine();
                    if (currentIndex != 0)
                    {
                        ImGui::PushID("Previous");
                        if(UI::UIImageButton((void*)static_cast<size_t>(tut_Arrow->GetID()), arrow_Size, { 1,0 }, { 0,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
                        {
                            currentIndex--;
                        }
                        ImGui::PopID();
                    }
                    if (currentIndex != 4)
                    {
                        ImGui::SetCursorPos({ pos.x + viewport->Size.x - arrow_Size.x - (viewport->Size.x * 0.01f),   (viewport->Size.y * 0.5f) - (arrow_Size.y * 0.5f) });
                        ImGui::PushID("Next");
                        if (UI::UIImageButton((void*)static_cast<size_t>(tut_Arrow->GetID()), arrow_Size, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
                        {
                            currentIndex++;
                        }
                        ImGui::PopID();
                    }
                    
                    auto tex = tex_map["Assets/closeBtn.dds"];
                    ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.85f,viewport->Size.y * 0.1f });
                    if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
                    {
                        enable_popup = false;
                    }
                    ImGui::PopStyleColor(3);
                }
                ImGui::End();
            }
        }
       
        bool enable_popup = false;
        int currentIndex = 0;
        tsptr<Texture> tutorial_Arr[5];
        ImVec4 tintHover = { 0.980f, 0.768f, 0.509f, 1.f };
        ImVec4 tintPressed = { 0.784f, 0.616f, 0.408f, 1.f };
        ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
        ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
    };
}