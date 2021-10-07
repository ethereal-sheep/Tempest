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
#include "IAsset.h"
#include "Texture/TextureAsset.h"
#include "Audio/AudioAsset.h"
#include "Model/ModelAsset.h"
#include "../Core.h"
#include "Util/range.h"

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

        auto GetAssetsByType(AssetType assetType) const;
        static AssetType GetAssetTypeFromPath(const std::string& filePath);

    private:
        tmap<size_t, tvector<asset_ptr>> assets; //enum Asset type : texture 
        tmap<std::string, asset_ptr> assets_with_name;
    };
}
