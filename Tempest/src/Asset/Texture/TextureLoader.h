/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace Tempest
{
	struct TextureResourceData
	{
		TextureResourceData() = default;
		TextureResourceData(int height, int width, unsigned int id, bool valid);
		int height;
		int width;
		unsigned int id;
		bool is_valid; // whether this texture is loaded successfully
	};

	struct TextureLoader
	{
		static TextureResourceData LoadTexture(const char* filename, bool RGBA = false);
	};
}
