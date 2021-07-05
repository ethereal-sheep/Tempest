#include "TextureLoader.h"
namespace Tempest
{
    TextureResourceData::TextureResourceData(int height, int width, unsigned int id, bool is_valid) :
        height{height},
        width{ width },
        id{id},
        is_valid{ is_valid }
    {
    }

    TextureResourceData TextureLoader::LoadTexture(const char* filename, bool RGBA)
    {
        bool success = false;
        int  width = 100;
        int  height = 100;

        unsigned int texID = 0;

        /* Fill in texture loading function */

        return TextureResourceData(height, width, texID, success);
    }
}