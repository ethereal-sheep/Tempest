#include "AudioAsset.h"
#include "Logger/Log.h"

namespace Tempest
{
    AudioAsset::AudioAsset(const std::string& assetName, const std::string& filePath) :
        IAsset{ AssetType::AUDIO, assetName, filePath }
    {
    }

    bool AudioAsset::Load()
    {
        /* Fill in function for sound loading */
        /* Might wanna do an audio handler instead of audio loader */

        is_ready = sound_ptr != nullptr;

        LOG_ASSERT_V(is_ready, "Audio Asset failed to load!");
        return is_ready;
    }

    void AudioAsset::Unload()
    {
        /* Release the sound if ready */

        is_ready = false;
    }
}