#pragma once
#include <variant>
#include "TMath.h"
#include "internal.h"

namespace Tempest
{
	class var_data
	{
	public:
		using Data = std::variant<bool, uint8_t, int, int64_t, float, string, vec2, vec3, vec4, uint32_t>;

		template <typename TData = bool>
		var_data(TData data) { set<TData>(data); }
		var_data() { set(true); }

		auto& GetData() { return data; }
		auto& GetType() const { return type; }

		var_data& set(bool _data)
		{
			type = pin_type::Bool;
			data = _data;

			return *this;
		}

		var_data& set(uint8_t _data)
		{
			type = pin_type::Byte;
			data = _data;

			return *this;
		}

		var_data& set(int _data)
		{
			type = pin_type::Int;
			data = _data;

			return *this;
		}

		var_data& set(int64_t _data)
		{
			type = pin_type::Int64;
			data = _data;

			return *this;
		}

		var_data& set(float _data)
		{
			type = pin_type::Float;
			data = _data;

			return *this;
		}

		var_data& set(std::string _data)
		{
			type = pin_type::String;
			data = std::move(_data);

			return *this;
		}

		var_data& set(uint32_t _data)
		{
			type = pin_type::Entity;
			data = _data;

			return *this;
		}

		var_data& set(vec2 _data)
		{
			type = pin_type::Vec2;
			data = _data;

			return *this;
		}

		var_data& set(vec3 _data)
		{
			type = pin_type::Vec2;
			data = _data;

			return *this;
		}

		var_data& set(vec4 _data)
		{
			type = pin_type::Vec4;
			data = _data;

			return *this;
		}

		template <typename Archive>
		friend Archive& operator& (Archive& ar, var_data& var)
		{
			ar.StartObject();
			ar.Member("VarType", var.type);

			if constexpr (Archive::IsLoading())
			{
				switch (var.type)
				{
				case pin_type::Bool:	var.data = bool{}; break;
				case pin_type::Byte:	var.data = uint8_t{}; break;
				case pin_type::Int:		var.data = int{}; break;
				case pin_type::Int64:	var.data = int64_t{}; break;
				case pin_type::Float:	var.data = float{}; break;
				case pin_type::String:	var.data = string{}; break;
				case pin_type::Vec2:	var.data = vec2{}; break;
				case pin_type::Vec3:	var.data = vec3{}; break;
				case pin_type::Vec4:	var.data = vec4{}; break;
				default:
					__debugbreak();
				}
			}

			ar.Variant("VarData", var.data);
			ar.EndObject();
			return ar;
		}
	private:
		pin_type type = pin_type::Bool;
		Data data;
	};


	
}