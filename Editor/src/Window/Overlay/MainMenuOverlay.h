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
#include "Instance/EditTimeInstance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"
#include <Tempest/src/Audio/AudioEngine.h>

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
        tset<string> SelectedMaps;
        string SelectedMap;
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
    };
}