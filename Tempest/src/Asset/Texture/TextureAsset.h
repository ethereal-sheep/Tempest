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
