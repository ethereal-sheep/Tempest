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

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"
#include "Window/Viewport/CameraControls.h"

struct WaypointEmitter_2D;
struct ExplosionEmitter_2D;

#include <Tempest/src/Audio/AudioEngine.h>

namespace Tempest
{
    class UnitSheetOverlay : public Window
    {
        enum TABS_TYPE
        {
            UNIT,
            WEAPON,
            ACTION,
            TOTAL
        };

        struct TabImageData
        {
            enum STATE
            {
                UNHOVER,
                HOVER
            };

            std::array< void*, 2> image_id;
            STATE current_state{ UNHOVER };
            ImVec2 size{ 0,0 };
            bool is_active{ false };
        };

        const char* window_name() override
        {
            return "UnitSheetOverlay";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

            Service<EventManager>::Get().register_listener<OpenUnitSheetTrigger>(&UnitSheetOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<CloseOverlayTrigger>(&UnitSheetOverlay::close_popup, this);
            Service<EventManager>::Get().register_listener<SimulateSelectionConfirm>(&UnitSheetOverlay::confirm_data, this);
            Service<EventManager>::Get().register_listener<CloseAllConResOverlayTrigger>(&UnitSheetOverlay::force_close, this);

            initialise_tabs();

            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.5f, 0.9f,
                    saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f; // Alpha
            }
        }

       /* ~UnitSheetOverlay() override
        {
            cs = nullptr;
        }*/

        void open_popup(const Event& e);
        void confirm_data(const Event& e);
        void close_popup(const Event& e);
        void force_close(const Event& e);

        void show(Instance&) override;

        void create_new_unit(Instance& instance);

        // helpers
        void push_button_style() const;
        void pop_button_style() const;
        void initialise_tabs();
        void display_unit_stats(const ImGuiViewport& viewport, Instance& instance);
        void display_weapon_stats(const ImGuiViewport& viewport, Instance& instance);
        void display_actions(const ImGuiViewport& viewport, Instance& instance);
       

        template<typename F>
        void render_tabs(TABS_TYPE type, F&& func);

        ImVec4 saved_palette[32] = {};

        bool OverlayOpen = false;
        bool IsUnitCreation = false;
        std::array<TabImageData, TOTAL> Tabs;
        TABS_TYPE CurrentTab{ TABS_TYPE::UNIT };
        tc::Character NewCharacter;
        tc::Character* cs = nullptr; //help
        Entity SelectedID = INVALID;
        Entity TempWeapon = UNDEFINED;
        Entity TempAction = UNDEFINED;
        ImGuiID HoveredID{ 0 };

        interpolater<float> inter{};
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);
        int tutorial_index = 0;
        bool tut_openSlide = true;

        CameraControls cam_ctrl;


        // Task complete VFX
        std::weak_ptr<ExplosionEmitter_2D> m_explosion_VFX;

        bool b_unit_NAME_task_vfx = false;
        bool b_unit_ATK_task_vfx = false;
        bool b_unit_HP_task_vfx = false;

        // Guiding Tutorial Button VFX
        std::weak_ptr<WaypointEmitter_2D> m_waypointEmitter;

        bool emitter_0 = false;
        bool emitter_1 = false;
        bool emitter_2 = false;
        bool b_Weapon_button_vfx = false;

        ChannelID voice_line{ 0 };
        bool voice_played{ false };

   /*     bool AddWeaponPopup = false;
        bool EditWeaponPopup = false;
        bool CreateOpen = false;
        std::string Title = "Unit Creation";
        tc::Weapon NewWeap;
        tc::Weapon EditWeap;
        tc::Character NewCharacter;
        Entity CurSelection = UNDEFINED;
        Entity SelectedID = INVALID;
        std::string NewStatName = "Stat";
        int NewStatValue = 0;*/
    };
}