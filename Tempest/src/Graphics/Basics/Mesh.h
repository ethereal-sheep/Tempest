/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Graphics/Basics/VertexArray.h"
#include "Graphics/Basics/GeometryFactory.h"

namespace Tempest
{
    enum struct MeshCode
    {
        PLANE,
        SPHERE,
        CUBE,
        ICOSAHEDRON
    };

    class Mesh
    {
        tsptr<VertexArray> m_VAO;
        MeshCode m_Code;
        uint32_t m_Count = 0;

    public:

        Mesh() = default;
        Mesh(const std::pair<Vertices, Indices>& data);
        
        //Mesh(const Mesh& m);

        void Bind() const;
        void Unbind() const;

        uint32_t GetVertexCount() const;
        tsptr<VertexArray> GetVertexArray();
        MeshCode GetMeshCode() const;
    };

}