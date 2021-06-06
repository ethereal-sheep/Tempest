#pragma once

#include "Util/pin.h"
#include "TMath.h"
#include "Util.h"


#define STANDARD_NODE_STUFF(NodeName, ...)									\
public:																		\
	enum struct inner_type { __VA_ARGS__, END };							\
	NodeName();																\
	inner_type get_type() const { return _type; }							\
	void set_type(inner_type type) { _type = type; }						\
	static std::unique_ptr<Node> create_node(const std::string& info);		\
	std::string get_type_string() override;									\
	NodeScript::INodeScript* create_script(ECS::EntityID id) override;		\
	Writer& serialize_child(Writer& writer) override;						\
private:																	\
	inner_type _type;														\

#define START_NODE(NodeName, ...)											\
class NodeName : public Node												\
{																			\
STANDARD_NODE_STUFF(NodeName, __VA_ARGS__)									\

#define END_NODE };															\

#define DEFINE_NODE(NodeName, ...)											\
START_NODE(NodeName, __VA_ARGS__)											\
END_NODE																	\

namespace Tempest
{
	using node_id_t = id_t;


	enum struct category_type
	{
		Cast, Variable, Group, Arithmetic, Trig,
		Random, Numerical, Constants, Logic, Vector,
		Utility, Entity,
		END
	};


	class Node
	{
	public:
		Node(node_id_t _id, category_type _category) :
			id{ _id }, category{ _category } {}

		virtual ~Node() = 0 {}
		virtual const string& get_type_string() = 0;
		virtual Writer& serialize_child(Writer& writer) = 0;

		Writer& serialize(Writer& writer);

		tvector<input_pin>& get_inputs() { return inputs; }
		tvector<output_pin>& get_outputs() { return outputs; }

		node_id_t get_id() const { return id; }
		const string& get_name() const { return name; }
		category_type get_category() const { return category; }

	protected:
		pin_id_t add_input(pin_type _type, const string& _name);
		pin_id_t add_output(pin_type _type, const string& _name);
		void set_name(const string& _name) { name = _name; }
		Writer& serialize_parent(Writer& writer);

		node_id_t id;
		string name;
		category_type category;

		tvector<input_pin> inputs;
		tvector<output_pin> outputs;

	public:
		vec2 size;
		vec2 position;
	};
}