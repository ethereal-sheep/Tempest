/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Graphics/OpenGL/Texture.h"

namespace Tempest
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		~RenderBuffer();

		RenderBuffer(const RenderBuffer&) = delete;
		RenderBuffer& operator=(const RenderBuffer&) = delete;

		void InitStorage(uint32_t width, uint32_t height, uint32_t samples = 0) const;

		uint32_t GetID() const;
		void Bind() const;
		void Unbind() const;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		uint32_t GetSamples() const;

	private:
		uint32_t m_ID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_Samples = 0;
	};
}