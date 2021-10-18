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
#include <memory>
#include <string>

namespace Tempest
{
	enum AssetType
	{
		UNKNOWN_ASSET,
		TEXTURE,
		MODEL,
		AUDIO
	};

	class IAsset
	{
	public:
		virtual ~IAsset() = default;

		IAsset(AssetType assetType, std::string assetName, std::string filePath) :
			asset_type{ assetType },
			asset_name{ assetName },
			file_path { filePath  }
		{
		}

		// Returns TRUE if Asset is ready to be used or FALSE if it is still loading
		virtual bool IsReady() const = 0;
		virtual bool Load()          = 0;
		virtual void Unload()        = 0;

		AssetType GetAssetType() const		 { return asset_type; }
		std::string GetAssetName() const	 { return asset_name; }
		std::string GetAssetFilePath() const { return file_path; }

	protected:
		AssetType asset_type;
		std::string asset_name;
		std::string file_path;

	};

	using asset_ptr = std::shared_ptr<IAsset>;
}
