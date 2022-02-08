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
            const auto name = ICON_FA_COGS " Settings";

            if (enable_popup)
            {
                ImGui::OpenPopup(name);
                enable_popup = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(600.f, 400.f), ImGuiCond_Appearing);
            if (ImGui::BeginPopupModal(name, NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
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
                ImGui::EndPopup();
            }
        }

        bool enable_popup = false;


    };
}