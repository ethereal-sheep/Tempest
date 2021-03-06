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
#include "Instance/EditTimeInstance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"
#include <Tempest/src/Audio/AudioEngine.h>

#include "Particles/Particles_2D/EmitterSystem_2D.h"
//#include "Particles/Particles_2D/LineEmitter_2D.h"
#include "Particles/Particles_2D/RandomMinMaxSpawnPointEmitter_2D.h"

namespace Tempest
{
    class MainMenuOverlay : public Window
    {
    public:
        enum class UI_SHOW
        {
            INITIAL,
            PROJECTS,
            NEW_PROJECT,
            MAIN_PAGE,
            CONFLICT_RES,
            SELECT_MAP,
            LOAD_MAP,
            SELECT_CONFLICT_RES,
        };

    private:
        const char* window_name() override
        {
            return "";
        }
        void init(Instance& instance) override
        {
            window_flags |=
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

            Service<EventManager>::Get().register_listener<OpenMainMenuTrigger>(&MainMenuOverlay::open_popup, this);
            if (dynamic_cast<EditTimeInstance*>(&instance))
            {
                change_state(UI_SHOW::MAIN_PAGE);
            }
            else
            {
                change_state(UI_SHOW::INITIAL);
            }

            AudioEngine ae;
            ae.StopAllChannels();
            MenuBGM = ae.Play("Sounds2D/CoReSyS_BGM1.wav", "BGM", 0.7f, true);

            EmitterSystem_2D::GetInstance().CreateBackgroundEmitter(m_bg_VFX, glm::vec2(0.f, 0.f), glm::vec2(1920.f, 1080.f));
            //EmitterSystem_2D::GetInstance().CreateBackgroundEmitter(m_bg_VFX, glm::vec2(0.f, 0.f), glm::vec2(2560.f, 1440.f));

            m_bg_VFX.lock()->m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 0.5f };
            m_bg_VFX.lock()->m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };

            //EmitterSystem_2D::GetInstance().CreateLineEmitter(m_bg_VFX_2, glm::vec2{ 0.0f, 1440.f }, glm::vec2(2560.f, 1440.f));
            //m_bg_VFX_2.lock()->m_PAM.m_velocityVariation = glm::vec2{ 0.f, -500.0f };
            //m_bg_VFX_2.lock()->m_EM.m_spawnTimeInterval = 0.016f;
            //m_bg_VFX_2.lock()->m_EM.m_spawnCountTimer = m_bg_VFX_2.lock()->m_EM.m_spawnTimeInterval;
            //m_bg_VFX_2.lock()->m_EM.m_rateOverTime = 1;
        }
        void change_state(UI_SHOW state);
        void open_popup(const Event& e);

        void show(Instance&) override;

        void OpenLocalUI(Instance& instance, const ImGuiViewport& viewport);

        bool appearing = true;

        bool OverlayOpen = true;
        std::string MapTitle{ "" };
        UI_SHOW MainMenuUI = UI_SHOW::INITIAL;
        int SelectedConflictRes{ 0 };
        int SelectedSequencesint{ -1 };
        tset<string> SelectedMaps;
        string SelectedMap = "";
        string SelectedMapPreview = "Assets/EmptyMapBG.dds";
        string NewMapName = "Map";
        string NewProjectName = "Blank";
        ImGuiID HoveredID{ 0 };
        std::vector<Entity> SelectedSequences; // will this be an entity id?
        std::vector<bool> OkayConRes = std::vector(3, false);
        std::vector<std::vector<std::pair<Entity, string>>> ConResSequences = std::vector(3, std::vector<std::pair<Entity, string>>());

        interpolater<float> inter{};
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);
        ImVec4 tintPressed = { 0.305f, 0.612f, 0.717f, 1.f };
        ImVec4 tintHover = { 0.443f, 0.690f, 0.775f, 1.f };
        ChannelID MenuBGM = 0;

        //Main Page texture change
        string confliceResBtn = "Assets/ConflictResolutionButton.dds";
        string startGameBtn = "Assets/StartGameButton.dds";
        string mapBuilderBtn = "Assets/MapBuilderButton.dds";
        string newMapBtn = "Assets/NewMapButton.dds";
        string loadMapBtn = "Assets/LoadMapButton.dds";

        string projectIconBtn = "Assets/ProjectIconBtn_default.dds";
        string settingIconBtn = "Assets/SettingIconBtn_default.dds";
        string creditsIconBtn = "Assets/CreditsIconBtn_default.dds";
        string quitIconBtn = "Assets/QuitIconBtn_default.dds";

        //BackGround Image
        string backGroundImg = "Assets/StartScreenBG.dds";

        // BG VFX
        std::weak_ptr<RandomMinMaxSpawnPointEmitter_2D> m_bg_VFX;
        //std::weak_ptr<LineEmitter_2D> m_bg_VFX_2;
    };
}