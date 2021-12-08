/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Asset.h"
#include "Util/magic_enum.hpp"
#include "../Logger/Log.h"
#include <filesystem>

namespace Tempest
{
    Asset::~Asset()
    {
        for (auto& it : assets_with_name)
            it.second->Unload();

        assets_with_name.clear();
        assets.clear();
    }

    void Asset::CreateContext()
    {
        constexpr std::size_t assetTypeCount = magic_enum::enum_count<AssetType>();
        for (size_t i = 0; i < assetTypeCount; ++i)
        {
            assets[i] = tvector<asset_ptr>();
        }
    }

    asset_ptr Asset::LoadAsset(std::string assetName, std::string filePath)
    {
		/* TODO: to replace with imgui popups */
		std::string message;

		// check if file exists
		if (!std::filesystem::exists(filePath))
		{
			message = "Asset filepath " + filePath + " not found!";
			LOG_ERROR(message);
			return nullptr;
		}

		// check if asset is already loaded
		auto existingAsset = assets_with_name.find(assetName);
		if (existingAsset != assets_with_name.end())
		{
			message = "Asset " + assetName + " has already been loaded, skip loading. ";
			LOG_INFO(message);

			return existingAsset->second;
		}

		// load accordingly
		bool loadResult = false;
		asset_ptr asset = nullptr;
		const AssetType assetType = GetAssetTypeFromPath(filePath);

		switch (assetType)
		{
		case AssetType::TEXTURE:
			asset = std::make_shared<TextureAsset>(assetName, filePath);
			break;

		case AssetType::MODEL:
			asset = std::make_shared<ModelAsset>(assetName, filePath);
			break;

		case AssetType::AUDIO:
			asset = std::make_shared<AudioAsset>(assetName, filePath);
			break;

		case AssetType::UNKNOWN_ASSET:
		default:
			LOG_ERROR("Unknown asset loaded");
			return nullptr;
		}

		// load the asset
		loadResult = asset->Load();

		// insert to map successful
		if (loadResult)
		{
			// insert into all containers
			assets[assetType].push_back(asset);
			assets_with_name[assetName] = asset;
			return asset;
		}

		return nullptr;
    }

    bool Asset::UnloadAsset(std::string assetName)
    {
		const auto assetIterator = assets_with_name.find(assetName);

		std::string message = "Could not find given asset: " + assetName;
		LOG_ASSERT_V(assetIterator != assets_with_name.end(), message.c_str());

		auto thisAsset = assetIterator->second;
		thisAsset->Unload(); // Unload asset

		// erase from all containers
		assets_with_name.erase(assetIterator);
		for (auto& [assetType, assetList] : assets)
		{
			for (auto it2 = assetList.begin(); it2 != assetList.end(); ++it2)
			{
				if (it2->get()->GetAssetName() == assetName)
				{
					assetList.erase(it2);
					return true;
				}
			}
		}

		LOG_ERROR("Cannot erase asset");
		return false;
    }

	asset_ptr Asset::GetAssetByName(std::string assetName) const
	{
		if (assetName.empty() || assets_with_name.find(assetName) == assets_with_name.end())
			return nullptr; // could not find the given asset

		return assets_with_name.at(assetName);
	}

	texture_asset_ptr Asset::GetTextureByName(std::string assetName) const
	{
		return std::static_pointer_cast<TextureAsset>(GetAssetByName(assetName));
	}

	model_asset_ptr Asset::GetModelByName(std::string assetName) const
	{
		return std::static_pointer_cast<ModelAsset>(GetAssetByName(assetName));
	}

	audio_asset_ptr Asset::GetAudioByName(std::string assetName) const
	{
		return std::static_pointer_cast<AudioAsset>(GetAssetByName(assetName));
	}

	auto Asset::GetAssetsByType(AssetType assetType) const
	{
		return make_const_range(assets.at(static_cast<size_t>(assetType)));
	}

	AssetType Asset::GetAssetTypeFromPath(const std::string& filePath)
	{
		std::string ext = std::filesystem::path(filePath).extension().string();
		std::for_each(ext.begin(), ext.end(), [](char& c) {
			c = static_cast<char>(::tolower(c));
		});

		/* hardcoded extension formats uwu */

		// texture extensions
		if (ext == ".png" ||
			ext == ".jpg" ||
			ext == ".tga" ||
			ext == ".dds" ||
			ext == ".bmp")
			return AssetType::TEXTURE;

		// model extensions (can fill in preferred)
		if (ext == "fbx")
			return AssetType::MODEL;

		// audio extensions
		if (ext == ".mp3" ||
			ext == ".wav" ||
			ext == ".ogg" ||
			ext == ".flac" ||
			ext == ".midi" ||
			ext == ".aiff")
			return AssetType::AUDIO;

		return AssetType::UNKNOWN_ASSET;
	}
}

