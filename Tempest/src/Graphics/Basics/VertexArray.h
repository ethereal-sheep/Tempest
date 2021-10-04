#pragma once
#include "Core.h"
#include "Graphics/Basics/IndexBuffer.h"
#include "Graphics/Basics/VertexBuffer.h"
#include "Graphics/OpenGL/BufferLayout.h"

namespace Tempest
{
    class VertexArray : std::enable_shared_from_this<VertexArray>
    {
        uint32_t m_ID;
        uint32_t m_Index = 0;
        tsptr<IndexBuffer> m_IBO;
        tvector<tsptr<VertexBuffer>> m_VBOs;

    public:

        VertexArray();
        VertexArray(const tsptr<IndexBuffer>& ibo, const tvector<tsptr<VertexBuffer>>& vbos, const tvector<BufferLayout>& layouts);
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        ~VertexArray();

        uint32_t GetID() const;

        void Bind() const;
        void Unbind() const;

        void AttachVertexBuffer(const VertexBuffer& vbo, const BufferLayout& layout);
        void AttachIndexBuffer(const IndexBuffer& ibo) const;
        void AttachVertexBufferInstanced(const VertexBuffer& vbo, const BufferLayout& layout);        
        void SetBindingDivisor(unsigned int index, unsigned int divisor) const;
        
        tsptr<VertexArray> GetShared();  
    };
}