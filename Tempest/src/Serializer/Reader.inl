#include "Reader.h"
#include <variant>

namespace Tempest
{
	template <typename... Functions>
	struct overload : Functions...
	{
		using Functions::operator()...;
		overload(Functions... functions) : Functions(functions)...{}
	};

	template<typename TData>
	bool Reader::Member(const char* identifier, TData& data)
	{
		if (HasMember(identifier))
		{
			Member(identifier);
			if constexpr (std::is_enum_v<TData>)
			{
				int i; *this & i;
				data = static_cast<TData>(i);
			}
			else
			{
				*this & data;
			}
			
			return true;
		}
		//CDN_CORE_WARN("Reader::Member - Unable to find member {0} when loading {1}", identifier, typeid(TData).name());
		return false;
	}

	template<typename TArray>
	Reader& Reader::Array(const char* identifier, TArray& arr)
	{
		size_t size = 0;
		
		if (!HasMember(identifier))
		{
			//CDN_CORE_WARN("Reader::Array - Unable to find identifier {0}", identifier);
		}
		
		Member(identifier);
		StartArray(&size);
		if (size == arr.size())
			for (size_t idx = 0; idx < arr.size(); ++idx)
				*this & (arr[idx]);
		return EndArray();
	}

	template<typename TVector>
	Reader& Reader::Vector(const char* identifier, TVector& vec)
	{
		size_t size = 0;
		if (!HasMember(identifier))
		{
			//CDN_CORE_WARN("Reader::Vector - Unable to find identifier {0}", identifier);
			return *this;
		}

		Member(identifier);
		StartArray(&size);
		vec.resize(size);
		for (auto idx = 0; idx < vec.size(); ++idx)
			*this & (vec[idx]);
		return EndArray();
	}

	template<typename TMap>
	inline Reader& Reader::Map(const char* identifier, const char* key_identifier, const char* value_identifier, TMap& map)
	{
		if (!HasMember(identifier))
		{
			//CDN_CORE_WARN("Reader::Map - Unable to find identifier {0}", identifier);
			return *this;
		}
		size_t size = 0;
		Member(identifier);
		StartArray(&size);
		for (size_t idx = 0; idx < size; ++idx)
		{
			typename TMap::key_type key;
			typename TMap::mapped_type value;
			StartObject();
			if (!Member(key_identifier, key))
			{
				//CDN_CORE_WARN("Reader::Map - Unable to find key identifier {0}", key_identifier);
				continue;
			}
			if (!Member(value_identifier, value))
			{
				//CDN_CORE_WARN("Reader::Map - Unable to find value identifier {0}", value_identifier);
				continue;
			}
			if (!(map.try_emplace(key, value)).second)
				//CDN_CORE_WARN("Reader::Map - Unable to insert {0} and {1} into map", key_identifier, value_identifier);
			EndObject();
		}
		return EndArray();
	}

	template<typename TVariant>
	Reader& Reader::Variant(const char* identifier, TVariant& var)
	{
		Member(identifier);
		return Variant(var);
	}

	template<typename TVariant>
	Reader& Reader::Variant(TVariant& var)
	{
		std::visit(overload(
			[&](uint8_t& c) { 
				unsigned data;
				*this & data;
				c = static_cast<char>(data);
			},
			[&](auto& obj) { *this & obj; }
		), var);
		return *this;
	}

	template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
	Reader& Reader::operator& (std::unique_ptr<T>& ptr)
	{
		ptr = std::make_unique<T>();
		*this & *ptr;
		return *this;
	}

	template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
	Reader& Reader::operator& (std::shared_ptr<T>& ptr)
	{
		ptr = std::make_shared<T>();
		*this & *ptr;
		return *this;
	}


	template<typename TFirst, typename TSecond>
	Reader& Reader::operator& (std::pair<TFirst, TSecond>& pair)
	{
		*this& pair.first;
		*this& pair.second;
		return *this;
	}


	template <typename TVec>
	Reader& Reader::vec2(const char* identifier, TVec& vec)
	{
		Member(identifier);
		StartArray();
		*this& vec.x;
		*this& vec.y;
		return EndArray();
	}

	template <typename TVec>
	Reader& Reader::vec3(const char* identifier, TVec& vec)
	{
		Member(identifier);
		StartArray();
		*this& vec.x;
		*this& vec.y;
		*this& vec.z;
		return EndArray();
	}

	template <typename TVec>
	Reader& Reader::vec4(const char* identifier, TVec& vec)
	{
		Member(identifier);
		StartArray();
		*this& vec.x;
		*this& vec.y;
		*this& vec.z;
		*this& vec.w;
		return EndArray();
	}
}