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

#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"

#include "Particles/WaypointEmitter.h"

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
            Service<EventManager>::Get().register_listener<CloseOverlayTrigger>(&SimulateOverlay::close_popup, this);
            Service<EventManager>::Get().register_listener<SimulateSelectionConfirm>(&SimulateOverlay::confirm_data, this);
            Service<EventManager>::Get().register_listener<CloseAllConResOverlayTrigger>(&SimulateOverlay::force_close, this);
        }
        void open_popup(const Event& e);
        void confirm_data(const Event& e);
        void close_popup(const Event& e);
        void force_close(const Event& e);

        void show(Instance&) override;

        void display_unit_section(Instance& instance, const ImVec2 start_pos, bool is_attacker);
        void push_button_style() const;
        void pop_button_style() const;

        bool OverlayOpen = false;
        unsigned Tab = 0;

        int tutorial_index = 0;

        const float Padding = 100.0f;
        const float HalfPadding = Padding * 0.5f;
        const unsigned NumOfButtons = 6;

        struct UnitData
        {
            Entity unit_id{ UNDEFINED };
            Entity weapon{ UNDEFINED };
            Entity action{ UNDEFINED };
            void Reset(Instance& instance);
            void Clear();
        };

        UnitData attacker{};
        UnitData defender{};
        Entity sequence{ UNDEFINED };

        uint32_t freq = 10000u;
        std::atomic_uint32_t win;
        std::atomic_uint32_t lose;
        std::atomic_int32_t attack;
        std::atomic_int32_t defend;
        std::atomic_bool finish;

        tsptr<Texture> enter_button;

        float padding{ 0.0f };
        ImVec4 btnTintHover = { 0.922f,0.922f,0.922f,1.f };
        ImVec4 btnTintPressed = { 0.768f, 0.768f, 0.768f, 1.f };

        interpolater<float> inter;
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);

        // For tutorial particle
        std::shared_ptr<WaypointEmitter> m_waypointEmitter;

        bool particle_0 = false;
        bool particle_1 = false;
        bool particle_2 = false;
    };
}