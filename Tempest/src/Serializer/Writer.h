/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <memory>

namespace Tempest
{
    class Writer
    {
        Writer(const Writer&);

    public:
        Writer();
        ~Writer();

        const char* GetString() const;
        Writer& StartObject();
        Writer& StartObject(const char* identifier);
        Writer& EndObject();
        Writer& StartMeta(const char* identifier = "Metadata");
        Writer& EndMeta();

        Writer& Key(const char* identifier);
        Writer& Member(const char* identifier);
        bool HasMember(const char*);
        template <typename TData>
        bool Member(const char* identifier, TData& data);

        void Reset();

        Writer& StartArray(size_t* size = nullptr);
        Writer& StartArray(const char* identifier, size_t* size = nullptr);
        Writer& EndArray();

        template <typename TArray>
        Writer& Array(const char* identifier, TArray& arr);
        template <typename TVector>
        Writer& Vector(const char* identifier, TVector& vec);
        template <typename TMap>
        Writer& Map(const char* identifier, const char* key_identifier, const char* value_identifier, TMap& map);
        
        template <typename TVariant>
        Writer& Variant(const char* identifier, TVariant& var);
        template <typename TVariant>
        Writer& Variant(TVariant& var);

        Writer& operator& (const int b);
        Writer& operator& (const bool b);
        Writer& operator& (const char c);
        Writer& operator& (const char* s);
        Writer& operator& (const float f);
        Writer& operator& (const double b);
        Writer& operator& (const int64_t b);
        Writer& operator& (const uint64_t b);
        Writer& operator& (const unsigned b);
        Writer& operator& (const std::string& s);
        Writer& operator& (const std::filesystem::path& p);

        //! Can only be used on non-abstract classes
        template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
        Writer& operator& (std::unique_ptr<T>& ptr);
        template <typename T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
        Writer& operator& (std::shared_ptr<T>& ptr);
        template<typename TFirst, typename TSecond>
        Writer& operator& (const std::pair<TFirst, TSecond>);

        //! Created mostly for glm vectors
        template <typename TVec>
        Writer& vec2(const char* identifier, const TVec& vec);
        template <typename TVec>
        Writer& vec3(const char* identifier, const TVec& vec);
        template <typename TVec>
        Writer& vec4(const char* identifier, const TVec& vec);

        Writer& SetNull();
        
        constexpr static bool IsSaving() { return true; }
        constexpr static bool IsLoading() { return false; }


    private:
        struct Impl;
        std::unique_ptr<Impl> m_Impl;
    };
}

#include "Writer.inl"