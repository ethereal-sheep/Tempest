#pragma once
#include <glew.h>
#include "Core.h"
#include "Util.h"

namespace Tempest
{
	class ShadowMap
	{
		uint32_t m_ID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	public:

		ShadowMap(uint32_t width = 1024, uint32_t height = 1024);
		~ShadowMap();

		uint32_t GetID() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
	};
}