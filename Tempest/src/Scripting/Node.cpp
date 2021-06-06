#include "Node.h"
#include "Util/static_id_generator.h"

namespace Tempest
{
	Writer& Node::serialize(Writer& writer)
	{
		serialize_parent(writer);
		serialize_child(writer);
		return writer;
	}

	Writer& Node::serialize_parent(Writer& writer)
	{
		writer.Member("name", name);
		writer.Member("Size", size);
		writer.Member("Position", position);
		return writer;
	}

	pin_id_t Node::add_input(pin_type _type, const string& _name)
	{
		pin_id_t iid = idgen::generate();
		inputs.emplace_back(iid, _type, _name);
		return iid;
	}
	pin_id_t Node::add_output(pin_type _type, const string& _name)
	{
		pin_id_t oid = idgen::generate();
		outputs.emplace_back(oid, _type, _name);
		return oid;
	}
}