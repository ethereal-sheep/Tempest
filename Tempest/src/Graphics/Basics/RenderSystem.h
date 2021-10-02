#pragma once
#include "Graphics/Basics/Mesh.h"
#include "Core.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/Camera.h"
#include "ECS/Components/Components.h"
#include "Graphics/Basics/RenderPipeline.h"
#include "Graphics/Basics/FrameBuffer.h"
#include "Graphics/Basics/Model.h"
#include "Graphics/Basics/LineRenderer.h"
#include "Graphics/Basics/Lights.h"
#include "Graphics/OpenGL/Texture.h"
#include "Graphics/Basics/ShadowBuffer.h"

/**
 * @brief 
 * @param RenderSystem Umbrella interface  
 * @param RenderPipeline Holds all the data structures that contains stuff to be used for rendering
 * @param Renderer Wrapper for opengl (culling, depth functions, etc.)
 * 
 */

namespace Tempest
{
	class RenderSystem
    {    
        using Transform = tc::Transform;

        //FontRenderer m_FontR;
        LineRenderer m_LineRenderer;
        RenderPipeline m_Pipeline;
        ShadowBuffer m_ShadowBuffer;
        //ShadowMap m_ShadowMap;
        Renderer m_Renderer;
        FrameBuffer m_FrameBuffer{ 1600, 900 };

        int  GammaCorrection = 1;
        bool GridActive = false;

        void InitMeshes();
        void InitShaders();
        void InitBuffers();

    public:

        RenderSystem(uint32_t width, uint32_t height);

        // submit api
        void Submit(MeshCode code, const Transform& transform);                             // Submitting Primitives
        void SubmitModel(const string& path, const Transform& transform);                   // Submitting Models via file path
        void SubmitCamera(const Camera& camera);                                            // Submitting Cameras
        void SubmitLights(const Directional_Light& dilight, const Transform& transform);    // Submitting Directional Light {Transform to be used for pos}
        void SubmitLights(const Point_Light& plight, const Transform& transform);           // Submitting Point Light {Transform to be used for pos}
        void SubmitLights(const SpotLight& slight, const Transform& transform);             // Submitting SpotLight {Transform to be used for pos}
        
        // rendering api
        void Draw();
        void DrawFrameBuffer();
        void BeginFrame();
        void Render();
        void EndFrame();

        void Resize(uint32_t width, uint32_t height);
        void RenderGrid(bool state);

        Camera& GetCamera();

        int GetActivePt_lightsNum();
        int GetGammaCorrection(); 
        std::vector<Directional_Light> dir_lights;
        std::vector<Point_Light> pt_lights;
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;

        uint32_t MAX_POINT_LIGHT = 10;

        // To be changed to objects instead of global values
        float shininess = 32.f;
        float ambientStrength = 0.05f;
        float specularStrength = 0.5f;


    private:        
       
        glm::mat4 to_Model_Matrix(const Transform& transform);

        void Clear();                                                                                        // Clear Pipeline
        void DrawSprites(MeshCode code, ShaderCode shaderType, int pt_light_num = -1);                                        // Render Sprites of different meshes
        void DrawSprites(const tuptr<Shader>& shader, const tvector<SpriteObj>& sprites, MeshCode code, ShaderCode shaderType, int pt_light_num = -1);     // Render Sprites of different meshes
        void RenderAAGrid();                                                                                 // Render Anti-Aliased Grid
    };
}