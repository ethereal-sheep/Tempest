#include "Node.h"
#include "Util/static_id_generator.h"

namespace Tempest
{
	Writer& Node::serialize(Writer& writer)
	{
		serialize_parent(writer);
		serialize_child(writer);
	}

	Writer& Node::serialize_parent(Writer& writer)
	{
		writer.Member("name", name);
		writer.Member("Size", size);
		writer.Member("Position", position);
		return writer;
	}

	pin_id_t Node::add_input(const string& _name, pin_type _type)
	{
		pin_id_t id = idgen::generate();
		inputs.emplace_back(id, _name, _type);
		return id;
	}
	pin_id_t Node::add_output(const string& _name, pin_type _type)
	{
		pin_id_t id = idgen::generate();
		outputs.emplace_back(id, _name, _type);
		return id;
	}
}