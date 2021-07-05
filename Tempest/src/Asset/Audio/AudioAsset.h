#pragma once
#include "../IAsset.h"

namespace Tempest
{
	namespace FMOD
	{
		class Sound;
	}

	class AudioAsset : public IAsset
	{
	public:
		AudioAsset(const std::string& assetName, const std::string& filePath);

		FMOD::Sound* GetSoundPtr() const { return sound_ptr; }
		bool IsReady() const override	 { return is_ready; }

		bool Load() override;
		void Unload() override;

	private:
		bool is_ready{ false };

		FMOD::Sound* sound_ptr{};
	};

	using audio_asset_ptr = std::shared_ptr<AudioAsset>;
}