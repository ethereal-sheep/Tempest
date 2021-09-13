#include "Graphics/Basics/ShadowMap.h"

namespace Tempest
{
	ShadowMap::ShadowMap(uint32_t width, uint32_t height)
	{
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    ShadowMap::~ShadowMap()
    {
        if (m_ID != 0)
            glDeleteTextures(1, &m_ID);
    }

    uint32_t ShadowMap::GetID() const
    {
        return m_ID;
    }

    uint32_t ShadowMap::GetWidth() const
    {
        return m_Width;
    }

    uint32_t ShadowMap::GetHeight() const
    {
        return m_Height;
    }
}