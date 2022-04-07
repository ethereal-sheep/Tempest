#include "MeshPrimitives.h"

namespace Tempest
{
	MeshPrim::MeshPrim(const std::pair<Vertices, Indices>& data)
	{
        auto [vert, ind] = data;
        for (auto i = 0; i < vert.position.size(); ++i)
        {
            mVertex mVert;
            mVert.Position = vert.position[i];
            mVert.Normal = vert.normal[i];
            mVert.TexCoords = vert.texCoord[i];

            vertices.push_back(mVert);
        }

        for (auto& j : ind)
            indices.push_back(j);

        setupMesh();
	}

	MeshPrim::~MeshPrim()
	{

	}

	void MeshPrim::Draw()
	{
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}

	void MeshPrim::setupMesh()
	{
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mVertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex), (GLvoid*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex), (GLvoid*)offsetof(mVertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mVertex), (GLvoid*)offsetof(mVertex, TexCoords));

        glBindVertexArray(0);
	}
}