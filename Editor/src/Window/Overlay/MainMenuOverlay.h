#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>

namespace Tempest
{
    class MainMenuOverlay : public Window
    {
        enum class UI_SHOW
        {
            INITIAL,
            MAP_UI,
            CONFLICT_UI,
            SETTINGS
        };
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenMainMenuTrigger>(&MainMenuOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        void OpenLocalUI(Instance& instance, const ImGuiViewport& viewport);

        bool OverlayOpen = false;
        UI_SHOW MainMenuUI = UI_SHOW::INITIAL;
    };
}