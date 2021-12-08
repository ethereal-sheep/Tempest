/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <glew.h>
#include "Graphics/Basics/IndexBuffer.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/Basics/Mesh.h"

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

	class Renderer
	{
	private:

		bool depthTest = false;
		unsigned int clearMask;

	public:

		Renderer();

		void Clear() const;
		void ClearDepth() const;
		void ClearColorDepth() const;
		void Flush() const;
		void Finish() const;

		void ClearColour(float r, float g, float b, float a);
		void DrawElements(DrawMode mode, const VertexArray& vao, const IndexBuffer& ibo, DrawType type = DrawType::UNSIGNED_INT);
		void DrawElements(DrawMode mode, const Mesh& mesh, DrawType type = DrawType::UNSIGNED_INT);
		void DrawElements(DrawMode mode, const tuptr<Mesh>& mesh, DrawType type = DrawType::UNSIGNED_INT);
		void DrawArrays(DrawMode mode, const VertexArray& vao, unsigned int vertexCount);
		void MultiDrawElementsIndirect(DrawMode mode, const VertexArray& vao, const IndexBuffer& ibo, const VertexBuffer& indirect);
		void MultiDrawArraysIndirect(DrawMode mode, const VertexArray& vao, const VertexBuffer& indirect);

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