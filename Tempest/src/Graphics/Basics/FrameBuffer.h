#pragma once
#include "Core.h"

namespace Tempest
{
    class FrameBuffer
    {
        uint32_t m_ID;

        uint32_t m_ColourBuffer = 0; 
        uint32_t m_RenderBuffer = 0;
        
        uint32_t m_Width;
        uint32_t m_Height;

        VAO vao;
    public: 

        FrameBuffer(uint32_t width, uint32_t height) 
        : m_Width(width), m_Height(height)
        {
            // create quad
            // create 
        }
        FrameBuffer(uint32_t width, uint32_t height, const Shader& ) 
        : m_Width(width), m_Height(height)
        {
            // create quad
            // create 
        }

        FrameBuffer()
        {
            
        }

        uint32_t GetID() const
        {
            return m_ID;
        }

        uint32_t GetWidth() const
        {
            return m_Width;
        }
        uint32_t GetHeight() const
        {
            return m_Height;
        }

        uint32_t GetColourBuffer() const
        {
            return m_ColourBuffer;
        }
        


        void Resize()
        {

        }

        void Bind() const
        {

        }

        void Unbind() const
        {

        }
    };
}