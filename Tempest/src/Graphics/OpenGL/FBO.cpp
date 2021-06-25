#include "FBO.h"
#include "Logger/Log.h"

namespace Tempest
{


    FBO::FBO(uint32_t width, uint32_t height) : m_Width(width), m_Height(m_Width)
    {
		glEnable(GL_DEPTH_TEST);

		constexpr float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    	};

        GLuint indices[6];
        GLuint offset = 0;
        
        for (int i = 0; i < 6; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
        
            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
        
            offset += 4;
        }

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glCreateBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		// create a color attachment texture
		glGenTextures(1, &m_ColourBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourBuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_CRITICAL("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FBO::~FBO()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteFramebuffers(1, &m_ID);
        glDeleteTextures(1, &m_ColourBuffer);
        glDeleteTextures(1, &m_RenderBuffer);
    }

    void FBO::Attach()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    }

    void FBO::Detach()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void FBO::Draw(Shader& shader)
    {
		shader.Bind();
        glBindVertexArray(m_vao);
        glBindTexture(GL_TEXTURE_2D, m_ColourBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
    void FBO::Draw()
    {
		m_Shader.Bind();
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBindTexture(GL_TEXTURE_2D, m_ColourBuffer);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }

    void FBO::Resize(uint32_t width, uint32_t height)
    {
        glDeleteTextures(1, &m_ColourBuffer);
        glDeleteTextures(1, &m_RenderBuffer);
        
        m_Width = width;
        m_Height = height;

        // create a color attachment texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_ColourBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourBuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_CRITICAL("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t FBO::Height() const
    {
        return m_Height;
    }

    uint32_t FBO::Width() const
    {
        return m_Width;
    }

    uint32_t FBO::ID() const
    {
        return m_ID;
    }

    uint32_t FBO::ColourBuffer() const
    {
        return m_ColourBuffer;
    }

    uint32_t FBO::IBO() const
    {
        return m_ibo;
    }
}
