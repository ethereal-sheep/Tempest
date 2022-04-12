/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/RenderBuffer.h"

namespace Tempest
{
	RenderBuffer::RenderBuffer()
	{
		glCreateRenderbuffers(1, &m_ID);
	}

	RenderBuffer::~RenderBuffer()
	{
		if (m_ID != 0)
			glDeleteRenderbuffers(1, &m_ID);
	}

	void RenderBuffer::InitStorage(uint32_t width, uint32_t height, uint32_t samples) const
	{
		glNamedRenderbufferStorageMultisample(m_ID, samples, GL_DEPTH_STENCIL, width, height);
	}

	uint32_t RenderBuffer::GetID() const
	{
		return m_ID;
	}

	void RenderBuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	}

	void RenderBuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	uint32_t RenderBuffer::GetWidth() const
	{
		return m_Width;
	}

	uint32_t RenderBuffer::GetHeight() const
	{
		return m_Height;
	}

	uint32_t RenderBuffer::GetSamples() const
	{
		return m_Samples;
	}
}