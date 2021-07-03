#pragma once
#include "IAsset.h"
#include "Texture/TextureAsset.h"
#include "Audio/AudioAsset.h"
#include "Model/ModelAsset.h"
#include <vector>
#include <unordered_map>

namespace Tempest
{
    class Asset
    {
        ~Asset();

        void CreateContext();

        asset_ptr LoadAsset(std::string assetName, std::string filePath);
        bool UnloadAsset(std::string assetName);

        asset_ptr GetAssetByName(std::string assetName) const;
        texture_asset_ptr GetTextureByName(std::string assetName) const;
        model_asset_ptr GetModelByName(std::string assetName) const;
        audio_asset_ptr GetAudioByName(std::string assetName) const;

        std::vector<asset_ptr> GetAssetsByType(AssetType assetType) const;
        static AssetType GetAssetTypeFromPath(const std::string& filePath);

    private:
        std::unordered_map<size_t, std::vector<asset_ptr>> assets;
        std::unordered_map<std::string, asset_ptr> assets_with_name;
    };
}
