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
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "Util/interpolater.h"

#include "Particles/WaypointEmitter.h"
#include "Particles/ExplosionEmitter.h"

namespace Tempest
{
    class AttackSystemOverlay : public Window
    {

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override
        {
            window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing;

            Service<EventManager>::Get().register_listener<OpenGraphTrigger>(&AttackSystemOverlay::open_popup, this);
            Service<EventManager>::Get().register_listener<CloseOverlayTrigger>(&AttackSystemOverlay::close_popup, this);
            Service<EventManager>::Get().register_listener<CloseAllConResOverlayTrigger>(&AttackSystemOverlay::force_close, this);

            static ax::NodeEditor::Config a;
            a.SettingsFile = nullptr;

            context = ax::NodeEditor::CreateEditor(&a);
            ax::NodeEditor::SetCurrentEditor(context);


            auto& editorStyle = ax::NodeEditor::GetStyle();

            editorStyle.Colors[0] = { 0,0,0,0 };
            editorStyle.Colors[2] = { 0,0,0,200 / 255.f };
        }

        void exit(Instance&) override
        {
            ax::NodeEditor::DestroyEditor(context);
        }

        void open_popup(const Event& e);
        void close_popup(const Event& e);
        void force_close(const Event& e);

        void show(Instance&) override;

		void draw_context(Instance& instance, float height);
		void draw_node(node_ptr n, const graph& g, Instance& instance);
		void draw_link(link l, pin_id_t from, pin_id_t to, pin_type type);
        void draw_input_pin(const input_pin& p, const graph& g);
		void draw_output_pin(const output_pin& p, float owidth);

		template <typename TNode>
		void node_context(
			graph& g,
			const char* title);

        void draw_context(graph& g, Instance& instance);

        void draw_node_context(graph& g, Instance& instance, ax::NodeEditor::NodeId contextNodeId);
        void draw_pin_context(graph& g, Instance& instance, ax::NodeEditor::PinId contextPinId);
        void draw_link_context(graph& g, Instance& instance, ax::NodeEditor::LinkId contextLinkId);
        void draw_background_context(graph& g, Instance& instance);


        void copy_selected();
        void paste_selected(graph& g, Instance& instance);

		void update_create(graph& g);
		void update_delete(graph& g);
		ImColor get_pin_color(pin_type type);
		ax::Drawing::IconType get_pin_icon(pin_type type);


        ImGuiID HoveredID{ 0 };
		graph temp_graph;
        Entity id = UNDEFINED;

        int tutorial_index = 0;
        
        // copyable stuff
        tvector<node> nodes_copied;
        tvector<size_t> links_copied;
        Entity copy_from = UNDEFINED;

        ax::NodeEditor::EditorContext* context;
		ImVec2 mouse = ImVec2(0, 0);
		bool OverlayOpen = false;
        bool attack_action = false;
        bool defend_action = false;
        std::string overlay_title{""};
        std::string sidebar_title{""};
        OPEN_GRAPH_TYPE type{ OPEN_GRAPH_TYPE::GRAPH_ACTION };
        ImGuiTextFilter filter;

        interpolater<float> inter{};
        std::vector<interpolater<float>> inter_nest = std::vector<interpolater<float>>(3);

        // For tutorial particle
        std::shared_ptr<WaypointEmitter> m_waypointEmitter;
        std::shared_ptr<ExplosionEmitter> m_explosionEmitter;

        bool particle_0 = false;
        //bool particle_1 = false;
        bool particle_2 = false;
        bool particle_3 = false;
        bool particle_4 = false;

        bool tut_openSlide = true;
    };
}