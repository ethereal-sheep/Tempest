#include "graph.h"
#include "Util.h"

namespace Tempest
{
	

	graph::graph(const string& _name, graph_type _type, m_resource* mem) :
		name{ _name }, type{ _type }, nodes{ mem }, links{ mem }
	{
		switch (type)
		{
		case Tempest::graph_type::regular:
			break;
		case Tempest::graph_type::conflict:
		{
			// attacking/defending entities
			auto atk = add_var("Attacking", pin_type::Int64);
			auto def = add_var("Defending", pin_type::Int64);
			LOG_ASSERT(atk);
			LOG_ASSERT(def);

			// atk/def systems 
			// temp: systems should be defined in character sheet (atk, def)
			auto atk_sys = add_var("Attacking_System", pin_type::Int64);
			auto def_sys = add_var("Defending_System", pin_type::Int64);
			auto res_sys = add_var("Resolving_System", pin_type::Int64);
			LOG_ASSERT(atk_sys);
			LOG_ASSERT(def_sys);
			LOG_ASSERT(res_sys);

			// start nodes

			// start event
			auto start = add_node(ConflictNode::create_node("Start"));

			// start links to System 1
			// attacking calculates the value and passes it on
			auto system1 = add_node(ConflictNode::create_node("Attacking"));

			// attacking links to defending
			// defending calculates the value and passes it on
			auto system2 = add_node(ConflictNode::create_node("Defending"));

			// resolve attacking and defending
			auto resolve = add_node(ConflictNode::create_node("Resolve"));


			LOG_ASSERT(start);
			LOG_ASSERT(system1);
			LOG_ASSERT(system2);
			LOG_ASSERT(resolve);

			// testing only
			auto output = add_node(ActionNode::create_node("Output"));
			LOG_ASSERT(output);
			auto var = add_var("Output", pin_type::Int);
			LOG_ASSERT(var);
		}
			break;
		case Tempest::graph_type::action:
		{
			// for output
			auto var = add_var("Output", pin_type::Int);
			auto owner = add_var("Owner", pin_type::Int64);

			auto input = add_node(ActionNode::create_node("Input"));
			auto output = add_node(ActionNode::create_node("Output"));

			LOG_ASSERT(var);
			LOG_ASSERT(owner);
			LOG_ASSERT(input);
			LOG_ASSERT(output);

			output->position.x = 300.f;

			auto flow = add_link(
				input->get_output_pin(0)->get_id(),
				output->get_input_pin(0)->get_id()
			);

			auto i = add_link(
				input->get_output_pin(1)->get_id(),
				output->get_input_pin(1)->get_id()
			);

			LOG_ASSERT(flow);
			LOG_ASSERT(i);

		}
			break;
		case Tempest::graph_type::resolution:
		{
			
		}
			break;
		default:
			break;
		}
	}


	graph::graph(const tpath& graph_file, m_resource* mem) :
		name{ graph_file.stem().string() }, nodes{ mem }, links{ mem }
	{
		Serializer serializer;
		string json = serializer.GetJson(graph_file);

		Reader reader(json.c_str());
		if (reader.HasError())
			throw graph_exception(graph_file.filename().string() + ": File cannot be opened!");

		filepath = graph_file;
		filepath.remove_filename();

		_deserialize(reader);

	}

	void graph::serialize() const
	{
		if(filepath.empty())
			throw graph_exception(name + ": Graph not saved before!");

		auto new_path = get_full_path();
		new_path.replace_extension(".json");

		// check if filename is valid to save
		if (!Serializer::SaveJson(new_path, ""))
			throw graph_exception(new_path.string() + ": Invalid filename!");

		Writer writer;
		_serialize(writer);

		if (!Serializer::SaveJson(new_path, writer.GetString()))
			throw graph_exception(new_path.string() + ": File cannot be saved!");
	}

	void graph::serialize(const string& new_name) const
	{
		auto new_path = filepath / new_name;
		new_path.replace_extension(".json");

		// check if filename is valid to save
		if (!Serializer::SaveJson(new_path, ""))
			throw graph_exception(new_path.string() + ": Invalid filename!");

		Writer writer;
		_serialize(writer);

		if (!Serializer::SaveJson(new_path, writer.GetString()))
			throw graph_exception(new_path.string() + ": File cannot be saved!");

		name = new_name;
	}

	void graph::serialize(const tpath& new_folder) const
	{

		auto new_path = new_folder / name;
		new_path.replace_extension(".json");

		// check if filename is valid to save
		if (!Serializer::SaveJson(new_path, ""))
			throw graph_exception(new_path.string() + ": Invalid filename!");

		Writer writer;
		_serialize(writer);

		if (!Serializer::SaveJson(new_path, writer.GetString()))
			throw graph_exception(new_path.string() + ": File cannot be saved!");

		filepath = new_folder;
	}

	void graph::serialize(const tpath& new_folder, const string& new_name) const
	{
		auto new_path = new_folder / new_name;
		new_path.replace_extension(".json");

		// check if filename is valid to save
		if (!Serializer::SaveJson(new_path, ""))
			throw graph_exception(new_path.string() + ": Invalid filename!");

		Writer writer;
		_serialize(writer);

		if (!Serializer::SaveJson(new_path, writer.GetString()))
			throw graph_exception(new_path.string() + ": File cannot be saved!");

		name = new_name;
		filepath = new_folder;
	}

	Writer& graph::_serialize(Writer& writer) const
	{
		writer.StartObject();
		{
			// meta part
			{
				writer.StartMeta();
				{
					writer.Member("Name", name);
					writer.Member("Type", "Graph");
					writer.Member("Inner", type);
				}
				writer.EndMeta();
			}

			// variables part
			// since var_set is multipurpose, all serializations are done within the set itself
			{
				variables.serialize(writer);
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
		return writer.EndObject();
	}
	Reader& graph::_deserialize(Reader& reader)
	{
		clear();

		tmap<node_id_t, node_id_t> old_new_ids;
		reader.StartObject();
		{
			// meta part
			{
				reader.StartMeta();
				{

					reader.Member("Name", name);

					string meta;
					reader.Member("Type", meta);
					if (meta != "Graph")
						throw graph_exception(name + ": Bad Meta Member!");

					reader.Member("Inner", type);

				}
				reader.EndMeta();
			}

			// variables part
			// since var_set is multipurpose, all serializations are done within the set itself
			{
				try
				{
					variables.deserialize(reader);
				}
				catch (const std::exception& a)
				{
					throw graph_exception(name + ": Exception thrown from var_set! " + a.what());
				}
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
						if (!node)
							throw graph_exception(
								name +
								": Node creation failed! " +
								"Category: " + metacategory +
								", Type:" + metatype);


						reader.StartObject("Node");
						{
							node_id_t old_id;
							reader.Member("Id", old_id);
							reader.Member("Size", node->size);
							reader.Member("Position", node->position);

							size_t pins_size;
							reader.StartArray("PinInfo", &pins_size);
							for (size_t idx = 0; idx < pins_size; ++idx)
							{
								reader.StartObject();

								size_t id;
								var_data var;
								reader.Member("Index", id);
								reader.Member("Var", var);

								reader.EndObject();

								if (node->get_input_pin(id) &&
									var.get_type() == node->get_input_pin(id)->get_type())
								{
									node->get_input_pin(id)->default_var = var;
								}
							}
							reader.EndArray();

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

					if (x == y)
						throw graph_exception(
							name +
							": Link creation failed! Linking two input/output pins!");

					// check if parents exists both in old_new
					if (old_new_ids.count(s_parent) &&
						old_new_ids.count(e_parent))
					{
						auto start_pin = create_pin_id(x, s_index, old_new_ids[s_parent]);
						auto end_pin = create_pin_id(y, e_index, old_new_ids[e_parent]);
						if (!add_link(start_pin, end_pin))
							throw graph_exception(
								name +
								": Linking failed between " +
								std::to_string(start_pin) + " and " +
								std::to_string(end_pin));
					}
				}
				reader.EndArray();
			}
		}
		return reader.EndObject();
	}
	void graph::clear()
	{
		nodes.clear();
		links.clear();
	}
}
