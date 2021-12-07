/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Graphics/OpenGL/BufferLayout.h"

namespace Tempest
{
    struct DrawArrayIndirect
    {
    	uint32_t vertexCount = 0;
    	uint32_t instanceCount = 0;
    	uint32_t baseVertex = 0;
    	uint32_t baseInstance = 0;
    };
    
    struct DrawElementsIndirect
    {
    	uint32_t vertexCount = 0;
    	uint32_t instanceCount = 0;
    	uint32_t firstIndex = 0;
    	uint32_t baseVertex = 0;
    	uint32_t baseInstance = 0;
    };

    enum struct BufferType
    {
        ARRAY_BUFFER,
        INDIRECT_BUFFER
    };

    class VertexBuffer : public std::enable_shared_from_this<VertexBuffer>
    {
        uint32_t m_ID;
        uint32_t m_Size;
        BufferType m_Type;

    public:

        VertexBuffer();
        //VertexBuffer(const VertexBuffer&) = delete;
        //VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(void* data, uint32_t size, BufferType type);
        ~VertexBuffer();

        uint32_t GetID() const;
        uint32_t GetSize() const;

        void Bind() const;     
        void Unbind() const;

        void SetData(void* data, int size, BufferType type);
        void SetSubData(void* data, int size, int offset = 0);
        void SetSubDataResize(void* data, int size);

        tsptr<VertexBuffer> GetShared();
    };
}