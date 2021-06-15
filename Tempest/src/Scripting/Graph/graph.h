#pragma once
#include "Core.h"
#include "Memory.h"
#include "Util/range.h"
#include "../Util/node.h"
#include "../Util/split_view.h"
#include "../Util/var_set.h"

namespace Tempest
{
    static const char* graphs_folder = "Graphs";

    using link = uint64_t;

    class graph_exception : public std::exception
    {
    public:
        graph_exception(const string& err_msg = "graph exception thrown!") : msg(err_msg) {}
        const char* what() const noexcept override { return msg.c_str(); }
    private:
        string msg;
    };


    /**
     * @brief Graph container for nodes, pins, and links.
     */
	class graph
	{
    public:
        friend class GMS;

        using Nodes = tmap<node_id_t, node_ptr>;
        using Links = tset<link>;
        using Variables = ordered_var_set<string>;

        graph(const graph&) = delete;
        graph& operator=(const graph&) = delete;
        graph(graph&&) noexcept = default;
        graph& operator=(graph&&) = default;

        /**
         * @brief Regular construction for a new graph.
         */
        graph(const string & _name = "Default", m_resource * mem = std::pmr::get_default_resource()) :
            name{ _name }, nodes{ mem }, links{ mem } {}

        /**
         * @brief Constructor from a graph file path.
         * @throw Throws graph_exception when the graph is not constructed properly
         */
        graph(const tpath& graph_file, m_resource* mem = std::pmr::get_default_resource());
        

        /**
         * @brief Adds a node to the graph. Returns a pointer to the created
         * node. Returns nullptr if creation is not successful.
         */
        template <typename... Args>
        node* add_node(Args&&... args)
        {
            std::unique_ptr<node> ptr{ std::forward<Args>(args)... };

            if (!ptr) return nullptr;

            LOG_ASSERT(!nodes.count(ptr->get_id()));
            auto [it, check] = nodes.emplace(ptr->get_id(), std::move(ptr));
            LOG_ASSERT(check);

            return it->second.get();
        }

        /**
         * @brief Adds a link to the graph. Returns true if link is added.
         */
        bool add_link(pin_id_t start, pin_id_t end)
        {
            // if link exist, just return
            auto l = concatenate_id_t(start, end);
            if (links.count(l)) return true;

            auto [x, s_index, s_parent] = pin_to_component(start);
            auto [y, e_index, e_parent] = pin_to_component(end);

            // if two input or output pins, fail
            if (x == y) return false;
            // if x is input and y is output, return opposite
            if (x == false) return add_link(end, start);
            // if same parent, fail
            if (s_parent == e_parent) return false;
            // if any of the parent ids cannot be found, fail
            if (!nodes.count(s_parent) || !nodes.count(e_parent)) return false;

            auto s_node = get_node(s_parent);
            auto e_node = get_node(e_parent);

            // if any of the indexes are out of range, fail
            if (s_node->get_num_outputs() <= s_index ||
                e_node->get_num_inputs() <= e_index) return false;
            // if types mismatch, fail
            if (s_node->get_outputs()[s_index].get_type() !=
                e_node->get_inputs()[e_index].get_type()) return false;

            // all okay, insert
            links.insert(l);
            s_node->get_outputs()[s_index].linked = true;
            e_node->get_inputs()[e_index].linked = true;

            return true;
        }

        /**
         * @brief Adds a variable to the graph based on type;
         * @tparam T Type of variable to be added to the graph.
         * @param key Key of variable
         * @param data Data of variable
         */
        template <typename T>
        T* add_var(const string& key, T data = {})
        {
            return variables.create<T>(key, std::move(data));
        }
        /**
         * @brief Adds a variable to the graph based on type;
         * @param key Key of variable
         * @param t Type of variable to be added to the graph.
         */
        var_data* add_var(const string& key, pin_type t)
        {
            if (!variables.create(key, t)) return nullptr;
            return variables.at_if(key);
        }

        /**
         * @brief Removes a node, and all connected links, from the graph.
         */
        void remove_node(node_id_t id)
        {
            // if node doesn't exist, fail
            if (!nodes.count(id)) return;

            // removes connected links
            algo::discard_if(links,
                [&](auto link) -> bool
                {
                    auto [start, end] = split_uint64_t(link);

                    auto [x, s_index, s_parent] = pin_to_component(start);
                    auto [y, e_index, e_parent] = pin_to_component(end);

                    return id == s_parent || id == e_parent;
                });

            // remove node
            nodes.erase(id);
        }
        /**
         * @brief Removes a all nodes, and all connected links, with given name
         */
        void remove_nodes_by_name(const string& node_name)
        {
            tvector<node_id_t> to_delete;
            for (auto& [id, node] : nodes)
            {
                if (node->get_name() == node_name)
                    to_delete.push_back(id);
            }

            for (auto id : to_delete)
                remove_node(id);
        }

        /**
         * @brief Gets name of graph
         */
        void remove_link(pin_id_t start, pin_id_t end)
        {
            auto element = concatenate_id_t(start, end);
            // if link doesn't exist, fail
            if (!links.count(element)) return;

            // remove the link
            links.erase(element);

            // set linked pins to false
            auto [x, s_index, s_parent] = pin_to_component(start);
            auto [y, e_index, e_parent] = pin_to_component(end);

            // since link exist, parents and pins must exist as well
            // so we don't check
            auto s_node = get_node(s_parent);
            auto e_node = get_node(e_parent);
            s_node->get_outputs()[s_index].linked = false;
            e_node->get_inputs()[e_index].linked = false;

        }

        /**
         * @brief Removes a variable from the graph
         */
        void remove_var(const string& key)
        {
            // if key doesn't exist
            if (!variables.count(key)) return;

            // find every node with this name
            string actual = "Local:";
            actual += key;
            remove_nodes_by_name(actual);
        }
        /**
         * @brief Removes a variable from the graph
         */
        bool rename_var(const string& old_name, const string& new_name)
        {
            // if trying to rename the same shit
            if (old_name == new_name) return false;
            // we try to rename the var name, else we return false
            if (!variables.rekey(old_name, new_name)) return false;

            // find every node with this name
            string suffix = "Local:";

            for (auto& [id, node] : nodes)
                if (node->get_name() == suffix + old_name)
                    node->set_name(suffix + new_name);

            return true;
        }
        /**
         * @brief Gets name of graph
         */
        const string& get_name() const { return name; }
        /**
         * @brief Gets filepath of graph
         */
        const tpath& get_path() const { return filepath; }

        /**
         * @brief Gets full path of graph
         */
        tpath get_full_path() const { return filepath / name; }

        /**
         * @brief Get all nodes in the graph
         */
        auto get_nodes() const { return make_const_range(nodes); }
        /**
         * @brief Gets all links
         */
        auto get_links() const { return split_view(links); }
        /**
         * @brief Gets all links in the graph as a split
         */
        auto get_links_as_split() const { return split_view(links); }
        /**
         * @brief Gets all links in the graph as a tri split
         */
        auto get_links_as_tri() const { return tri_split_view(links); }
        /**
         * @brief Gets all links in the graph as a tri split
         */
        auto get_variables() const { return make_const_range(variables); }

        /**
         * @brief Gets a node via the node_id. If node doesn't exist, returns nullptr.
         */
        node* get_node(node_id_t id) {
            if (nodes.count(id))
                return nodes.at(id).get();
            return nullptr;
        }

        /**
         * @brief Clears the entire graph of nodes and links.
         */
        void clear();


    private:

        /**
         * @brief Serialize to current path
         */
        void serialize() const;

        /**
         * @brief Serialize to current path with new name
         */
        void serialize(const string& new_name) const;

        /**
         * @brief Serialize to new path with current name
         */
        void serialize(const tpath& new_folder) const;

        /**
         * @brief Serialize to current path with new name
         */
        void serialize(const tpath& new_folder, const string& new_name) const;



        Writer& _serialize(Writer& writer) const;

        mutable string name;
        mutable tpath filepath;

        Nodes nodes;
        Links links;
        Variables variables;
        
	};

}