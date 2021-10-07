/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/OpenGL/Shader.h"
#include "Util.h"

namespace Tempest
{
    class FBO
    {
        uint32_t m_ID = 0;

        uint32_t m_ColourBuffer = 0;
        uint32_t m_RenderBuffer = 0;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        uint32_t m_vao = 0;
        uint32_t m_ibo = 0;
        uint32_t m_vbo = 0;
        uint32_t m_Count = 0;

        Shader m_Shader{ "Shaders/FrameBuffer_vertex.glsl", "Shaders/FrameBuffer_fragment.glsl" };
    public:

        FBO(uint32_t width, uint32_t height);
        FBO(uint32_t width, uint32_t height, tuptr<Shader> shader);
        FBO() = delete;

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
        void SetFrameBufferSize();
    };
}