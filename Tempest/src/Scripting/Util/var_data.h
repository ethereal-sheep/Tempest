#pragma once
#include <variant>
#include "TMath.h"
#include "internal.h"
#include "Util/type_traits.h"

namespace Tempest
{

	/**
	 * @brief Variable data class. Stores a variable data based on pin_type.
	 */
	class var_data
	{
	public:
		using Data = std::variant<bool, uint8_t, int, int64_t, float, string, vec2, vec3, vec4, uint32_t>;

		template <typename T>
		static constexpr bool is_member = is_type_list_member<T, Data>();

		/**
		 * @brief Template constructor
		 */
		template <typename T = bool>
		var_data(T data) { set<T>(data); }


		/**
		 * @brief Gets name of graph
		 */
		var_data(pin_type t) : type(t)
		{
			switch (t)
			{
			case pin_type::Bool:	variable_data = bool{}; break;
			case pin_type::Byte:	variable_data = uint8_t{}; break;
			case pin_type::Int:		variable_data = int{}; break;
			case pin_type::Int64:	variable_data = int64_t{}; break;
			case pin_type::Float:	variable_data = float{}; break;
			case pin_type::String:	variable_data = string{}; break;
			case pin_type::Vec2:	variable_data = vec2{}; break;
			case pin_type::Vec3:	variable_data = vec3{}; break;
			case pin_type::Vec4:	variable_data = vec4{}; break;
			default:
				variable_data = bool{};
				type = pin_type::Bool;
				break;
			}
		}

		var_data() { set(true); }

		/**
		 * @brief Gets a reference to the underlying variable.
		 * @throw Throws bad_variant_access if the variable type is wrong.
		 */
		template <typename T, typename = std::enable_if_t<is_member<T>>>
		T& get() { return std::get<T>(variable_data); }
		template <typename T, typename = std::enable_if_t<is_member<T>>>
		const T& get() const  { return std::get<T>(variable_data); }

		/**
		 * @brief Gets the a pointer of type T if typing is correct. Returns
		 * nullptr if it fails.
		 */
		template <typename T, typename = std::enable_if_t<is_member<T>>>
		T* get_if()
		{
			if (try_get<T>())
				return &get<T>();
			return nullptr;
		}
		template <typename T, typename = std::enable_if_t<is_member<T>>>
		const T* get_if() const
		{
			if (try_get<T>())
				return &get<T>();
			return nullptr;
		}

		/**
		 * @brief Tries to get the variable of type T. Returns true
		 * if variable of type T is stored in the variable data class.
		 */
		template <typename T>
		bool try_get() const
		{
			try
			{
				auto temp [[maybe_unused]] = std::get<T>(variable_data);
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

		/**
		 * @brief Gets underlying data variant
		 */
		auto& data(){ return variable_data; }

		/**
		 * @brief Gets type of variable
		 */
		pin_type get_type() const { return type; }

		template <typename T>
		var_data& set(T _data)
		{
			type = pin_type::Bool;
			variable_data = static_cast<bool>(_data);

			return *this;
		}
		template <>
		var_data& set<uint8_t>(uint8_t _data)
		{
			type = pin_type::Byte;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<char>(char _data)
		{
			type = pin_type::Byte;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<int>(int _data)
		{
			type = pin_type::Int;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<int64_t>(int64_t _data)
		{
			type = pin_type::Int64;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<float>(float _data)
		{
			type = pin_type::Float;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<double>(double _data)
		{
			type = pin_type::Float;
			variable_data = static_cast<float>(_data);

			return *this;
		}
		template <>
		var_data& set<string>(std::string _data)
		{
			type = pin_type::String;
			variable_data = std::move(_data);

			return *this;
		}
		template <>
		var_data& set<const char*>(const char* _data)
		{
			type = pin_type::String;
			variable_data = string(_data);

			return *this;
		}
		template <>
		var_data& set<uint32_t>(uint32_t _data)
		{
			type = pin_type::Entity;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<size_t>(size_t _data)
		{
			type = pin_type::Entity;
			variable_data = static_cast<uint32_t>(_data);

			return *this;
		}
		template <>
		var_data& set<vec2>(vec2 _data)
		{
			type = pin_type::Vec2;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<vec3>(vec3 _data)
		{
			type = pin_type::Vec3;
			variable_data = _data;

			return *this;
		}
		template <>
		var_data& set<vec4>(vec4 _data)
		{
			type = pin_type::Vec4;
			variable_data = _data;

			return *this;
		}

		/**
		 * @brief Serialization function.
		 */
		template <typename Archive>
		friend Archive& operator& (Archive& ar, var_data& var)
		{
			ar.StartObject();
			ar.Member("VarType", var.type);

			if constexpr (Archive::IsLoading())
			{
				switch (var.type)
				{
				case pin_type::Bool:	var.variable_data = bool{}; break;
				case pin_type::Byte:	var.variable_data = uint8_t{}; break;
				case pin_type::Int:		var.variable_data = int{}; break;
				case pin_type::Int64:	var.variable_data = int64_t{}; break;
				case pin_type::Float:	var.variable_data = float{}; break;
				case pin_type::String:	var.variable_data = string{}; break;
				case pin_type::Vec2:	var.variable_data = vec2{}; break;
				case pin_type::Vec3:	var.variable_data = vec3{}; break;
				case pin_type::Vec4:	var.variable_data = vec4{}; break;
				default:
					var.variable_data = bool{};
					var.type = pin_type::Bool;
					break;
				}
			}

			ar.Variant("VarData", var.variable_data);
			ar.EndObject();
			return ar;
		}
	private:
		pin_type type = pin_type::Bool;
		Data variable_data;
	};


	
}