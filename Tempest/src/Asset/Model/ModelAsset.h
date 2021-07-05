#pragma once
#include "../IAsset.h"

namespace Tempest
{
	class SampleModelData;

    class ModelAsset : public IAsset
    {
	public:
		ModelAsset(const std::string& assetName, const std::string& filePath);

		SampleModelData* GetModelPtr() const { return model_ptr; }
		bool IsReady() const override		 { return is_ready; }

		bool Load() override;
		void Unload() override;

	private:
		bool is_ready{ false };

		SampleModelData* model_ptr{}; // can change to your prefered data and type
    };

    using model_asset_ptr = std::shared_ptr<ModelAsset>;
}