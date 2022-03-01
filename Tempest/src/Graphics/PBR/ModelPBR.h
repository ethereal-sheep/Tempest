/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
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
#include "Animation/Animation.h"

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

        tomap<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }

        std::vector<MeshPBR> meshes;
        std::vector<glm::vec3> colours;
        std::vector<uint32_t> mats;
        std::vector<TexturePBR> mm;
        tomap<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;
        bool HasAnimation = false;

        std::unordered_map<std::string, Animation> animations;

    private:
        
        std::string directory;

        void processNode(aiNode* node, [[maybe_unused]] const aiScene* scene);
        MeshPBR processMesh(aiMesh* mesh);
        void SetVertexBoneDataToDefault(Vertex& vertex);
        void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
        bool ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh);

        const int MAX_BONE_INFLUENCE = 4;
    };

}