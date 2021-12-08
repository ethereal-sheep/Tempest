/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ModelAsset.h"
#include "Logger/Log.h"

namespace Tempest
{
    ModelAsset::ModelAsset(const std::string& assetName, const std::string& filePath) :
        IAsset{ AssetType::MODEL, assetName, filePath }
    {
    }

    bool ModelAsset::Load()
    {
        /* Fill in function for model loading, can use assimp here */
        is_ready = model_ptr != nullptr; // can replace with preferred data and type

        LOG_ASSERT_V(is_ready, "Model Asset failed to load!");
        return is_ready;
    }

    void ModelAsset::Unload()
    {
        /* Unload the model if ready */

        is_ready = false;
    }
}