#pragma once
#include <glew.h>
#include <cstdint>
#include "Graphics/OpenGL/Shader.h"

namespace Tempest
{
    class FBO
    {
        uint32_t m_ID = 0;
        uint32_t m_ColourBuffer = 0;
        uint32_t m_RenderBuffer = 0;

        uint32_t m_Width;
        uint32_t m_Height;

        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ibo = 0;

        Shader m_Shader{ "Shaders/FrameBuffer_vertex.glsl", "Shaders/FrameBuffer_fragment.glsl" };

    public:

        FBO(uint32_t width, uint32_t height);
        ~FBO();

        FBO(const FBO&) = delete;
        FBO& operator=(const FBO&) = delete;

        void Attach();
        void Detach();
        void Draw(Shader& shader);
        void Draw();
        void Resize(uint32_t width, uint32_t height);

        uint32_t Height() const;
        uint32_t Width() const;
        uint32_t ID() const;
        uint32_t IBO() const;
        uint32_t ColourBuffer() const;

    };
}
