#pragma once
#include "Graph/graph.h"
#include "Util/range.h"

namespace Tempest
{
	static const char* graphs_folder = "Graphs";

	class GMS
	{
	public:
		using Graphs = tvector<graph>;

		GMS(m_resource* mem = std::pmr::get_default_resource()) :
			memory_resource(mem),
			graphs(mem) {}

		string new_graph()
		{
			// make sure name is unique
			string new_name = algo::get_next_name("Untitled", graphs.begin(), graphs.end(), 
				[](auto& graph, auto& string)
				{
					return graph.get_name() == string;
				});

			graphs.emplace_back(new_name, memory_resource);
			return new_name;
		}

		bool save_all(const tpath& root_filepath) const
		{
			tpath folder = root_filepath / graphs_folder;

			// if directory doesn't exist, create new_directory
			if (!std::filesystem::exists(folder))
				std::filesystem::create_directory(folder);

			bool check = true;

			for (auto& savable : graphs)
			{
				try
				{
					savable.serialize(folder);
				}
				catch (const graph_exception& except)
				{
					LOG_ERROR("{}", except.what());
					check = false;
				}
			}

			return check;
		}

		bool save(const tpath& root_filepath, const graph& savable) const
		{
			tpath folder = root_filepath / graphs_folder;

			// if directory doesn't exist, create new_directory
			if (!std::filesystem::exists(folder))
				std::filesystem::create_directory(folder);

			try
			{
				savable.serialize(folder);
			}
			catch (const graph_exception& except)
			{
				LOG_ERROR("{}", except.what());
				return false;
			}
			return true;
		}

		bool save_as(const tpath& root_filepath, const graph& savable, const string& new_name) const
		{
			// if name is same, we just save
			if (savable.get_name() == new_name)
				return save(root_filepath, savable);

			// make sure new_name is not opened currently
			auto find = std::find_if(graphs.begin(), graphs.end(),
				[&](auto& g)
				{
					return new_name == g.get_name();
				});
			if (find != graphs.end()) return false;


			tpath folder = root_filepath / graphs_folder;

			// if directory doesn't exist, create new_directory
			if (!std::filesystem::exists(folder))
				std::filesystem::create_directory(folder);

			try
			{
				savable.serialize(folder, true, new_name);
			}
			catch (const graph_exception& except)
			{
				LOG_ERROR("{}", except.what());
				return false;
			}
			return true;
		}


		bool load(const tpath& graph_filepath)
		{
			try
			{
				graphs.emplace_back(graph_filepath, memory_resource);
			}
			catch (const graph_exception& except)
			{
				LOG_ERROR("{}", except.what());
				return false;
			}
			return true;
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