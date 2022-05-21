/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Writer.h"
#include <variant>

namespace Tempest
{
	template<typename TData>
	bool Writer::Member(const char* identifier, TData& data)
	{
		Key(identifier);
		if constexpr (std::is_enum_v<TData>)
			*this & static_cast<int>(data);
		else
			*this & data;
		return true;
	}

	template<typename TArray>
	Writer& Writer::Array(const char* identifier, TArray& arr)
	{
		Key(identifier);
		StartArray();
		for (auto idx = 0; idx < arr.size(); ++idx)
			*this& (arr[idx]);
		return EndArray();
	}

	template <typename TVector>
	Writer& Writer::Vector(const char* identifier, TVector& vec)
	{
		return Array(identifier, vec);
	}

	template<typename TMap>
	inline Writer& Writer::Map(const char* identifier, const char* key_identifier, const char* value_identifier, TMap& map)
	{
		Key(identifier);
		StartArray();
		for (auto& [key, value] : map)
		{
			StartObject();
			Member(key_identifier, key);
			Member(value_identifier, value);
			EndObject();
		}
		return EndArray();
	}

	template <typename TVariant>
	Writer& Writer::Variant(const char* identifier, TVariant& var)
	{
		Key(identifier);
		return Variant(var);
	}

	template<typename TVariant>
	Writer& Writer::Variant(TVariant& var)
	{
		std::visit([&](auto& obj) { *this & obj; }, var);
		return *this;
	}

	template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
	Writer& Writer::operator& (std::unique_ptr<T>& ptr)
	{
		ptr ? *this & *ptr : SetNull();
		return *this;
	}

	template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
	Writer& Writer::operator& (std::shared_ptr<T>& ptr)
	{
		ptr ? *this & *ptr : SetNull();
		return *this;
	}
	template<typename TFirst, typename TSecond>
	Writer& Writer::operator& (const std::pair<TFirst, TSecond> pair)
	{
		StartObject();
		Member("first", pair.first);
		Member("second", pair.second);
		return EndObject();
	}

	template <typename TVec>
	Writer& Writer::vec2(const char* identifier, const TVec& vec)
	{
		Key(identifier);
		StartArray();
		*this & vec.x;
		*this & vec.y;
		return EndArray();
	}

	template <typename TVec>
	Writer& Writer::vec3(const char* identifier, const TVec& vec)
	{
		Key(identifier);
		StartArray();
		*this & vec.x;
		*this & vec.y;
		*this & vec.z;
		return EndArray();
	}

	template <typename TVec>
	Writer& Writer::vec4(const char* identifier, const TVec& vec)
	{
		Key(identifier);
		StartArray();
		*this & vec.x;
		*this & vec.y;
		*this & vec.z;
		*this & vec.w;
		return EndArray();
	}
}

