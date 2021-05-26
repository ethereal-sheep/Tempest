#pragma once

#include <glew.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include "Graphics/OpenGL/IndexBuffer.h"
#include "Graphics/OpenGL/VertexArray.h"

namespace Tempest
{
	enum struct BlendFactor
	{
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA
	};

	enum struct DrawMode
	{
		POINTS,
		LINE_STRIP,
		LINE_LOOP,
		LINE,
		LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY,
		PATCHES
	};

	enum struct DrawType
	{
		UNSIGNED_INT,
		UNSIGNED_BYTE,
		UNSIGNED_SHORT
	};

	enum struct PolyMode
	{
		POINT,
		LINE,
		FILL
	};

	enum struct BlendFunc
	{
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX
	};

	enum struct DepthFunc
	{
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS
	};

	//struct IndexBuffer
	//{
	//	unsigned int GetCount() { return 0; }
	//	void Bind() {}
	//	void Unbind() {}
	//};
	//
	//struct VertexArray
	//{
	//	void Bind() {}
	//	void Unbind() {}
	//};

	class Renderer
	{
	private:

		bool depthTest = false;
		unsigned int clearMask;

	public:

		Renderer();

		void Clear() const;
		void Flush() const;
		void Finish() const;

		void ClearColour(float r, float g, float b, float a);
		void DrawElements(DrawMode mode,  VertexArray vao,  IndexBuffer ibo, DrawType type = DrawType::UNSIGNED_INT);
		void DrawArrays(DrawMode mode,  VertexArray vao, unsigned int vertexCount);

		void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned height) const;
		void SetPolygonMode(PolyMode mode);
		void SetDepthFunc(DepthFunc fn);
		void SetBlendFunc(BlendFunc fn);
		void SetStencilFunc(DepthFunc fn, int ref, unsigned int mask);
		void SetPointSize(float size);
		void SetLineWidth(float size);

		void EnableBlend(bool enable, BlendFactor src = BlendFactor::SRC_ALPHA, BlendFactor dest = BlendFactor::ONE_MINUS_SRC_ALPHA);
		void EnableColorMask(bool r, bool g, bool b, bool a);
		void EnableDepthMask(bool enable);
		void EnableDepthTest(bool enable);
		void EnableStencilTest(bool enable);
		void EnableMultiSampling(bool enable);
		void EnableCulling(bool enable, bool winding, bool back);
		void EnableClearColour(bool enable);	
	};
}