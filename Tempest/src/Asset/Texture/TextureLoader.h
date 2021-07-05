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
