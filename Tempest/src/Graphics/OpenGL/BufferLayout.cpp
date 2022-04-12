/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "BufferLayout.h"

namespace Tempest
{
	BufferLayout::BufferLayout(Layout_Format format, std::initializer_list<VertexType> attributes)
	{
		switch (format)
		{
		case Layout_Format::SOA:
			CalculateNoninterleaved(attributes);
			break;

		case Layout_Format::AOS:
			CalculateInterleaved(attributes);
			break;
		}	
	}

	size_t BufferLayout::GetSize() const
	{
		return m_Attributes.size();
	}

	uint32_t BufferLayout::CountOf(VertexType type)
	{
		switch (type)
		{
		default: assert(0);
		case VertexType::Float:			return 1;
		case VertexType::Float2:		return 2;
		case VertexType::Float3:		return 3;
		case VertexType::Float4:		return 4;
		case VertexType::Mat3:			return 3 * 3;
		case VertexType::Mat4:			return 4 * 4;
		case VertexType::Int:			return 1;
		case VertexType::Int2:			return 2;
		case VertexType::Int3:			return 3;
		case VertexType::Int4:			return 4;
		case VertexType::Bool:			return 1;
		case VertexType::BYTEPAD:		return 1;
		}
	}

	uint32_t BufferLayout::SizeOf(VertexType type)
	{
		switch (type)
		{
		default: assert(0);
		case VertexType::Float:
		case VertexType::Float2:
		case VertexType::Float3:
		case VertexType::Float4:
		case VertexType::Mat3:
		case VertexType::Mat4:		return CountOf(type) * 4;
		case VertexType::Int:
		case VertexType::Int2:
		case VertexType::Int3:
		case VertexType::Int4:		return CountOf(type) * 4;
		case VertexType::Bool:		return CountOf(type) * 1;
		case VertexType::BYTEPAD:	return CountOf(type) * 1;
		}
	}

	void BufferLayout::CalculateNoninterleaved(const std::initializer_list<VertexType>& attributes)
	{
		uint32_t offset = 0;
		for (const auto& _type : attributes)
		{
			if (_type != VertexType::Mat3 && _type != VertexType::Mat4 && _type != VertexType::BYTEPAD)
			{
				auto& attribute = m_Attributes.emplace_back();

				attribute.type = _type;
				attribute.normalized = false;
				attribute.offset = offset;
				attribute.relative_offset = 0;
				attribute.stride = SizeOf(_type);
			}
			else if (_type == VertexType::Mat3)
			{
				for (uint32_t i = 0; i < 3; ++i)
				{
					auto& attribute = m_Attributes.emplace_back();

					attribute.type = VertexType::Float3;
					attribute.normalized = false;
					attribute.offset = offset;
					attribute.relative_offset = i * SizeOf(VertexType::Float3);
					attribute.stride = SizeOf(_type);
				}
			}
			else if (_type == VertexType::Mat4)
			{
				for (uint32_t i = 0; i < 4; ++i)
				{
					auto& attribute = m_Attributes.emplace_back();

					attribute.type = VertexType::Float4;
					attribute.normalized = false;
					attribute.offset = offset;
					attribute.relative_offset = i * SizeOf(VertexType::Float4);
					attribute.stride = SizeOf(_type);
				}
			}
			offset += SizeOf(_type);
		}
	}
	void BufferLayout::CalculateInterleaved(const std::initializer_list<VertexType>& attributes)
	{
		uint32_t offset = 0;
		for (const auto& _type : attributes)
		{
			if (_type != VertexType::Mat3 && _type != VertexType::Mat4 && _type != VertexType::BYTEPAD)
			{
				auto& attribute = m_Attributes.emplace_back();

				attribute.type = _type;
				attribute.normalized = false;
				attribute.relative_offset = offset;
			}
			else if (_type == VertexType::Mat3)
			{
				for (uint32_t i = 0; i < 3; ++i)
				{
					auto& attribute = m_Attributes.emplace_back();

					attribute.type = VertexType::Float3;
					attribute.normalized = false;
					attribute.relative_offset = offset + i * SizeOf(VertexType::Float3);
				}
			}
			else if (_type == VertexType::Mat4)
			{
				for (uint32_t i = 0; i < 4; ++i)
				{
					auto& attribute = m_Attributes.emplace_back();

					attribute.type = VertexType::Float4;
					attribute.normalized = false;
					attribute.relative_offset = offset + i * SizeOf(VertexType::Float4);
				}
			}
			offset += SizeOf(_type);
		}
		for (auto& attrib : m_Attributes)
		{
			attrib.stride = offset;
		}
	}

}
