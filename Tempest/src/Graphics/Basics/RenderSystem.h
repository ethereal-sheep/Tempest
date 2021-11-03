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
#include "Graphics/Basics/Mesh.h"
#include "Core.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/Camera.h"
#include "ECS/Components/Components.h"
#include "Graphics/Basics/RenderPipeline.h"
#include "Graphics/Basics/FBO.h"
#include "Graphics/Basics/Model.h"
#include "Graphics/Basics/LineRenderer.h"
#include "Graphics/Basics/Lights.h"
#include "Graphics/OpenGL/Texture.h"
#include "Graphics/Basics/ShadowBuffer.h"

#include "Graphics/PBR/ShapePBR.h"

#include "Graphics/PBR/TexturePBR.h"

#include "Graphics/PBR/MaterialPBR.h"

#include "Graphics/PBR/ModelPBR.h"
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
        FBO m_FrameBuffer{ 1600, 900 };

        int  GammaCorrection = 1;
        bool GridActive = false;

        void InitMeshes();
        void InitShaders();
        void InitBuffers();

    public:

        RenderSystem(uint32_t width, uint32_t height);
        ~RenderSystem() = default;
        // submit api
        void Submit(MeshCode code, const Transform& transform);                             // Submitting Primitives
        void SubmitModel(const string& path, const Transform& transform);                   // Submitting Models via file path
        void SubmitCamera(const Camera& camera);                                            // Submitting Cameras
        void SubmitLights(const Directional_Light& dilight, const Transform& transform);    // Submitting Directional Light {Transform to be used for pos}
        void SubmitLights(const Point_Light& plight, const Transform& transform);           // Submitting Point Light {Transform to be used for pos}
        void SubmitLights(const SpotLight& slight, const Transform& transform);             // Submitting SpotLight {Transform to be used for pos}

        void DrawLine(const Line& line, const glm::vec4& color);                            // Drawing Lines
        void DrawLine(const AABB& box, const glm::vec4& color);                             // Drawing Bounding Boxes
        
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
        void SetGammaCorrection(int);
        std::vector<Directional_Light> dir_lights;
        std::vector<Point_Light> pt_lights;
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;

        uint32_t MAX_POINT_LIGHT = 10;

        // To be changed to objects instead of global values
        float shininess = 32.f;
        float ambientStrength = 0.05f;
        float specularStrength = 0.5f;
        const GLfloat near_plane = 1.0f, far_plane = 25.0f;

        //Testing
        GLuint screenQuadVAO, screenQuadVBO;
        GLuint gBuffer, zBuffer, gPosition, gNormal, gAlbedo, gEffects;
        GLuint saoFBO, saoBlurFBO, saoBuffer, saoBlurBuffer;
        GLuint postprocessFBO, postprocessBuffer;
        GLuint envToCubeFBO, irradianceFBO, prefilterFBO, brdfLUTFBO, envToCubeRBO, irradianceRBO, prefilterRBO, brdfLUTRBO;

        GLint gBufferView = 1;
        GLint tonemappingMode = 1;
        GLint lightDebugMode = 3;
        GLint attenuationMode = 2;
        GLint saoSamples = 12;
        GLint saoTurns = 7;
        GLint saoBlurSize = 4;
        GLint motionBlurMaxSamples = 32;

        //GLfloat lastX = 1600 / 2;
        //GLfloat lastY = 900 / 2;
        GLfloat deltaTime = 0.0f;
        //GLfloat lastFrame = 0.0f;
        //GLfloat deltaGeometryTime = 0.0f;
        //GLfloat deltaLightingTime = 0.0f;
        //GLfloat deltaSAOTime = 0.0f;
        //GLfloat deltaPostprocessTime = 0.0f;
        //GLfloat deltaForwardTime = 0.0f;
        //GLfloat deltaGUITime = 0.0f;
        GLfloat materialRoughness = 0.01f;
        GLfloat materialMetallicity = 0.02f;
        GLfloat ambientIntensity = 0.005f;
        GLfloat saoRadius = 0.3f;
        GLfloat saoBias = 0.001f;
        GLfloat saoScale = 0.7f;
        GLfloat saoContrast = 0.8f;
        GLfloat lightPointRadius1 = 3.0f;
        GLfloat lightPointRadius2 = 3.0f;
        GLfloat lightPointRadius3 = 3.0f;
        GLfloat cameraAperture = 16.0f;
        GLfloat cameraShutterSpeed = 0.5f;
        GLfloat cameraISO = 1000.0f;
        GLfloat modelRotationSpeed = 1.0f;
        GLfloat fps = 60.f;

        bool cameraMode;
        bool pointMode = true;
        bool directionalMode = false;
        bool iblMode = false;
        bool saoMode = false;
        bool fxaaMode = false;
        bool motionBlurMode = false;
        bool screenMode = false;
        bool firstMouse = true;
        bool guiIsOpen = true;
        bool keys[1024];

        glm::vec3 albedoColor = glm::vec3(1.0f);
        glm::vec3 materialF0 = glm::vec3(0.04f);  // UE4 dielectric


        // 1 fixed model first
        glm::vec3 modelPosition = glm::vec3(0.0f);
        glm::vec3 modelPosition2 = glm::vec3(0.0f);
        glm::vec3 modelRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 modelScale = glm::vec3(0.1f);

        glm::mat4 projViewModel;
        glm::mat4 prevProjViewModel = projViewModel;
        glm::mat4 envMapProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        ShapePBR quadRender;
        ShapePBR envCubeRender;

        ModelPBR objectModel;

        TexturePBR objectAlbedo;
        TexturePBR objectNormal;
        TexturePBR objectRoughness;
        TexturePBR objectMetalness;
        TexturePBR objectAO;

        ModelPBR objectModel2;

        TexturePBR objectAlbedo2;
        TexturePBR objectNormal2;
        TexturePBR objectRoughness2;
        TexturePBR objectMetalness2;
        TexturePBR objectAO2;


        TexturePBR envMapHDR;
        TexturePBR envMapCube;
        TexturePBR envMapIrradiance;
        TexturePBR envMapPrefilter;
        TexturePBR envMapLUT;

        MaterialPBR pbrMat;



        
        void iblSetup();

        bool dirShadowBool = true;

        // to de
        glm::mat4 model1;
        glm::mat4 model2;
    private:        
       
        glm::mat4 to_Model_Matrix(const Transform& transform);

        void Clear();                                                                                        // Clear Pipeline
        void DrawSprites(MeshCode code, ShaderCode shaderType, int pt_light_num = -1);                                        // Render Sprites of different meshes
        void DrawSprites(const tuptr<Shader>& shader, const tvector<SpriteObj>& sprites, MeshCode code, ShaderCode shaderType, int pt_light_num = -1);     // Render Sprites of different meshes
        void RenderAAGrid();                                                                                 // Render Anti-Aliased Grid

        void gBufferSetup();
        void saoSetup();
        void postprocessSetup();
    };
}