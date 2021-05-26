#pragma once
#include <glew.h>
#include <glm.hpp>
#include <initializer_list>
#include <vector>

namespace Tempest
{
	enum struct VertexType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,

		BYTEPAD
	};

	class BufferLayout
	{
	public:
		struct Attribute
		{
			VertexType type;
			bool normalized = false;
			unsigned int offset = 0;
			unsigned int relative_offset = 0;
			unsigned int stride = 0;
		};

		explicit BufferLayout() = default;
		explicit BufferLayout(std::initializer_list<VertexType> attributes);

		size_t GetSize() const;

		static uint32_t CountOf(VertexType type);
		static uint32_t SizeOf(VertexType type);

		auto begin()		{ return m_Attributes.begin(); }
		auto end()			{ return m_Attributes.end(); }
		auto begin() const	{ return m_Attributes.cbegin(); }
		auto end() const	{ return m_Attributes.cend(); }
		auto cbegin() const { return m_Attributes.cbegin(); }
		auto cend() const	{ return m_Attributes.cend(); }

	private:
		void CalculateNoninterleaved(const std::initializer_list<VertexType>& attributes);

		std::vector<Attribute> m_Attributes;
	};
}
