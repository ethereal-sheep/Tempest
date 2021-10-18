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
#include "../IAsset.h"
#include "TextureLoader.h"

namespace Tempest
{
	class TextureAsset : public IAsset
	{
	public:
		TextureAsset(const std::string& assetName, const std::string& filePath);

		const TextureResourceData& GetTexture() const { return texture_data; }
		bool IsReady() const override				  { return is_ready; }

		bool Load() override;
		void Unload() override;

	private:
		bool is_ready{ false };

		TextureResourceData texture_data{};
	};

	using texture_asset_ptr = std::shared_ptr<TextureAsset>;
}
