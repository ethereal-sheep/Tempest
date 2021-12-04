/**********************************************************************************
* \author		_ (_@digipen.edu)
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
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

            Service<EventManager>::Get().register_listener<OpenGraphTrigger>(&AttackSystemOverlay::open_popup, this);

            context = ax::NodeEditor::CreateEditor();
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



		graph temp_graph;
        Entity id = UNDEFINED;
        
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
    };
}