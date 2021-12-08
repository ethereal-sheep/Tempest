/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
                Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "../Viewport/CameraControls.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Instance/RuntimeInstance.h"

namespace Tempest
{
    class CombatModeOverlay : public Window
    {
        enum class INFO_TYPE
        {
            CHAR,
            ACTIONS,
            WEAPONS
        };

        enum class BATTLE_STATE
        {
            CURR_TURN,       // display character information and action options
            SELECT_ACTION,   // select performable actions of character
            SELECT_WEAPON,   // select weapons from inventory 
            SELECT_OTHER,    // select enemy to perform action on
            BATTLE_GLIMPSE,  // display chance of success of selected action
            COMMENCE_BATTLE  // simulate battle
        };

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

            Service<EventManager>::Get().register_listener<OpenCombatModeTrigger>(&CombatModeOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<CombatModeVisibility>(&CombatModeOverlay::visibility, this);
            Service<EventManager>::Get().register_listener<ChangeTurnOrder>(&CombatModeOverlay::change_turn_order, this);
        }
        void open_popup(const Event& e);
        void visibility(const Event& e);
        void change_turn_order(const Event& e);

        void show(Instance&) override;
        void push_style_color() const;
        void pop_style_color() const;
        void render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity);

        glm::ivec2 calculate_world_mouse(const Camera& cam);
        void no_selected(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void menu(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void attacking(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void moving(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void glimpse(RuntimeInstance& instance);
        void fight(RuntimeInstance& instance);
        void display_selected(RuntimeInstance& instance, Entity character);
        void display_selected_right(RuntimeInstance& instance, Entity character);
        Entity increase_turn();

        void try_build_all(Instance& instance)
        {
            instance.srm.clear();
            for (auto id : instance.ecs.view<tc::Graph>())
            {
                auto& sys = instance.ecs.get<tc::Graph>(id);
                try_build(id, sys.g, instance);
            }
        }

        void try_build(Entity id, graph& g, Instance& instance)
        {
            tmap<node_id_t, script*> map;

            // add variables
            for (auto& [name, type, var] : g.get_variables())
            {
                instance.srm.add_variable_to_id(id, name, var);
            }

            // add node scripts
            for (auto& [node_id, ptr] : g.get_nodes())
            {
                map[node_id] = ptr->create_script_pack(id, instance);
            }

            // link scripts
            for (auto [start, end] : g.get_links_as_split())
            {
                auto [x, s_index, s_parent] = pin_to_component(start);
                auto [y, e_index, e_parent] = pin_to_component(end);

                LOG_ASSERT(map.count(s_parent));
                LOG_ASSERT(map.count(e_parent));

                auto s_node = g.get_node(s_parent);
                auto e_node = g.get_node(e_parent);

                auto pin = s_node->get_output_pin(s_index);
                auto type = pin->get_type();

                if (type == pin_type::Flow)
                {
                    map[s_parent]->set_next(map[e_parent], s_index); // script of next script
                }
                else
                {
                    map[e_parent]->set_input(
                        e_index - e_node->get_num_input_flows(), // index of input pin in script
                        map[s_parent], // script of prev script
                        s_index - s_node->get_num_output_flows()); // index of out pin in prev script
                }
            }
        }

        bool OverlayOpen = false;

        tvector<Entity> units;

        CameraControls cam_ctrl;

        enum struct State {
            NO_SELECTED,
            MENU,
            ATTACKING,
            MOVING,
            GLIMPSE,
            FIGHT,
            NEXT,
        };

        State state = State::NO_SELECTED;

        unsigned curr_turn;

        Entity curr_entity = UNDEFINED;
        Entity selected_action = UNDEFINED;
        Entity selected_weapon = UNDEFINED;

        Entity other_entity = UNDEFINED;
        Entity other_selected_action = UNDEFINED;
        Entity other_selected_weapon = UNDEFINED;

        uint32_t freq = 10000u;
        std::atomic_uint32_t win;
        std::atomic_uint32_t lose;
        std::atomic_int32_t attack;
        std::atomic_int32_t defend;
        std::atomic_bool finish;

        int result = 0;
        int atk_output = 0;
        int def_output = 0;
        bool atk_rolled = false;
        bool def_rolled = false;

        float action_button_diff = 0.0f;
        float placeholder_height = 0.0f;
        ImVec2 action_background_size{0.f,0.f};
        bool display_curr_stat{ false };
        bool display_other_stat{ false };
        INFO_TYPE info_type{ INFO_TYPE::CHAR};
        BATTLE_STATE battle_state{ BATTLE_STATE::CURR_TURN };


        tmap<int, tmap<int, uint32_t>> range_map;
        tmap<int, tmap<int, bool>> visited;
    };
}