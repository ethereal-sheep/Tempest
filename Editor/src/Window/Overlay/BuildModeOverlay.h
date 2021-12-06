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
#include "Window/Viewport/CameraControls.h"
#include "Graphics/Basics/RenderSystem.h"

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
            //cam_ctrl.reset(Service<RenderSystem>::Get().GetCamera());
        }

        void open_popup(const Event& e);
        void draw_splitter(float posy);
        void show(Instance&) override;
        void renderTop();
        void renderBtm(Instance& instance);
        void draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat);
        void camEnable(Instance& instance);


        bool OverlayOpen = false;
        id_t current = INVALID;
        CameraControls cam_ctrl;
        LineDrawing line;
        ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
        ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };
        float icon_size = 50.f;
        float border = 10.f;
        float swidth = 250.f;
        float min_swidth = 150.f;
        float max_swidth = 500.f;
    };
}