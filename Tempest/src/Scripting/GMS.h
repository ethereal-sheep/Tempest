#pragma once
#include "Graph/graph.h"
#include "Util/range.h"

namespace Tempest
{

	/**
	 * @brief Graph Manager System. Manages all open graphs and allows them to be edited 
	 * and stuff.
	 */
	class GMS
	{
	public:
		using Graphs = tvector<graph>;

		/**
		 * @brief Graph Manager System. Manages all open graphs and allows them to be edited
		 * and stuff.
		 */
		GMS(m_resource* mem = std::pmr::get_default_resource()) :
			memory_resource(mem),
			graphs(mem) {}

		/**
		 * @brief Creates a new graph and returns it.
		 */
		graph& new_graph(GraphType type = GraphType::Regular)
		{
			// make sure name is unique
			string new_name = algo::get_next_name("Untitled", graphs.begin(), graphs.end(), 
				[](auto& graph, auto& string)
				{
					return graph.get_name() == string;
				});

			return graphs.emplace_back(new_name, type, memory_resource);
		}

		/**
		 * @brief Loads a graph from a filepath and returns it.
		 * @throw exception if graph is not loaded properly
		 */
		graph& load(const tpath& graph_filepath)
		{
			// check if filepath exist
			tpath check_path = graph_filepath;
			check_path.replace_extension();

			// check if file has been loaded already
			for (auto& graph : graphs)
			{
				if (graph.get_full_path() == check_path)
					return graph;
			}
			return graphs.emplace_back(check_path.replace_extension(".json"), memory_resource);
		}

		/**
		 * @brief Saves all graphs in the current GMS. Reference to unsaved graphs are
		 * returned.
		 */
		tvector<string> save_all() const
		{
			tvector<string> unsavable;
			for (auto& savable : graphs)
			{
				try
				{
					savable.serialize();
				}
				catch (const std::exception& a)
				{
					unsavable.push_back(savable.get_name());
					LOG_ERROR("{}", a.what());
				}
			}
			return unsavable;
		}
		/**
		 * @brief Saves all graphs in the current GMS. Reference to unsaved graphs are
		 * returned.
		 */
		tvector<string> save_all_to(const tpath& filepath) const
		{
			// if directory doesn't exist
			if (!std::filesystem::exists(filepath))
				throw graph_exception(string("Graph: ") + filepath.string() + " does not exist!");

			tvector<string> unsavable;
			for (auto& savable : graphs)
			{
				try
				{
					savable.serialize(filepath);
				}
				catch (const std::exception& a)
				{
					unsavable.push_back(savable.get_name());
					LOG_ERROR("{}", a.what());
				}
			}

			return unsavable;
		}

		/**
		 * @brief Saves a graph.
		 */
		void save(const graph& savable) const
		{
			savable.serialize();
		}

		/**
		 * @brief Saves a graph to a new location as a new name
		 */
		void save_as(const tpath& filepath, const string& new_name, const graph& savable) const
		{
			// if directory doesn't exist
			if (!std::filesystem::exists(filepath))
				throw graph_exception(string("Graph: ") + filepath.string() + " does not exist!");

			// if name is same, we just save
			if (savable.get_name() == new_name)
				save_to(filepath, savable);

			// make sure new_name is not opened currently
			auto find = std::find_if(graphs.begin(), graphs.end(),
				[&](const graph& g)
				{
					return (filepath / new_name) == g.get_full_path();
				});

			if (find != graphs.end())
				throw graph_exception(string("Graph: ") + (filepath / new_name).string() + " is currently opened in editor!");

			savable.serialize(filepath, new_name);
		}

		/**
		 * @brief Saves a graph to a new location.
		 */
		void save_to(const tpath& filepath, const graph& savable) const
		{
			// if directory doesn't exist
			if (!std::filesystem::exists(filepath))
				throw graph_exception(string("Graph: ") + filepath.string() + " does not exist!");

			// make sure new_name is not opened currently
			auto find = std::find_if(graphs.begin(), graphs.end(),
				[&](const graph& g)
				{
					return (filepath / savable.get_name()) == g.get_full_path();
				});

			if (find != graphs.end())
				throw graph_exception(string("Graph: ") + (filepath / savable.get_name()).string() + " is currently opened in editor!");

			savable.serialize(filepath);
		}

		auto get_graphs()
		{
			return make_range(graphs);
		}

		auto get_graphs() const
		{
			return make_range(graphs);
		}

		void clear() 
		{ 
			graphs.clear(); 
		}

	private:
		m_resource* memory_resource;
		Graphs graphs;
		
	};


	
}