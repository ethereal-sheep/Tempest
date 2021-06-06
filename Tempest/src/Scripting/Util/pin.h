#pragma once

#include "link.h"
#include "Util.h"

namespace Tempest
{

	enum struct pin_type
	{
		Flow,
		Bool, Byte, Int, Int64, Float, String, Vec2, Vec3, Vec4,
		END,
		Entity, Vector
	};

	template <bool input>
	class pin
	{
	public:

		pin(pin_id_t _id, pin_type _type, string _name) :
			id{ _id }, type{ _type }, name{ std::move(_name) } {}

		pin(const pin&) = delete;
		pin& operator=(const pin&) = delete;
		pin(pin&&) = delete;
		pin& operator=(pin&&) = delete;


		static constexpr bool value = input;
		static constexpr bool is_input = input;
		static constexpr bool is_output = !input;

		template <typename Archive>
		friend Archive& operator&(Archive& ar, pin& pin)
		{
			string type = Archive::IsLoading() ? "" : magic_enum::enum_name(pin.type).data();
			ar.StartObject();
			ar.Member("id", pin.id);
			ar.Member("type", type);
			ar.Member("name", pin.name);
			return ar.EndObject();
		}

		pin_id_t get_id() const { return id; }
		pin_type get_type() const { return type; }
		const string& get_name() const { return name; }

		operator pin_id_t() const { return id; }

	private:
		pin_id_t id;
		pin_type type;
		string name;

	};

	using input_pin = pin<true>;
	using output_pin = pin<false>;
}
