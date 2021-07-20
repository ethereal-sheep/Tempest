#pragma once
#include <glew.h>
#include "Core.h"

namespace Tempest
{
    class IndexBuffer : std::enable_shared_from_this<IndexBuffer>
    {
        uint32_t m_ID;
        uint32_t m_Count;

    public:

        IndexBuffer(const uint32_t* array, uint32_t count);
        ~IndexBuffer();

        uint32_t GetID() const;
        uint32_t GetCount() const;

        void Bind() const;
        void Unbind() const;

        tsptr<IndexBuffer> GetShared();
    };
}