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
#include "Core.h"

#include "glew.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/PBR/MeshPBR.h"
#include "Graphics/PBR/MaterialPBR.h"

#include <map>

namespace Tempest
{

    class ModelPBR
    {
    public:
        ModelPBR();
        ~ModelPBR();
        void loadModel(std::string file);
        void Draw();

        std::vector<MeshPBR> meshes;
        std::vector<glm::vec3> colours;
        std::vector<uint32_t> mats;
        std::vector<TexturePBR> mm;
    private:
        
        std::string directory;

        void processNode(aiNode* node, [[maybe_unused]] const aiScene* scene);
        MeshPBR processMesh(aiMesh* mesh);
    };

}