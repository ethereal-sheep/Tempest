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

struct WaypointEmitter_2D;
struct ExplosionEmitter_2D;

#include <Tempest/src/Audio/AudioEngine.h>
namespace Tempest
{
    class WeaponSheetOverlay : public Window
    {
        enum TABS_TYPE
        {
            WEAPON,
            USE,
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
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

            Service<EventManager>::Get().register_listener<OpenWeaponSheetTrigger>(&WeaponSheetOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<CloseOverlayTrigger>(&WeaponSheetOverlay::close_popup, this);
            Service<EventManager>::Get().register_listener<CloseAllConResOverlayTrigger>(&WeaponSheetOverlay::force_close, this);
            initialise_tabs();
        }

    /*    ~WeaponSheetOverlay() override
        {
            weap = nullptr;
        }*/

        void open_popup(const Event& e);
        void close_popup(const Event& e);
        void force_close(const Event& e);

        void show(Instance&) override;

        void create_new_weapon(Instance& instance);
        void initialise_tabs();
        void display_weapon_stats(const ImGuiViewport& viewport, Instance& instance);
        void display_use_stats(const ImGuiViewport& viewport, Instance& instance);

        template<typename F>
        void render_tabs(TABS_TYPE type, F&& func);

        ImGuiID HoveredID{ 0 };
        int tutorial_index = 0;
        std::array<TabImageData, TOTAL> Tabs;
        TABS_TYPE CurrentTab{ TABS_TYPE::WEAPON };
        bool OverlayOpen = false;
        tc::Weapon NewWeapon;
        tc::Weapon* weap = nullptr; // help
        Entity SelectedID = INVALID;
        interpolater<float> inter{};
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);

        // Task complete VFX
        std::weak_ptr<ExplosionEmitter_2D> m_explosion_VFX;

        bool b_weapon_RENAME_task_vfx = false;
        bool b_weapon_ATK_task_vfx = false;

        // Guiding Tutorial Button VFX
        std::weak_ptr<WaypointEmitter_2D> m_waypointEmitter;

        bool emitter_0 = false;
        bool emitter_1 = false;
        bool emitter_2 = false;
        bool emitter_3 = false;

        ChannelID voice_line;
        bool voice_played{ false };
    };
}