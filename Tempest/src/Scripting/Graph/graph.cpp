#include "graph.h"
#include "Util.h"

namespace Tempest
{

	graph::graph(const tpath& graph_file, m_resource* mem) :
		name{ graph_file.stem().string() }, nodes{ mem }, links{ mem }
	{
		Serializer serializer;
		string json = serializer.GetJson(graph_file);

		Reader reader(json.c_str());
		if (reader.HasError())
			throw graph_exception(graph_file.filename().string() + ": File cannot be opened!");

		// used to map old ids to new ids
		tmap<node_id_t, node_id_t> old_new_ids;

		reader.StartObject();
		{
			// meta part
			{
				reader.StartMeta();
				{
					string meta;
					reader.Member("Type", meta);
					if (meta != "Graph")
						throw graph_exception(graph_file.filename().string() + ": Bad Meta Member!");
				}
				reader.EndMeta();
			}

			// nodes part
			{
				size_t size = 0;
				reader.StartArray("Nodes", &size);
				for (size_t i = 0; i < size; ++i)
				{
					string metacategory, metatype;
					reader.StartObject();
					{
						reader.StartMeta();
						{
							reader.Member("Category", metacategory);
							reader.Member("Type", metatype);
						}
						reader.EndMeta();

						auto node = add_node(CreateNode(metacategory, metatype));
						if (!node) continue; // warn here


						reader.StartObject("Node");
						{
							node_id_t old_id;
							reader.Member("Id", old_id);
							reader.Member("Size", node->size);
							reader.Member("Position", node->position);

							// add the new id into 
							old_new_ids[old_id] = node->get_id();
						}
						reader.EndObject();
					}
					reader.EndObject();
				}
				reader.EndArray();
			}
			// links part
			{
				size_t size = 0;
				reader.StartArray("Links", &size);
				for (size_t i = 0; i < size; ++i)
				{
					pin_id_t old_start;
					pin_id_t old_end;
					reader.StartObject();
					{
						reader.Member("Start", old_start);
						reader.Member("End", old_end);
					}
					reader.EndObject();

					auto [x, s_index, s_parent] = pin_to_component(old_start);
					auto [y, e_index, e_parent] = pin_to_component(old_end);

					if (x == y) continue; // warn here

					// check if parents exists both in old_new
					if (old_new_ids.count(s_parent) &&
						old_new_ids.count(e_parent))
					{
						auto start_pin = create_pin_id(x, s_index, old_new_ids[old_start]);
						auto end_pin = create_pin_id(y, e_index, old_new_ids[old_end]);
						add_link(start_pin, end_pin); // warn here if fail
					}
				}
				reader.EndArray();
			}
		}
		reader.EndObject();

	}

	bool graph::serialize(const tpath& folder, bool save_as, const string& filename) const
	{
		string actual_name;
		if (save_as)
			actual_name = filename;
		else
			actual_name = name;

		// check if filename is valid to save
		if (!Serializer::SaveJson(folder / (actual_name + ".json"), ""))
			throw graph_exception(actual_name + ".json" + ": Invalid filename!");

		Writer writer;
		writer.StartObject();
		{
			// meta part
			{
				writer.StartMeta();
				{
					writer.Member("Type", "Graph");
				}
				writer.EndMeta();
			}

			// nodes part
			{
				writer.StartArray("Nodes");
				for (auto& [id, node_p] : nodes)
				{
					if (!node_p) continue;

					std::string metacategory = magic_enum::enum_name(node_p->get_category()).data();
					std::string metatype = node_p->get_type_string();

					writer.StartObject();
					{
						writer.StartMeta();
						{
							writer.Member("Category", metacategory);
							writer.Member("Type", metatype);
						}
						writer.EndMeta();
						node_p->serialize(writer);
					}
					writer.EndObject();
				}
				writer.EndArray();
			}

			// links part
			{
				writer.StartArray("Links");
				for (auto [start, end] : get_links())
				{
					writer.StartObject();
					{
						writer.Member("Start", start);
						writer.Member("End", end);
					}
					writer.EndObject();
				}
				writer.EndArray();
			}
		}
		writer.EndObject();

		// if we are saving as, we rename the current graph
		if (save_as) name = actual_name;

		return Serializer::SaveJson(folder / (actual_name + ".json"), writer.GetString());
	}
	void graph::clear()
	{
		nodes.clear();
		links.clear();
	}
}
