#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>

namespace Tempest
{
    class SimulateOverlay : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            Service<EventManager>::Get().register_listener<OpenSimulateTrigger>(&SimulateOverlay::open_popup, this);
        }
        void open_popup(const Event& e);

        void show(Instance&) override;

        bool OverlayOpen = false;
        unsigned Tab = 0;

        const float Padding = 100.0f;
        const float HalfPadding = Padding * 0.5f;
        const unsigned NumOfButtons = 6;

        Entity Action = UNDEFINED;
        Entity System = UNDEFINED;

        Entity Attacker = UNDEFINED;
        Entity Defender = UNDEFINED;
    };
}