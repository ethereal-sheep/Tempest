#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
namespace Tempest
{
    class AddActionPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            Service<EventManager>::Get().register_listener<AddingActionsTrigger>(&AddActionPopup::open_popup, this);
        }

        void open_popup(const Event&)
        {
            enable_popup = true;

        }

        void show(Instance& instance) override
        {
            if (enable_popup)
            {
                ImGui::OpenPopup("Add Action");
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowSize(ImVec2(500, 550));
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
                ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };

                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
                ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
                ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });

                if (ImGui::BeginPopupModal("Add Action", NULL, flags))
                {
                    ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
                    ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
                    ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.35f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
                    ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
                    ImVec4 textcol = { 0,0,0,1 };
                    if (ImGui::IsWindowFocused() == false)
                    {
                        col = { 0.980f, 0.768f, 0.509f, 0.7f };
                        textcol = { 0,0,0,0.7f };
                    }

                    string te = "ADDING ACTIONS";
                    ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
                    ImGui::PushFont(FONT_OPEN);
                    ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
                    ImGui::PopFont();

                    auto img = tex_map["Assets/Actions.dds"];
                    auto halfToneImg = tex_map["Assets/HalftoneWhite.dds"];
                    ImVec2 imgMin = { winMin.x + ImGui::GetWindowWidth() * 0.15f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
                    ImVec2 imgMax = { imgMin.x + img->GetWidth() * 0.7f, imgMin.y + img->GetHeight() * 0.7f };
                    ImVec2 htMin = { winMin.x, winMin.y + ImGui::GetWindowHeight() * 0.55f };
                    ImVec2 htMax = { htMin.x + halfToneImg->GetWidth(), htMin.y + halfToneImg->GetHeight() };
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(img->GetID()), imgMin, imgMax);
                    ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(halfToneImg->GetID()), htMin, htMax);

                    // START INSERTING STUFF FROM HERE ON


                    { // TO BE REMOVE
                        ImGui::Dummy({ 10.f, 400.f });
                        if (ImGui::Button("Close"))
                        {
                            ImGui::CloseCurrentPopup();
                            enable_popup = false;
                        }
                    }

                    ImGui::EndPopup();
                }
                
                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor(2);
            }
        }

        bool enable_popup = false;
    };
}
