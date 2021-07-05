#include "TextureAsset.h"
#include "TextureLoader.h"
#include "Logger/Log.h"

namespace Tempest
{
    TextureAsset::TextureAsset(const std::string& assetName, const std::string& filePath) :
        IAsset{ AssetType::TEXTURE, assetName, filePath }
    {
    }

    bool TextureAsset::Load()
    {
        texture_data = TextureLoader::LoadTexture(file_path.c_str(), true);
        is_ready = texture_data.is_valid && texture_data.id;

        LOG_ASSERT_V(is_ready, "Texture Asset failed to load!");
        return is_ready;
    }

    void TextureAsset::Unload()
    {
        // if (is_ready), delete texture

        is_ready = false;
    }
}