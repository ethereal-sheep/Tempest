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

namespace Tempest
{

    class ModelPBR
    {
    public:
        ModelPBR();
        ~ModelPBR();
        void loadModel(std::string file);
        void Draw();

    private:
        std::vector<MeshPBR> meshes;
        std::string directory;

        void processNode(aiNode* node, const aiScene* scene);
        MeshPBR processMesh(aiMesh* mesh, const aiScene* scene);
    };

}