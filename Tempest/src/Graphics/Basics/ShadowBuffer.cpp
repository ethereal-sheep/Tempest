#include "Graphics/Basics/ShadowBuffer.h"

namespace Tempest
{
	ShadowBuffer::ShadowBuffer(uint32_t width , uint32_t height)
	{
        glEnable(GL_DEPTH_TEST);
        m_Width = width;
        m_Height = height;
        //constexpr float quadVertices[] =
        //{
        //    // positions  
        //    -1.0f, -1.0f, 
        //     1.0f, -1.0f, 
        //     1.0f,  1.0f, 
        //    -1.0f,  1.0f 
        //};

        //GLuint indices[6];
        //GLuint offset = 0;

        //for (int i = 0; i < 6; i += 6)
        //{
        //    indices[i + 0] = 0 + offset;
        //    indices[i + 1] = 1 + offset;
        //    indices[i + 2] = 2 + offset;

        //    indices[i + 3] = 2 + offset;
        //    indices[i + 4] = 3 + offset;
        //    indices[i + 5] = 0 + offset;

        //    offset += 4;
        //    m_Count += 6;
        //}

        //glGenVertexArrays(1, &m_vao);
        //glGenBuffers(1, &m_vbo);
        //glBindVertexArray(m_vao);
        //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        //glCreateBuffers(1, &m_ibo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //glBindVertexArray(0);

        //glGenFramebuffers(1, &m_ID);
        //glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);


        
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