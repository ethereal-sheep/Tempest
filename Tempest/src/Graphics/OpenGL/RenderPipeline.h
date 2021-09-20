//#pragma once
//#include <TMath.h>
//#include "Graphics/OpenGL/VertexBuffer.h"
//#include "Graphics/OpenGL/Skybox.h"
//#include "Graphics/OpenGL/Mesh.h"
//#include "Graphics/OpenGL/Material.h"
//#include "Graphics/OpenGL/IndexBuffer.h"
//#include "Graphics/OpenGL/VertexArray.h"
//#include "Graphics/OpenGL/Camera.h"
//
//namespace Tempest
//{
//	struct CameraUnit
//	{
//		glm::vec3 viewportPosition;
//		glm::mat4 viewProjectionMatrix;
//		//glm::mat4 invViewProjectionMatrix;
//		//glm::mat4 staticViewProjectionMatrix;
//		glm::mat3 inverseRotation;
//		glm::mat4 orthoProjectionMatrix;
//
//	};
//
//	struct DebugBufferMesh
//	{
//		VertexArray* vao = nullptr;
//		VertexBuffer* indirect = nullptr;
//	};
//
//	struct SpriteMesh
//	{
//		Mesh cube = Mesh::GenerateIndexedCube(1, 1);
//		VertexBuffer vbo;
//	};
//
//	struct MaterialUnit
//	{
//		float transparency = 1.f;
//	};
//
//	struct SpriteUnit
//	{
//		glm::mat4 modelTransform;
//		glm::mat3 normalMatrix;
//		glm::vec4 texMinMax;
//		glm::vec3 colour;
//		float transparency;
//	};
//
//	enum struct Shape
//	{
//		SHAPE_SPHERE,
//		SHAPE_CUBE,
//		SHAPE_PLANE,
//		SHAPE_ICOSAHEDRON
//	};
//
//	struct RenderPipeline
//	{
//		DebugBufferMesh m_Debug;
//		std::vector<SpriteUnit> sprites;
//		std::vector<Camera> cameras;
//		std::vector<Shape> shapes;
//		std::vector<glm::mat4> transforms;
//
//		VertexBuffer shaderStorage;
//		VertexBuffer indirect;
//
//		std::vector<MaterialUnit> materialUnits;
//	};
//}