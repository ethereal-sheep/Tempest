/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "../Util/LineDrawing.h"
#include "Util/shape_manip.h"

namespace Tempest
{
    class BuildModeOverlay : public Window
    {
        const char* window_name() override
        {
            return "BuildMode";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

            Service<EventManager>::Get().register_listener<OpenBuildModeOverlay>(&BuildModeOverlay::open_popup, this);

        }

        void open_popup(const Event& e);

        void show(Instance&) override;
        void draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat);
        bool OverlayOpen = false;
        LineDrawing line;
        float icon_size = 50.f;
    };
}