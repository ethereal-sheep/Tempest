/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Graphics/Basics/Material.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Graphics/Basics/Mesh.h"
#include "Util.h"
#include "TMath.h"
#include "Util/Range.h"
#include <glm.hpp>

namespace Tempest
{
	class Mesh;
	struct Material;

	class Model
	{
	public:
		Model() = default;
		Model(const char* file);

		auto GetMeshes() const
		{
			return make_const_range(m_Meshes);
		}

		operator bool() const 
		{
			return m_Meshes.size();
		}

		const tpath& GetFilename() const
		{
			return m_File;
		}

	private:
		void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, const tvector<tsptr<Material>>& materials);		// Nodes
		void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, const tvector<tsptr<Material>>& materials);		// Mesh Data
		//void ProcessMaterialData(); // Material Textures + Material Info

		tpath m_File;
		//tvector<Material> m_Materials;
		tvector<tpair<Mesh, tsptr<Material>>> m_Meshes;

	};
}