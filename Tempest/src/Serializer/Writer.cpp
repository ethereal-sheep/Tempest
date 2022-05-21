/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core.h"
#include "Writer.h"
#include "rapidjson/prettyWriter.h"
#include "rapidjson/stringbuffer.h"

namespace Tempest
{
	struct Writer::Impl
	{
		Impl() 
		: m_Buffer{}
		, m_Writer{m_Buffer}{}

		rapidjson::StringBuffer m_Buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> m_Writer;
	};

	Writer::Writer()		
	{
		m_Impl = std::make_unique<Writer::Impl>();
	}

	const char* Writer::GetString() const
	{
		return m_Impl->m_Buffer.GetString();
	}

	Writer& Writer::StartObject()
	{
		m_Impl->m_Writer.StartObject();
		return *this;
	}

	Writer& Writer::StartObject(const char* identifier)
	{
		Member(identifier);
		return StartObject();
	}

	Writer& Writer::EndObject()
	{
		m_Impl->m_Writer.EndObject();
		return *this;
	}

	Writer& Writer::StartMeta(const char* identifier)
	{
		Member(identifier);
		return StartObject();
	}

	Writer& Writer::EndMeta()
	{
		return EndObject();
	}

	Writer& Writer::Key(const char* identifier)
	{
		m_Impl->m_Writer.Key(identifier);
		return *this;
	}

	Writer& Writer::Member(const char* identifier)
	{
		m_Impl->m_Writer.String(identifier, static_cast<unsigned>(strlen(identifier)));
		return *this;
	}

	bool Writer::HasMember(const char*)
	{
		return false;
	}

	void Writer::Reset()
	{
		m_Impl->m_Buffer.Clear();
		m_Impl->m_Writer.Reset(m_Impl->m_Buffer);
	}

	Writer& Writer::StartArray(size_t*)
	{
		m_Impl->m_Writer.StartArray();
		return *this;
	}

	Writer& Writer::StartArray(const char* identifier, size_t* size)
	{
		Member(identifier);
		return StartArray(size);
	}

	Writer& Writer::EndArray()
	{
		m_Impl->m_Writer.EndArray();
		return *this;
	}

	Writer& Writer::operator &(const int b)
	{
		m_Impl->m_Writer.Int(b);
		return *this;
	}

	Writer& Writer::operator &(const bool b)
	{
		m_Impl->m_Writer.Bool(b);
		return *this;
	}

	Writer& Writer::operator &(const char c)
	{
		m_Impl->m_Writer.Uint(c);
		return *this;
	}

	Writer& Writer::operator &(const char* s)
	{
		return *this & std::string(s);
	}

	Writer& Writer::operator &(const float f)
	{
		m_Impl->m_Writer.Double(f);
		return *this;
	}

	Writer& Writer::operator &(const unsigned b)
	{
		m_Impl->m_Writer.Uint(b);
		return *this;
	}

	Writer& Writer::operator &(const int64_t b)
	{
		m_Impl->m_Writer.Int64(b);
		return *this;
	}

	Writer& Writer::operator &(const double b)
	{
		m_Impl->m_Writer.Double(b);
		return *this;
	}

	Writer& Writer::operator &(const uint64_t b)
	{
		m_Impl->m_Writer.Uint64(b);
		return *this;
	}

	Writer& Writer::operator &(const std::string& s)
	{
		m_Impl->m_Writer.String(s.c_str(), static_cast<unsigned>(s.size()));
		return *this;
	}

	Writer& Writer::operator &(const std::filesystem::path& p)
	{
		m_Impl->m_Writer.String(p.string().c_str(), static_cast<unsigned>(p.string().size()));
		return *this;
	}

	Writer& Writer::SetNull()
	{
		m_Impl->m_Writer.Null();
		return *this;
	}

	Writer::~Writer() = default;
}