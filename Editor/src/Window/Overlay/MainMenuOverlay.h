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
                MainMenuUI = UI_SHOW::NEW_PROJECT;
            }
        }
        void change_state(UI_SHOW state);
        void open_popup(const Event& e);

        void show(Instance&) override;

        void OpenLocalUI(Instance& instance, const ImGuiViewport& viewport);

        bool OverlayOpen = true;
        std::string MapTitle{ "" };
        UI_SHOW MainMenuUI = UI_SHOW::INITIAL;
        int SelectedConflictRes{ 0 };
        string SelectedMap = "";
        std::vector<Entity> SelectedSequences; // will this be an entity id?
        std::vector<bool> OkayConRes = std::vector(3, false);
        std::vector<std::vector<std::pair<Entity, string>>> ConResSequences = std::vector(3, std::vector<std::pair<Entity, string>>());

        interpolater<float> inter{};
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);
    };
}