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
#include "Util/interpolater.h"


// Tutorial VFX
struct ExplosionEmitter_2D;
struct LineEmitter_2D;

// In-Game Simulation VFX
struct Unit_Turn_IndicatorEmitter_3D;
struct Smoke_Poof_Emitter_3D;

struct CharacterDamageEmitter_3D;
struct CharacterDeathEmitter_3D;
struct CharacterTileCharged_Emitter_3D;

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

        enum TABS_TYPE
        {
            STAT,
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

            initialise_tabs();

            turn_tex_size = ImVec2{ tex_map["Assets/TurnSelected.dds"]->GetWidth() * 1.0f, tex_map["Assets/TurnSelected.dds"]->GetHeight() * 1.0f };
        }

        void open_popup(const Event& e);
        void visibility(const Event& e);
        void change_turn_order(const Event& e);

        void show(Instance&) override;
        void push_style_color() const;
        void pop_style_color() const;
        void render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity);

        void reset_menu();

        glm::ivec2 calculate_world_mouse(const Camera& cam);
        void no_selected(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void menu(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void attacking(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void moving(RuntimeInstance& instance, const glm::ivec2& world_mouse);
        void glimpse(RuntimeInstance& instance);
        void show_cinematic(RuntimeInstance& instance);
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

        void initialise_tabs();
        void display_unit_stats(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs);
        void display_weapon_stats(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs);
        void display_items(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs);
        void display_actions(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs);

        template<typename F>
        void render_tabs(TABS_TYPE type, F&& func);

        bool OverlayOpen = false;

        tvector<Entity> units;
        tvector<Entity> submitted_units;

        CameraControls cam_ctrl;

        enum struct State {
            NO_SELECTED,
            MENU,
            ATTACKING,
            MOVING,
            GLIMPSE,
            FIGHT,
            CINEMATIC,
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


        Entity cloned_curr_entity = UNDEFINED;
        Entity cloned_other_entity = UNDEFINED;

        uint32_t freq = 10000u;
        std::atomic_uint32_t win;
        std::atomic_uint32_t lose;
        std::atomic_int32_t attack;
        std::atomic_int32_t defend;
        std::atomic_bool finish;

        int damage = 0;
        int result = 0;
        int atk_output = 0;
        int def_output = 0;
        bool atk_rolled = false;
        bool def_rolled = false;

        float action_button_diff = 40.0f;
        float placeholder_height = 0.0f;
        ImVec2 turn_tex_size{ 0.f,0.f };
        ImVec2 action_background_size{0.f,0.f};
        bool display_curr_stat{ false };
        bool display_other_stat{ false };
        INFO_TYPE info_type{ INFO_TYPE::CHAR};
        BATTLE_STATE battle_state{ BATTLE_STATE::CURR_TURN };
        ImGuiID HoveredID{ 0 };

        tsptr<Texture> combat_button_tex[3];

        tmap<int, tmap<int, uint32_t>> range_map;
        tmap<int, tmap<int, bool>> visited;
        std::array<TabImageData, TOTAL> tabs;
        TABS_TYPE current_tab{ TABS_TYPE::STAT };

        int tutorial_index{ 0 };

        interpolater<int> inter1;
        interpolater<int> inter2;
        interpolater<float> inter3;
        interpolater<float> inter4;
        interpolater<float> inter5;
        interpolater<float> inter6;
        interpolater<float> atk_roll_inter[3];
        interpolater<float> def_roll_inter[3];
        interpolater<float> banner;
        interpolater<float> menu1;
        interpolater<float> menu2;

        std::vector<interpolater<float>> inter_nest = std::vector< interpolater<float>>(5);
        std::map<Entity, UnitResult> units_results;

        std::weak_ptr<CharacterDamageEmitter_3D> m_characterDamageEmitter;
        std::weak_ptr<CharacterDeathEmitter_3D> m_characterDeathEmitter;

        bool start_inter_atk_roll = true;
        bool start_inter_def_roll = true;
        bool damageOnce = false;

        std::weak_ptr<LineEmitter_2D> m_winningNumber_VFX;
        bool b_playerOne_Rolled = false;
        bool b_playerTwo_Rolled = false;

        std::weak_ptr<ExplosionEmitter_2D> m_combatRoll_VFX;
        bool b_combatRoll_VFX_Ready = false;

        // 3D VFX
        std::weak_ptr<Unit_Turn_IndicatorEmitter_3D> m_unitTileEmitter;
        bool nextUnit = false;
        bool stopMoving = true;
        
        std::weak_ptr< CharacterTileCharged_Emitter_3D> m_characterAttackEmitter;
        bool beginAttack = true;
        
        std::weak_ptr<Smoke_Poof_Emitter_3D> m_unit_Remove_VFX;
    };
}