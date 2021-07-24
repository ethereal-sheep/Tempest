#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/OpenGL/Shader.h"
#include "Util.h"

namespace Tempest
{
    class FrameBuffer
    {
        uint32_t m_ID = 0;

        uint32_t m_ColourBuffer = 0; 
        uint32_t m_RenderBuffer = 0;
        
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        //VertexArray m_VAO;
        uint32_t m_vao = 0;
        uint32_t m_ibo = 0;
        uint32_t m_vbo = 0;
        uint32_t m_Count = 0;

        //[[maybe_unused]] tuptr<Shader> m_Shader;
        Shader m_Shader{ "Shaders/FrameBuffer_vertex.glsl", "Shaders/FrameBuffer_fragment.glsl" };
    public: 

        FrameBuffer(uint32_t width, uint32_t height);
        FrameBuffer(uint32_t width, uint32_t height, tuptr<Shader> shader);
        FrameBuffer() = delete;

        uint32_t GetID() const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        uint32_t GetColourBuffer() const;
        uint32_t& GetIndexBuffer();
        uint32_t& GetVertexArray();
     
        void Resize(uint32_t width, uint32_t height);
        void Validate() const;
        void Draw();

        void Bind() const;
        void Unbind() const;
    };
}