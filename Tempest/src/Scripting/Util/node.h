#pragma once

#include "pin.h"
#include "script.h"
#include "TMath.h"
#include "Util.h"
#include "ECS/Entity.h"
#include "Util/algorithm.h"


/**
* @brief See line 200 for maintanance instructions
*
*/
namespace Tempest
{
	/**
	 * @brief Node category type. Umbrella category for a node type.
	 */
	enum struct category_type;

	//forward declare
	class Instance;

	class node_exception : public std::exception
	{
	public:
		node_exception(const string& err_msg = "node exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Node object owned by a graph. Contains information
	 * on how to build an underlying script.
	 */
	class node
	{
	public:
		friend class graph;

		node(category_type _category) :
			id{ idgen::generate() }, category{ _category } {}

		node(const node&) = delete;
		node& operator=(const node&) = delete;
		node(node&&) = default;
		node& operator=(node&&) = default;
		virtual ~node() = 0 {}

		/**
		 * @brief Gets the type information of the node. Used for
		 * serialization. Information is required on how to build the node.
		 */
		virtual string get_type_string() = 0;


		/**
		 * @brief Creates a script based on the node's type information.
		 */
		virtual script* create_script(Entity entity, Instance& srm) = 0;

		/**
		 * @brief Creates a script based on the node's type information.
		 */
		script* create_script_pack(Entity entity, Instance& srm);

		/**
		 * @brief Serializing function. Writes to the writer object.
		 */
		Writer& serialize(Writer& writer) const;

		/**
		 * @brief Returns input pins.
		 */
		const tvector<input_pin>& get_inputs() const { return inputs; }
		/**
		 * @brief Returns output pins.
		 */
		const tvector<output_pin>& get_outputs() const { return outputs; }

		/**
		 * @brief Get number of input flows.
		 */
		size_t get_num_input_flows() const { return flow_inputs; }
		/**
		 * @brief Get number of output flows.
		 */
		size_t get_num_output_flows() const { return flow_outputs; }
		/**
		 * @brief Get number of input values.
		 */
		size_t get_num_input_values() const { return non_flow_inputs; }
		/**
		 * @brief Get number of output values.
		 */
		size_t get_num_output_values() const { return non_flow_outputs; }
		/**
		 * @brief Get total number of input pins.
		 */
		size_t get_num_inputs() const { return inputs.size(); }
		/**
		 * @brief Get total number of output pins.
		 */
		size_t get_num_outputs() const { return outputs.size(); }

		/**
		 * @brief Get node id.
		 */
		node_id_t get_id() const { return id; }
		/**
		 * @brief Get name of node.
		 */
		const string& get_name() const { return name; }
		/**
		 * @brief Get node category.
		 */
		category_type get_category() const { return category; }

		input_pin* get_input_pin(size_t index);

		output_pin* get_output_pin(size_t index);


		operator node_id_t() const { return id; }

	protected:
		/**
		 * @brief Adds an input pin to the node.
		 */
		pin_id_t add_input(pin_type _type, const string& _name);
		/**
		 * @brief Adds an ouput pin to the node.
		 */
		pin_id_t add_output(pin_type _type, const string& _name);
		/**
		 * @brief Sets the name of the node i.e what name it will be rendered as.
		 */
		void set_name(const string& _name) { name = _name; }

		node_id_t id;
		string name;
		category_type category;

		tvector<input_pin> inputs;
		tvector<output_pin> outputs;

	private:
		Writer& serialize_parent(Writer& writer) const;

		size_t flow_inputs = 0;
		size_t flow_outputs = 0;
		size_t non_flow_inputs = 0;
		size_t non_flow_outputs = 0;

	public:
		vec2 size;
		vec2 position;

	};


	/**
	 * @brief Macros. Ugly but does the job.
	 * Use anonymous namespace to hide ;)
	 */
	namespace {
#define STANDARD_NODE_STUFF(NodeName, NodeCategory, ...)					\
public:																		\
	enum struct inner_type { __VA_ARGS__, END };							\
	NodeName(inner_type t) : node{category_type::NodeCategory}, type{t} {}	\
	inner_type get_type() const { return type; }							\
	void set_type(inner_type _type) { type = _type; }						\
	static node_ptr create_node(const std::string& info);					\
	string get_type_string() override;										\
	script* create_script(Entity entity, Instance& srm) override;			\
private:																	\
	inner_type type;														\

#define START_NODE(NodeName, NodeCategory, ...)								\
class NodeName : public node												\
{																			\
STANDARD_NODE_STUFF(NodeName, NodeCategory, __VA_ARGS__)					\

#define END_NODE };															\

#define DEFINE_NODE(NodeName, NodeCategory, ...)							\
START_NODE(NodeName, NodeCategory, __VA_ARGS__)								\
END_NODE																	\


#define NODE_CASE(NodeName, NodeCategory)									\
case category_type::NodeCategory:											\
{																			\
	return NodeName::create_node(type);										\
}																			\
	break																	\


#define NODE_SWITCH_START \
		static inline std::unique_ptr<node> create_helper(					\
	category_type t, const string& type) {									\
	switch (t)																\
	{																		\

#define NODE_SWITCH_END \
	case Tempest::category_type::END: [[fallthrough]];						\
	default:																\
		return nullptr;														\
	}																		\
}																			\

	}

	/**
	* @brief
	*   =================================================================
	*				   NODEs MAINTENANCE INSTRUCTIONS
	*   =================================================================
	*
	*/

	enum struct category_type
	{
		Cast, Variable, Dice, Arithmetic, GetStat, SetStat, GetMainStat, 
		Resolution, System
		
		,Group, Trig,
		Random, Numerical, Constants, Logic, Vector,
		Utility, Entity,
		END, test, util,
	};


	DEFINE_NODE(test_node, test, testing1, testing2, all);
	DEFINE_NODE(CastNode, Cast, _cannot_be_empty);
	DEFINE_NODE(VariableNode, Variable, LocalGet, LocalSet, GlobalGet, GlobalSet);
	DEFINE_NODE(DiceNode, Dice, D4, D6, D8, D10, D12, D20);
	DEFINE_NODE(ArithmeticNode, Arithmetic, Plus, Minus, Multiply, Divide);
	DEFINE_NODE(GetStatNode, GetStat, _cannot_be_empty);
	DEFINE_NODE(SetStatNode, SetStat, _cannot_be_empty);
	DEFINE_NODE(GetMainStatNode, GetMainStat, _cannot_be_empty);
	DEFINE_NODE(ResolutionNode, Resolution, Start, Resolve, Attacking, Defending);
	DEFINE_NODE(SystemNode, System, Input, Output);
	DEFINE_NODE(UtilNode, util, Print, In);

	NODE_SWITCH_START

		/* BELOW THIS PLEASE*/

		NODE_CASE(test_node, test);
		NODE_CASE(VariableNode, Variable);
		NODE_CASE(DiceNode, Dice);
		NODE_CASE(ArithmeticNode, Arithmetic);
		NODE_CASE(GetStatNode, GetStat);
		NODE_CASE(SetStatNode, SetStat);
		NODE_CASE(GetMainStatNode, GetMainStat);
		NODE_CASE(ResolutionNode, Resolution);
		NODE_CASE(SystemNode, System);
		NODE_CASE(UtilNode, util);

		/* ABOVE THIS PLEASE */

	NODE_SWITCH_END



	/**
		* @brief Factory function for creating nodes. Takes in a category string
		* and inner type string and creates a node.
		* @throw node_exception if node is badly created
		* @return node_ptr Node as a unique pointer.
		*/
	node_ptr CreateNode(const string& category, const string& type);

	/**
	 * @brief Factory function for creating nodes. Takes in a category type
	 * and inner type string and creates a node.
	 * @throw node_exception if node is badly created
	 * @return node_ptr Node as a unique pointer.
	 */
	node_ptr CreateNode(category_type category, const string& type);
}