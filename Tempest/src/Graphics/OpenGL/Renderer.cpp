
#include "Renderer.h"

namespace Tempest
{
	constexpr GLenum GLBlendType(BlendFactor bf)
	{
		switch (bf)
		{
			default: assert(0);
			case BlendFactor::ZERO:							return GL_ZERO;						 
			case BlendFactor::ONE:							return GL_ONE;						 
			case BlendFactor::SRC_COLOR:					return GL_SRC_COLOR;				 
			case BlendFactor::ONE_MINUS_SRC_COLOR:			return GL_ONE_MINUS_SRC_COLOR;		 
			case BlendFactor::DST_COLOR:					return GL_DST_COLOR;				 
			case BlendFactor::ONE_MINUS_DST_COLOR:			return GL_ONE_MINUS_DST_COLOR;		 
			case BlendFactor::SRC_ALPHA:					return GL_SRC_ALPHA;				 
			case BlendFactor::ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;		 
			case BlendFactor::DST_ALPHA:					return GL_DST_ALPHA;				 
			case BlendFactor::ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;		 
			case BlendFactor::CONSTANT_COLOR:				return GL_CONSTANT_COLOR;			 
			case BlendFactor::ONE_MINUS_CONSTANT_COLOR:		return GL_ONE_MINUS_CONSTANT_COLOR;	 
			case BlendFactor::CONSTANT_ALPHA:				return GL_CONSTANT_ALPHA;			 
			case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:		return GL_ONE_MINUS_CONSTANT_ALPHA;	 
		}
	}

	constexpr GLenum GLMode(DrawMode dm)
	{
		switch (dm)
		{
			default: assert(0);
			case DrawMode::POINTS:						return GL_POINTS;
			case DrawMode::LINE_STRIP:					return GL_LINE_STRIP;
			case DrawMode::LINE_LOOP:					return GL_LINE_LOOP;
			case DrawMode::LINE:						return GL_LINES;
			case DrawMode::LINE_STRIP_ADJACENCY:		return GL_LINE_STRIP_ADJACENCY;
			case DrawMode::LINES_ADJACENCY:				return GL_LINES_ADJACENCY;
			case DrawMode::TRIANGLE_STRIP:				return GL_TRIANGLE_STRIP;
			case DrawMode::TRIANGLE_FAN:				return GL_TRIANGLE_FAN;
			case DrawMode::TRIANGLES:					return GL_TRIANGLES;
			case DrawMode::TRIANGLE_STRIP_ADJACENCY:	return GL_TRIANGLE_STRIP_ADJACENCY;
			case DrawMode::PATCHES:						return GL_PATCHES;
		}
	}

	constexpr GLenum GLDrawType(DrawType dt)
	{
		switch (dt)
		{
			default: assert(0);
			case DrawType::UNSIGNED_INT:	return GL_UNSIGNED_INT;
			case DrawType::UNSIGNED_BYTE:	return GL_UNSIGNED_BYTE;
			case DrawType::UNSIGNED_SHORT:	return GL_UNSIGNED_SHORT;
		}
	}

	constexpr GLenum GLPolyMode(PolyMode pm)
	{
		switch (pm)
		{
			default: assert(0);
			case PolyMode::POINT:		return GL_POINT;
			case PolyMode::LINE:		return GL_LINE;
			case PolyMode::FILL:		return GL_FILL;
		}
	}

	constexpr GLenum GLBlendFunc(BlendFunc bf)
	{
		switch (bf)
		{
			default: assert(0);
			case BlendFunc::ADD:						return GL_FUNC_ADD;
			case BlendFunc::SUBTRACT:					return GL_FUNC_SUBTRACT;
			case BlendFunc::REVERSE_SUBTRACT:			return GL_FUNC_REVERSE_SUBTRACT;
			case BlendFunc::MIN:						return GL_MIN;
			case BlendFunc::MAX:						return GL_MAX;
		}
	}

	constexpr GLenum GLDepthFunc(DepthFunc df)
	{
		switch (df)
		{
			default: assert(0);
			case DepthFunc::NEVER:				return GL_NEVER;
			case DepthFunc::LESS:				return GL_LESS;
			case DepthFunc::EQUAL:				return GL_EQUAL;
			case DepthFunc::LESS_EQUAL:			return GL_LEQUAL;
			case DepthFunc::GREATER:			return GL_GREATER;
			case DepthFunc::NOT_EQUAL:			return GL_NOTEQUAL;
			case DepthFunc::GREATER_EQUAL:		return GL_GEQUAL;
			case DepthFunc::ALWAYS:				return GL_ALWAYS;
		}
	}

	Renderer::Renderer()
		: clearMask{GL_COLOR_BUFFER_BIT} {}

	void Renderer::Clear() const
	{
		glClear(clearMask);
	}

	void Renderer::ClearColorDepth() const
	{
		glClear(clearMask | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Flush() const
	{
		glFlush();
	}

	void Renderer::Finish() const
	{
		glFinish();
	}

	void Renderer::ClearColour(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::DrawElements(DrawMode mode, const VertexArray& vao, const IndexBuffer& ibo, DrawType type)
	{
		if (ibo.GetCount() != 0)
		{
			vao.Bind();
			ibo.Bind();
			glDrawElements(GLMode(mode), ibo.GetCount(), GLDrawType(type), nullptr);
			vao.Unbind();
			ibo.Unbind();
		}
	}

	void Renderer::DrawArrays(DrawMode mode, const VertexArray& vao, unsigned int vertexCount)
	{
		if (vertexCount != 0)
		{
			vao.Bind();
			glDrawArrays(GLMode(mode), 0, vertexCount);
			vao.Unbind();
		}
	}

	void Renderer::MultiDrawElementsIndirect(DrawMode mode, const VertexArray& vao, const IndexBuffer& ibo,
		const VertexBuffer& indirect)
	{
		//if (indirect.GetSize() != 0)
		//{
		//	vao.Bind();
		//	ibo.Bind();
		//	indirect.Bind();
		//	glMultiDrawElementsIndirect(GLMode(mode), GL_UNSIGNED_INT, nullptr,
		//		indirect.GetSize() / sizeof(DrawElementsIndirectCommand), 0);
		//	vao.Unbind();
		//	ibo.Unbind();
		//	indirect.Unbind();
		//}
		(void)mode;
		(void)vao;
		(void)ibo;
		(void)indirect;
	}

	void Renderer::MultiDrawArraysIndirect(DrawMode mode, const VertexArray& vao, const VertexBuffer& indirect)
	{
		//if (indirect.GetSize() != 0)
		//{
		//	vao.Bind();
		//	indirect.Bind();
		//	glMultiDrawArraysIndirect(GLMode(mode), nullptr, indirect.GetSize() / sizeof(DrawArraysIndirectCommand), 0);
		//	vao.Unbind();
		//	indirect.Unbind();
		//}
		(void)mode;
		(void)vao;
		(void)indirect;
	}

	void Renderer::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned height) const
	{
		glViewport(x, y, width, height);
	}

	void Renderer::SetPolygonMode(PolyMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GLPolyMode(mode));
	}

	void Renderer::SetDepthFunc(DepthFunc fn)
	{
		glDepthFunc(GLDepthFunc(fn));
	}

	void Renderer::SetBlendFunc(BlendFunc fn)
	{
		glBlendEquation(GLBlendFunc(fn));
	}

	void Renderer::SetStencilFunc(DepthFunc fn, int ref, unsigned int mask)
	{
		glStencilFunc(GLDepthFunc(fn), ref, mask);
	}

	void Renderer::SetPointSize(float size)
	{
		glPointSize(size);
	}

	void Renderer::SetLineWidth(float size)
	{
		glLineWidth(size);
	}

	void Renderer::EnableBlend(bool enable, BlendFactor src, BlendFactor dest)
	{
		if (enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GLBlendType(src), GLBlendType(dest));
		}

		else
		{
			glDisable(GL_BLEND);
		}
	}

	void Renderer::EnableColorMask(bool r, bool g, bool b, bool a)
	{
		glColorMask(r, g, b, a);
	}

	void Renderer::EnableDepthMask(bool enable)
	{
		glDepthMask(enable);
	}

	void Renderer::EnableDepthTest(bool enable)
	{
		depthTest = enable;

		if (depthTest)
		{
			glEnable(GL_DEPTH_TEST);
			clearMask |= GL_DEPTH_BUFFER_BIT;
		}

		else
		{
			glDisable(GL_DEPTH_TEST);
			clearMask |= ~GL_DEPTH_BUFFER_BIT;
		}
	}

	void Renderer::EnableStencilTest(bool enable)
	{
		if (enable)
		{
			clearMask |= GL_STENCIL_BUFFER_BIT;
			glEnable(GL_STENCIL_TEST);
		}

		else
		{
			clearMask |= ~GL_STENCIL_BUFFER_BIT;
			glDisable(GL_STENCIL_TEST);
		}
	}

	void Renderer::EnableMultiSampling(bool enable) 
	{
		if (enable)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}

	void Renderer::EnableCulling(bool enable, bool winding, bool back)
	{
		if (enable)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		if (winding)
			glFrontFace(GL_CCW);
		else
			glFrontFace(GL_CW);

		if (back)
			glCullFace(GL_BACK);
		else
			glCullFace(GL_FRONT);
	}

	void Renderer::EnableClearColour(bool enable)
	{
		if (enable)
			clearMask |= GL_COLOR_BUFFER_BIT;
		else
			clearMask |= ~GL_COLOR_BUFFER_BIT;
	}
}