#pragma once
#include <memory>
#include <fstream>

namespace Tempest
{
	class Reader
	{
		Reader(const Reader&);

	public: 
		Reader(std::ifstream& stream);
		Reader(const char* json);
		~Reader();

		Reader& StartObject();
		Reader& StartObject(const char* identifier);
		Reader& EndObject();
		Reader& StartMeta(const char* identifier = "Metadata");
		Reader& EndMeta();

		Reader& Member(const char* identifier);
		template <typename TData>
		bool Member(const char* identifier, TData& data);
		bool HasMember(const char* identifier);
		bool HasError() const;

		Reader& StartArray(size_t* size = nullptr);
		Reader& StartArray(const char* identifier, size_t* size = nullptr);
		Reader& EndArray();

		template <typename TArray>
		Reader& Array(const char* identifier, TArray& arr);
		template <typename TVector>
		Reader& Vector(const char* identifier, TVector& vec);
		template <typename TMap>
		Reader& Map(const char* identifier, const char* key_identifier, const char* value_identifier, TMap& map);
		
		//Deprecate
		/*template <typename TVariant>
		Reader& Variant(const char* identifier, TVariant& var);
		template <typename TVariant>
		Reader& Variant(TVariant& var);*/

		Reader& operator& (int& i);
		Reader& operator& (bool& b);
		Reader& operator& (float& f);
		Reader& operator& (double& d);
		Reader& operator& (unsigned& u);
		Reader& operator& (int64_t& i64);
		Reader& operator& (uint64_t& u64);
		Reader& operator& (std::string& s);
		Reader& operator&(std::filesystem::path& p);

		//! Can only be used on non-abstract classes
		template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
		Reader& operator& (std::unique_ptr<T>& ptr);
		template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
		Reader& operator& (std::shared_ptr<T>& ptr);

		//! Created mostly for glm vectors
		template <typename TVec>
		Reader& vec2(const char* identifier, TVec& vec);
		template <typename TVec>
		Reader& vec3(const char* identifier, TVec& vec);
		template <typename TVec>
		Reader& vec4(const char* identifier, TVec& vec);

		constexpr static bool IsSaving() { return false; }
		constexpr static bool IsLoading() { return true; }

	private:

		struct Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}

#include "Reader.inl"