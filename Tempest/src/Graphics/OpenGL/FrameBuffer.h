#pragma once
#include "Graphics/OpenGL/Texture.h"
#include "Logger/Log.h"

namespace Tempest
{
	enum struct FrameBufferMode
	{
		READ,
		WRITE
	};

	enum struct FrameCopyBit
	{
		COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT
	};

	enum struct FrameCopyFilter
	{
		NEAREST,
		LINEAR
	};

	class FrameBuffer 
	{
	private:
		uint32_t m_ID = 0;
		uint32_t m_colorAttachmentCount = 0;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

	public:

		FrameBuffer();
		~FrameBuffer();

		void AttachColorBuffer(const Texture& texture);
		void PopColorBuffer();

		void AttachDepthBuffer(const Texture& texture) const;
		void PopDepthBuffer();

		void AttachDepthStencilBuffer(const Texture& texture) const;

		void CopyBufferContents(const FrameBuffer& src, FrameCopyBit copy, FrameCopyFilter filter,
			uint32_t srcX0, uint32_t srcY0, uint32_t srcX1, uint32_t srcY2,
			uint32_t dstX0, uint32_t dstY0, uint32_t dstX1, uint32_t dstY2) const;

		void UseDrawBuffers() const;
		void UseOnlyDepth() const;
		void Validate() const;

		uint32_t GetID() const;
		void Bind(FrameBufferMode mode = FrameBufferMode::WRITE) const;
		static void Unbind(FrameBufferMode mode = FrameBufferMode::WRITE);

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		uint32_t GetColorAttachmentCount() const;
	};
}