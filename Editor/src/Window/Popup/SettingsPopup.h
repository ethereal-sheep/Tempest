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

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Audio/AudioEngine.h"


namespace Tempest
{
    class SettingsPopup : public Window
    {
        const char* window_name() override
        {
            return "";
        }

        void open_popup(const Event& e)
        {
            //auto a = event_cast<SettingsTrigger>(e);
            enable_popup = true;
        }

        void init(Instance&) override
        {

            window_flags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav;

            Service<EventManager>::Get().register_listener<SettingsTrigger>(&SettingsPopup::open_popup, this);



            AudioEngine ae;
            // check APPDATA if there exist a file with project info
            char* pValue;
            size_t len;
            [[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
            if (!pValue)
            {
                LOG_WARN("APPDATA environment variable could not be found!");
                return;
            }
            tpath path{ pValue };
            free(pValue); // It's OK to call free with NULL
            path /= "Documents";
            path /= "CoReSys";
            path /= "_settings.json";

            Serializer serializer;
            string json = serializer.GetJson(path);

            Reader reader(json.c_str());
            if (!reader.HasError())
            {
                reader.StartObject();
                {
                    float vol;
                    reader.Member("Master", vol);
                    ae.SetMasterVolume(std::clamp(vol, 0.f, 1.f));
                }
                {
                    float vol;
                    reader.Member("BGM", vol);
                    ae.SetBusVolume("BGM", std::clamp(vol, 0.f, 1.f));
                }
                {
                    float vol;
                    reader.Member("SFX", vol);
                    ae.SetBusVolume("SFX", std::clamp(vol, 0.f, 1.f));
                }
                {
                    auto& gv = Service<RenderSystem>::Get().gammaValue;
                    reader.Member("Gamma", gv);
                }
                reader.EndObject();
            }
            else
                LOG("No existing settings found.");
        }

        void exit(Instance&) override
        {
            AudioEngine ae;
            // check APPDATA if there exist a file with project info
            char* pValue;
            size_t len;
            [[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
            if (!pValue)
            {
                LOG_WARN("APPDATA environment variable could not be found!");
                return;
            }
            tpath envpath{ pValue };
            free(pValue); // It's OK to call free with NULL
            envpath /= "Documents";
            envpath /= "CoReSys";

            if (!fs::exists(envpath))
                fs::create_directory(envpath);


            Writer writer;
            writer.StartObject();
            {
                float vol = ae.GetMasterVolume();
                writer.Member("Master", vol);
            }
            {
                float vol = ae.GetBusVolume("BGM");
                writer.Member("BGM", vol);
            }
            {
                float vol = ae.GetBusVolume("SFX");
                writer.Member("SFX", vol);
            }
            {
                auto& gv = Service<RenderSystem>::Get().gammaValue;
                writer.Member("Gamma", gv);
            }
            writer.EndObject();
            Serializer::SaveJson(envpath / "_settings.json", writer.GetString());
        }

        void show(Instance&) override
        {
            const auto wrap_width = 200.f;
            const auto name = ICON_FA_COGS "Settings";

            if (enable_popup)
            {
                ImGui::OpenPopup(name);
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(930.f, 658.f), ImGuiCond_Appearing);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
            ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
            ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
            ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });
            if (ImGui::BeginPopupModal(name, NULL, flags))
            {
                //Adding Top Right yellow box
                ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
                ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
               
                ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };

                string te = "SETTINGS";
                ImGui::PushFont(FONT_OPEN);
                auto textSize = ImGui::CalcTextSize(te.c_str());
                ImVec2 boxMax = { winMin.x + textSize.x + ImGui::GetWindowWidth() * 0.15f , winMin.y + textSize.y + 10.f };
                ImGui::GetWindowDrawList()->AddRectFilled(winMin, boxMax, ImGui::GetColorU32(col));
                ImGui::GetWindowDrawList()->AddText(TextMin, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
                ImGui::PopFont();
                
                //auto img = tex_map["Assets/SettingBox.dds"];
                //ImGui::Image((void*)static_cast<size_t>(img->GetID()), { (float)img->GetWidth(), (float)img->GetHeight() });

                //Setting Child Window Pos & Size
                auto img = tex_map["Assets/SoundSetting.dds"];
                float paddingX = (float)img->GetWidth() * 0.075f;
                float sliderWidth = 0.f;
                float paddingBetween = 20.f;
                AudioEngine ae;
                ImGui::Dummy({ 0,ImGui::GetWindowHeight() * 0.1f });
                ImGui::Dummy({ (ImGui::GetWindowWidth() - (float)img->GetWidth()) * 0.5f ,0 });
                ImGui::SameLine();
                ImGui::BeginChild("SettingsID", ImVec2((float)img->GetWidth(), 456.f), true);

                // ============== SOUND SETTINGS ============== 
                ImGui::Image((void*)static_cast<size_t>(img->GetID()), { (float)img->GetWidth(), (float)img->GetHeight() });
               
                ImGui::PushFont(FONT_BODY);
                //Volume
                
                ImGui::Dummy({ 0, paddingBetween });
                ImGui::Text("Volume");
                ImGui::SameLine();
                ImGui::Dummy({paddingX - ImGui::GetItemRectSize().x, 0.f });
                ImGui::SameLine();
                UI::UICheckBox_1("", &isVolMute);
                ImGui::SameLine();
                auto SliderPosX = ImGui::GetCursorPosX();
                sliderWidth = (float)img->GetWidth() - SliderPosX;
                ImGui::PushItemWidth(sliderWidth);
                {
                    float vol = ae.GetMasterVolume();
                    
                    if (ImGui::SliderFloat("##MasterVolume", &vol, 0.f, 1.f))
                    {
                        ae.SetMasterVolume(std::clamp(vol, 0.f, 1.f));
                    }
                } 
                ImGui::PopItemWidth();

                //SFX
                ImGui::Dummy({ 0, paddingBetween });
                ImGui::Text("SFX");
                ImGui::SameLine();
                ImGui::Dummy({ paddingX - ImGui::GetItemRectSize().x, 0.f });
                ImGui::SameLine();
                UI::UICheckBox_1("", &isVolMute);
                ImGui::SameLine();
                ImGui::PushItemWidth(sliderWidth);
                {
                    float vol = ae.GetBusVolume("SFX");
                    if (ImGui::SliderFloat("##SFX", &vol, 0.f, 1.f))
                    {
                        ae.SetBusVolume("SFX", std::clamp(vol, 0.f, 1.f));
                    }
                }
                ImGui::PopItemWidth();

                //BGM
                ImGui::Dummy({ 0, paddingBetween });
                ImGui::Text("BGM");
                ImGui::SameLine();
                ImGui::Dummy({ paddingX - ImGui::GetItemRectSize().x, 0.f });
                ImGui::SameLine();
                UI::UICheckBox_1("", &isVolMute);
                ImGui::SameLine();
                ImGui::PushItemWidth(sliderWidth);
                {
                    float vol = ae.GetBusVolume("BGM");
                    if (ImGui::SliderFloat("##BGM", &vol, 0.f, 1.f))
                    {
                        ae.SetBusVolume("BGM", std::clamp(vol, 0.f, 1.f));
                    }
                }
                ImGui::PopItemWidth();


                // ============== VISUAL SETTINGS ============== 
                img = tex_map["Assets/VisualSetting.dds"];
                ImGui::Dummy({ 0,paddingBetween });
                ImGui::Image((void*)static_cast<size_t>(img->GetID()), { (float)img->GetWidth(), (float)img->GetHeight() });
                //Gamma
                ImGui::Dummy({ 0, paddingBetween });
                ImGui::Text("Gamma");
                ImGui::SameLine();
                ImGui::SetCursorPosX(SliderPosX);
                ImGui::PushItemWidth(sliderWidth);
                {
                    auto& gv = Service<RenderSystem>::Get().gammaValue;
                    if (ImGui::SliderFloat("##Gamma", &gv, 1.f, 4.f))
                    {
                        gv = std::clamp(gv, 1.f, 4.f);
                    }
                }
                ImGui::PopItemWidth();
                ImGui::PopFont();

                
                ImGui::EndChild();
                if (UI::UIButton_2("Confirm", "Confirm", {ImGui::GetWindowWidth() * 0.84f, ImGui::GetWindowHeight() * 0.9f }, { -20.f,10.f }, FONT_BODY))
                {
                    ImGui::CloseCurrentPopup();
                    enable_popup = false;
                }
                //ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(img->GetID()), imgMin, imgMax);

                /*if (ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Sound"))
                    {
                        AudioEngine ae;

                        // master
                        {
                            float vol = ae.GetMasterVolume();
                            if (ImGui::SliderFloat("Master", &vol, 0.f, 1.f))
                            {
                                ae.SetMasterVolume(std::clamp(vol, 0.f, 1.f));
                            }
                        }

                        // bgm
                        {
                            float vol = ae.GetBusVolume("BGM");
                            if (ImGui::SliderFloat("BGM", &vol, 0.f, 1.f))
                            {
                                ae.SetBusVolume("BGM", std::clamp(vol, 0.f, 1.f));
                            }
                        }
                        // sfx
                        {
                            float vol = ae.GetBusVolume("SFX");
                            if (ImGui::SliderFloat("SFX", &vol, 0.f, 1.f))
                            {
                                ae.SetBusVolume("SFX", std::clamp(vol, 0.f, 1.f));
                            }
                        }

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Graphics"))
                    {
                        // gamma
                        {
                            auto& gv = Service<RenderSystem>::Get().gammaValue;
                            if (ImGui::SliderFloat("Gamma", &gv, 1.f, 4.f))
                            {
                                gv = std::clamp(gv, 1.f, 4.f);
                            }
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::Text("\n");

                if (ImGui::Button("Close")) {
                    ImGui::CloseCurrentPopup();
                }
                */
                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);
        }
        bool isVolMute = false;
        bool enable_popup = false;


    };
}