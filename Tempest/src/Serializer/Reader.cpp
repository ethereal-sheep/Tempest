/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core.h"
#include <stack>
#include "Reader.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

namespace Tempest
{
    struct JsonReaderStackItem
    {
        enum struct State
        {
            BeforeStart,
            Started,
            Closed
        };

        JsonReaderStackItem(const rapidjson::Value& value, State state = State::BeforeStart)
            : m_Value{ value }
            , m_State{ state }
        {}

        unsigned m_Index = 0;
        const rapidjson::Value& m_Value;
        State m_State = State::BeforeStart;
    };

	struct Reader::Impl
	{
        using JsonReaderStack = std::stack<JsonReaderStackItem>;

        Impl() : m_Doc{}, m_Stack{}, m_Error{ false } {}

        template <typename TObj, typename TValue>
        void SetValue(TObj& obj, TValue v)
        {
            if (!m_Error) 
            {
                obj = v;
                Next();
            }
        }

        bool Check(int) const { return Current().IsInt(); }
        bool Check(bool) const { return Current().IsBool(); }
        bool Check(float) const { return Current().IsFloat(); }
        bool Check(double) const { return Current().IsDouble(); }
        bool Check(unsigned) const { return Current().IsUint(); }
        bool Check(int64_t) const { return Current().IsInt64(); }
        bool Check(uint64_t) const { return Current().IsUint64(); }
        bool Check(const std::string&) const { return Current().IsString(); }

        template <typename TData>
        bool CheckValue(const TData& data)
        {
            if (m_Error || m_Stack.empty() || !Check(data)) 
                return false;
            return true;
        }

        void CheckParse()
        {
            m_Error = m_Doc.HasParseError();

            if (!m_Error)
                Push(m_Doc);
        }

        bool BeforeStart() const { return Top().m_State == JsonReaderStackItem::State::BeforeStart; }
        bool Started() const { return Top().m_State == JsonReaderStackItem::State::Started; }
        bool Closed() const { return Top().m_State == JsonReaderStackItem::State::Closed; }

        JsonReaderStackItem& Top() { return m_Stack.top(); }
        const JsonReaderStackItem& Top() const { return m_Stack.top(); }
        const rapidjson::Value& Current() const { return Top().m_Value; }

        void Push(const rapidjson::Value& value) { m_Stack.push(JsonReaderStackItem(value)); }

        void Next()
        {
            if (m_Stack.empty())
            {
                m_Error = true;
                return;
            }
                
            if (!m_Error)
            {
                m_Stack.pop();

                if (m_Stack.empty() || !Current().IsArray())
                    return;

                if (Started())
                {
                    if (Top().m_Index < Current().Size() - 1)
                        Push(Current()[++Top().m_Index]);
                    else
                        Top().m_State = JsonReaderStackItem::State::Closed;
                }
                else
                {
                    m_Error = true;
                }
            }
        }

        rapidjson::Document m_Doc;
        JsonReaderStack m_Stack;
        bool m_Error;
    };

    Reader::Reader(std::ifstream& stream)
    {
        rapidjson::IStreamWrapper isw(stream);
        m_Impl = std::make_unique<Impl>();
        m_Impl->m_Doc.ParseStream(isw);
        m_Impl->CheckParse();
    }

    Reader::Reader(const char* json)
	{
		m_Impl = std::make_unique<Impl>();
        m_Impl->m_Doc.Parse(json);
        m_Impl->CheckParse();
	}

    Reader::~Reader() = default;

    Reader& Reader::StartObject()
    {
        if (!HasError())
        {
            if (m_Impl->Current().IsObject() && m_Impl->BeforeStart())
                m_Impl->Top().m_State = JsonReaderStackItem::State::Started;
            else
                m_Impl->m_Error = true;
        }

        return *this;
    }

    Reader& Reader::StartObject(const char* identifier)
    {
        Member(identifier);
        return StartObject();
    }

    Reader& Reader::EndObject()
    {
        if (!HasError())
        {
            if (m_Impl->Current().IsObject() && m_Impl->Started())
                m_Impl->Next();
            else
                m_Impl->m_Error = true;
        }

        return *this;
    }

    Reader& Reader::StartMeta(const char* identifier)
    {
        Member(identifier);
        return StartObject();
    }

    Reader& Reader::EndMeta()
    {
        return EndObject();
    }

    Reader& Reader::Member(const char* identifier)
    {
        if (!HasError())
        {
            if ((m_Impl->Current().IsObject() || m_Impl->Current().IsNull()) && m_Impl->Started())
            {
                auto memberItr = m_Impl->Current().FindMember(identifier);

                if (memberItr != m_Impl->Current().MemberEnd())
                    m_Impl->Push(memberItr->value);
            }
            else
            {
                m_Impl->m_Error = true;
            }
        }

        return *this;
    }

    bool Reader::HasMember(const char* identifier)
    {
        if (!m_Impl->m_Error && m_Impl->Current().IsObject() && m_Impl->Started())
            return m_Impl->Current().HasMember(identifier);

        return false;
    }

    bool Reader::HasError() const
    {
        return m_Impl->m_Error;
    }

    Reader& Reader::StartArray(size_t* size)
    {
        if (!HasError())
        {
            if (m_Impl->Current().IsArray() && m_Impl->BeforeStart())
            {
                m_Impl->Top().m_State = JsonReaderStackItem::State::Started;

                if (size)
                    *size = m_Impl->Current().Size();

                if (!m_Impl->Current().Empty())
                    m_Impl->Push(m_Impl->Current()[m_Impl->Top().m_Index]);
                else
                    m_Impl->Top().m_State = JsonReaderStackItem::State::Closed;
            }
            else
            {
                m_Impl->m_Error = true;
            }
        }

        return *this;
    }

    Reader& Reader::StartArray(const char* identifier, size_t* size)
    {
        Member(identifier);
        return StartArray(size);
    }

    Reader& Reader::EndArray()
    {
        if (!HasError())
        {
            if (m_Impl->Current().IsArray() && m_Impl->Closed())
                m_Impl->Next();
            else
                m_Impl->m_Error = true;
        }

        return *this;
    }

    Reader& Reader::operator& (int& i)
    {
        if(m_Impl->CheckValue(i))
            m_Impl->SetValue(i, m_Impl->Current().GetInt());
        return *this;
    }

    Reader& Reader::operator& (bool& b)
    {
        if (m_Impl->CheckValue(b))
            m_Impl->SetValue(b, m_Impl->Current().GetBool());
        return *this;
    }

    Reader& Reader::operator& (float& f)
    {
        if (m_Impl->CheckValue(f))
            m_Impl->SetValue(f, m_Impl->Current().GetFloat());
        return *this;
    }

    Reader& Reader::operator& (double& d)
    {
        if (m_Impl->CheckValue(d))
            m_Impl->SetValue(d, m_Impl->Current().GetDouble());
        return *this;
    }

    Reader& Reader::operator& (unsigned& u)
    {
        if (m_Impl->CheckValue(u))
            m_Impl->SetValue(u, m_Impl->Current().GetUint());
        return *this;
    }

    Reader& Reader::operator& (int64_t& i64)
    {
        if (m_Impl->CheckValue(i64))
            m_Impl->SetValue(i64, m_Impl->Current().GetInt64());
        return *this;
    }

    Reader& Reader::operator& (uint64_t& u64)
    {
        if (m_Impl->CheckValue(u64))
            m_Impl->SetValue(u64, m_Impl->Current().GetUint64());
        return *this;
    }

    Reader& Reader::operator& (std::string& s)
    {
        if (m_Impl->CheckValue(s))
            m_Impl->SetValue(s, m_Impl->Current().GetString());
        return *this;
    }

    Reader& Reader::operator& (std::filesystem::path& p)
    {
        if (m_Impl->CheckValue(p.string()))
            m_Impl->SetValue(p, m_Impl->Current().GetString());
        return *this;
    }
}