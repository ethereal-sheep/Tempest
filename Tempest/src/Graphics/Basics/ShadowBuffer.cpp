/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/ShadowBuffer.h"

namespace Tempest
{
	ShadowBuffer::ShadowBuffer(uint32_t width , uint32_t height)
	{
        glEnable(GL_DEPTH_TEST);
        m_Width = width;
        m_Height = height;      
	}

	void ShadowBuffer::AttachDepthAttachment(const ShadowMap& sm) const
	{
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, sm.GetID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowBuffer::ClearAttachments()
	{
        glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, 0, 0);
	}

	void ShadowBuffer::Validate() const
	{
        switch (glCheckNamedFramebufferStatus(m_depthMapFBO, GL_FRAMEBUFFER))
        {
        case GL_FRAMEBUFFER_COMPLETE:
        	return;
        case GL_FRAMEBUFFER_UNDEFINED:
        	LOG_CRITICAL("Framebuffer undefined.");
        	return;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        	LOG_CRITICAL("Framebuffer incomplete attachment.");
        	return;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        	LOG_CRITICAL("Framebuffer incomplete missing attachment.");
        	return;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        	LOG_CRITICAL("Framebuffer incomplete draw buffer.");
        	return;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        	LOG_CRITICAL("Framebuffer incomplete read buffer.");
        	return;
        case GL_FRAMEBUFFER_UNSUPPORTED:
        	LOG_CRITICAL("Framebuffer unsupported.");
        	return;
        default:;
        	LOG_CRITICAL("Framebuffer unknown error.");
        }
	}

	void ShadowBuffer::Draw(const std::vector<Point_Light>& point_lights, const tvector<MeshCode>& sprites, const tvector<glm::mat4>& transforms, const tmap<MeshCode, tuptr<Mesh>>& meshes)
	{
        (void)point_lights;
        (void)sprites;
        (void)transforms;
        (void)meshes;

	}

	void ShadowBuffer::Bind() const
	{
        glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	}

	void ShadowBuffer::Unbind() const
	{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void ShadowBuffer::ResizeViewport(const ShadowMap& sm)
    {
        glViewport(0, 0, sm.GetWidth(), sm.GetHeight());
    }

	uint32_t ShadowBuffer::GetID() const
	{
        return m_ID;
	}

	uint32_t& ShadowBuffer::GetIndexBuffer()
	{
        return m_ibo;
	}

	uint32_t& ShadowBuffer::GetVertexArray()
	{
        return m_vao;
	}
}