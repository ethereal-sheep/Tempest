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
        tsptr<Texture> AddUnitTex;
        tsptr<Texture> AddActionTex;
        tsptr<Texture> AddLinkTex;
    };
}