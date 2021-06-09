#pragma once

#include "var_data.h"
#include "internal.h"
#include "Util.h"


namespace Tempest
{
	/**
	 * @brief Pin object owned by a parent node
	 */
	template <bool input>
	class pin
	{
	public:
		friend class graph;

		pin(pin_id_t _id, pin_type _type, string _name) :
			id{ _id }, type{ _type }, name{ std::move(_name) } {}

		pin(const pin&) = delete;
		pin& operator=(const pin&) = delete;
		pin(pin&&) = default;
		pin& operator=(pin&&) = default;


		static constexpr bool value = input;
		static constexpr bool is_input = input;
		static constexpr bool is_output = !input;

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

		mutable var_data default_var;
	};

	using input_pin = pin<true>;
	using output_pin = pin<false>;
}
