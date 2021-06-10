#pragma once

#include "var_data.h"
#include "internal.h"
#include "Util.h"


namespace Tempest
{
	/**
	 * @brief Pin object owned by a parent node
	 */
	class input_pin
	{
	public:
		friend class graph;

		input_pin(pin_id_t _id, pin_type _type, string _name) :
			id{ _id }, type{ _type }, name{ std::move(_name) } 
		{
			// set var_data
			switch (type)
			{
			case Tempest::pin_type::Bool:
				break;
			case Tempest::pin_type::Byte:
				break;
			case Tempest::pin_type::Int:
				break;
			case Tempest::pin_type::Int64:
				break;
			case Tempest::pin_type::Float:
				break;
			case Tempest::pin_type::String:
				break;
			case Tempest::pin_type::Vec2:
				break;
			case Tempest::pin_type::Vec3:
				break;
			case Tempest::pin_type::Vec4:
				break;
			case Tempest::pin_type::Entity:
				break;
			case Tempest::pin_type::Flow: [[fallthrough]];
			case Tempest::pin_type::Vector: [[fallthrough]];
			case Tempest::pin_type::END: [[fallthrough]];
			default:
				LOG_CRITICAL("Bad Pin Type!");
				break;
			}
		}

		input_pin(const input_pin&) = delete;
		input_pin& operator=(const input_pin&) = delete;
		input_pin(input_pin&&) = default;
		input_pin& operator=(input_pin&&) = default;


		static constexpr bool value = true;
		static constexpr bool is_input = value;
		static constexpr bool is_output = !value;

		/**
		 * @brief Gets pin id.
		 */
		pin_id_t get_id() const { return id; }
		/**
		 * @brief Gets pin type.
		 */
		pin_type get_type() const { return type; }
		/**
		 * @brief Gets name of pin.
		 */
		const string& get_name() const { return name; }
		/**
		 * @brief Returns true if pin is linked.
		 */
		bool is_linked() const { return linked; }

		operator pin_id_t() const { return id; }

	private:
		pin_id_t id;
		pin_type type;
		string name;
		mutable bool linked = false;

	public:

		// extra variable type for input pins for in node 
		// default variables
		mutable var_data default_var;
	};
	/**
	 * @brief Pin object owned by a parent node
	 */
	class output_pin
	{
	public:
		friend class graph;

		output_pin(pin_id_t _id, pin_type _type, string _name) :
			id{ _id }, type{ _type }, name{ std::move(_name) } {}

		output_pin(const output_pin&) = delete;
		output_pin& operator=(const output_pin&) = delete;
		output_pin(output_pin&&) = default;
		output_pin& operator=(output_pin&&) = default;


		static constexpr bool value = false;
		static constexpr bool is_input = value;
		static constexpr bool is_output = !value;

		/**
		 * @brief Gets pin id.
		 */
		pin_id_t get_id() const { return id; }
		/**
		 * @brief Gets pin type.
		 */
		pin_type get_type() const { return type; }
		/**
		 * @brief Gets name of pin.
		 */
		const string& get_name() const { return name; }
		/**
		 * @brief Returns true if pin is linked.
		 */
		bool is_linked() const { return linked; }

		operator pin_id_t() const { return id; }

	private:
		pin_id_t id;
		pin_type type;
		string name;
		mutable bool linked = false;
	};
}
