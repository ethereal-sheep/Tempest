/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
                Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
#include "Util/interpolater.h"
#include <Tempest/src/Audio/AudioEngine.h>

struct Smoke_Poof_Emitter_3D;

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
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs;

            Service<EventManager>::Get().register_listener<OpenBuildModeOverlay>(&BuildModeOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<BuildModeTutorialIndexTrigger>(&BuildModeOverlay::tutorial_index_trigger, this);
            //cam_ctrl.reset(Service<RenderSystem>::Get().GetCamera());
        }

        void open_popup(const Event& e);
        void tutorial_index_trigger(const Event& e);
        void draw_splitter(float posy);
        void show(Instance&) override;
        void renderTop();
        void renderBtm(Instance& instance);
        void draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat);
        void camEnable(Instance& instance);
        bool is_metal_prototype(std::string name);


        bool OverlayOpen = false;
        int tutorial_index = 0;
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
        ImVec2 place_box;
        ImVec2 place_box_size;
        ImVec2 ButtonSize = { 60.f,60.f };
        tc::Transform temp_xform;
        interpolater<float> banner;
        tvector<interpolater<float>> inter_nest = tvector<interpolater<float>>(5);
        tsptr<Texture> option_btns[3];

        ChannelID MapBuilderBGM = 0;
        ChannelID voice_line{ 0 };
        bool voice_played[8] = { false, false, false, false,
                                false, false, false, false };
        ImGuiTextFilter filter;
        std::string metal_prototypes[6] = { "Cell Bench", "Metal Door", "Lab Machine", "Canteen Sink", "Canteen Tray Trolley", "Security Locker" };
        std::weak_ptr<Smoke_Poof_Emitter_3D> m_Prop_Placement_VFX;
    };
}