//#include "Graphics/OpenGL/FrameBuffer.h"
//
//namespace Tempest
//{
//	constexpr GLenum GLModeOf(FrameBufferMode mode)
//	{
//		switch (mode)
//		{
//			default: assert(0);
//			case FrameBufferMode::READ:			return GL_READ_FRAMEBUFFER;
//			case FrameBufferMode::WRITE:		return GL_DRAW_FRAMEBUFFER;
//		}
//	}
//
//	constexpr GLenum GLModeOf(FrameCopyBit mode)
//	{
//		switch (mode)
//		{
//			default: assert(0);
//			case FrameCopyBit::COLOR_BUFFER_BIT:		return GL_COLOR_BUFFER_BIT;
//			case FrameCopyBit::DEPTH_BUFFER_BIT:		return GL_DEPTH_BUFFER_BIT;
//			case FrameCopyBit::STENCIL_BUFFER_BIT:		return GL_STENCIL_BUFFER_BIT;
//		}
//	}
//
//	constexpr GLenum GLModeOf(FrameCopyFilter mode)
//	{
//		switch (mode)
//		{
//			default: assert(0);
//			case FrameCopyFilter::LINEAR:		return GL_LINEAR;
//			case FrameCopyFilter::NEAREST:		return GL_NEAREST;
//		}
//	}
//
//	FrameBuffer::FrameBuffer()
//	{
//		glCreateFramebuffers(1, &m_ID);
//	}
//
//	FrameBuffer::~FrameBuffer()
//	{
//		if (m_ID != 0)
//			glDeleteFramebuffers(1, &m_ID);
//	}
//
//	void FrameBuffer::AttachColorBuffer([[maybe_unused]] const Texture& texture)
//	{
//		const auto attachment = GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount++;
//		
//		glNamedFramebufferTexture(m_ID, attachment, texture.GetID(), 0);
//		
//		m_Width = texture.GetWidth();
//		m_Height = texture.GetHeight();
//	}
//
//	void FrameBuffer::PopColorBuffer()
//	{
//		if (m_colorAttachmentCount != 0)
//		{
//			const auto attachment = GL_COLOR_ATTACHMENT0 + m_colorAttachmentCount--;
//			glNamedFramebufferTexture(m_ID, attachment, 0, 0);
//		}
//	}
//
//	void FrameBuffer::AttachDepthBuffer([[maybe_unused]] const Texture& texture) const
//	{
//		glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, texture.GetID(), 0);
//	}
//
//	void FrameBuffer::AttachDepthStencilBuffer(const Texture& texture) const
//	{
//		glNamedFramebufferTexture(m_ID, GL_DEPTH_STENCIL_ATTACHMENT, texture.GetID(), 0);
//	}
//
//	void FrameBuffer::PopDepthBuffer()
//	{
//		glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, 0, 0);
//	}
//
//	void FrameBuffer::CopyBufferContents(const FrameBuffer& src, FrameCopyBit copy, FrameCopyFilter filter,
//		uint32_t srcX0, uint32_t srcY0, uint32_t srcX1, uint32_t srcY1,
//		uint32_t dstX0, uint32_t dstY0, uint32_t dstX1, uint32_t dstY1) const
//	{
//		glBlitNamedFramebuffer(m_ID, src.GetID(),
//			srcX0, srcY0, srcX1, srcY1,
//			dstX0, dstY0, dstX1, dstY1,
//			GLModeOf(copy), GLModeOf(filter));
//	}
//
//	void FrameBuffer::UseDrawBuffers() const
//	{
//		const auto attachments = std::make_unique<uint32_t[]>(m_colorAttachmentCount);
//		for (uint32_t i = 0; i < m_colorAttachmentCount; ++i)
//			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
//
//		glNamedFramebufferDrawBuffers(m_ID, m_colorAttachmentCount, attachments.get());
//	}
//
//	void FrameBuffer::UseOnlyDepth() const
//	{
//		glNamedFramebufferDrawBuffer(m_ID, GL_NONE);
//		glNamedFramebufferReadBuffer(m_ID, GL_NONE);
//	}
//
//	void FrameBuffer::Validate() const
//	{
//		switch (glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER))
//		{
//		case GL_FRAMEBUFFER_COMPLETE:
//			return;
//		case GL_FRAMEBUFFER_UNDEFINED:
//			LOG_CRITICAL("Framebuffer undefined.");
//			return;
//		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
//			LOG_CRITICAL("Framebuffer incomplete attachment.");
//			return;
//		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
//			LOG_CRITICAL("Framebuffer incomplete missing attachment.");
//			return;
//		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
//			LOG_CRITICAL("Framebuffer incomplete draw buffer.");
//			return;
//		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
//			LOG_CRITICAL("Framebuffer incomplete read buffer.");
//			return;
//		case GL_FRAMEBUFFER_UNSUPPORTED:
//			LOG_CRITICAL("Framebuffer unsupported.");
//			return;
//		default:;
//			LOG_CRITICAL("Framebuffer unknown error.");
//		}
//	}
//
//	uint32_t FrameBuffer::GetID() const
//	{
//		return m_ID;
//	}
//
//	void FrameBuffer::Bind(FrameBufferMode mode) const
//	{
//		glBindFramebuffer(GLModeOf(mode), m_ID);
//	}
//
//	void FrameBuffer::Unbind(FrameBufferMode mode)
//	{
//		glBindFramebuffer(GLModeOf(mode), 0);
//	}
//
//	uint32_t FrameBuffer::GetWidth() const
//	{
//		return m_Width;
//	}
//
//	uint32_t FrameBuffer::GetHeight() const
//	{
//		return m_Height;
//	}
//	uint32_t FrameBuffer::GetColorAttachmentCount() const
//	{
//		return m_colorAttachmentCount;
//	}
//}