/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
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
#include "Animation/AnimationManager.h"
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
        //RenderPipeline m_Pipeline;
        ShadowBuffer m_ShadowBuffer;
        //ShadowMap m_ShadowMap;
        Renderer m_Renderer;
        FBO m_FrameBuffer{ 1600, 900 };
        AnimationManager m_Animation;

        int  GammaCorrection = 1;
        bool GridActive = false;

        void InitMeshes();
        void InitShaders();
        void InitBuffers();

    public:
        RenderPipeline m_Pipeline;
        RenderSystem(uint32_t width, uint32_t height);
        ~RenderSystem() = default;
        // submit api
        void Submit(MeshCode code, const Transform& transform);                             // Submitting Primitives
        void SubmitModel(const string& path, const Transform& transform);                   // Submitting Models via file path
        void SubmitModel(const string& path, const glm::mat4& model_matrix);
        void SubmitModel(const string& path, const glm::mat4& model_matrix, vec3 color);
        void SubmitModel(const string& path, const Transform& transform, std::string anim, uint32_t id);
        void SubmitModel(const string& path, const glm::mat4& model_matrix, std::string anim, uint32_t id);
        void SubmitCamera(const Camera& camera);                                            // Submitting Cameras
        void SubmitLights(const Directional_Light& dilight, const Transform& transform);    // Submitting Directional Light {Transform to be used for pos}
        void SubmitLights(const Point_Light& plight);                                       // Submitting Point Light {Transform to be used for pos}
        void SubmitLights(const SpotLight& slight, const Transform& transform);             // Submitting SpotLight {Transform to be used for pos}

        void DrawLine(const Line& line, const glm::vec4& color);                            // Drawing Lines
        void DrawLine(const AABB& box, const glm::vec4& color);                             // Drawing Bounding Boxes
        
        // rendering api
        void Draw();
        void DrawFrameBuffer();
        void BeginFrame();
        void Render();
        void EndFrame();

        void UpdateAnimation(float dt);

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
        uint32_t NumPLight = 0;

        // To be changed to objects instead of global values
        float shininess = 32.f;
        float ambientStrength = 0.5f;
        float specularStrength = 0.5f;
        const GLfloat near_plane = 1.0f, far_plane = 20.0f;//25.0f;
        float gammaValue = 2.2f; // 1.0f to 4.0f

        //Testing
        GLuint gBuffer, zBuffer, gPosition, gNormal, gAlbedo, gEffects;
        GLuint saoFBO, saoBlurFBO, saoBuffer, saoBlurBuffer;
        GLuint postprocessFBO, postprocessBuffer;
        GLuint envToCubeFBO, irradianceFBO, prefilterFBO, brdfLUTFBO, envToCubeRBO, irradianceRBO, prefilterRBO, brdfLUTRBO;
        GLuint gBuffer2, gBuffer2t;
        
        GLint gBufferView = 1;     // To see the different buffers
        GLint tonemappingMode = 2; // Tonemapping types 
        GLint attenuationMode = 2; // Attenuation type

        // Motion Blur settings
        GLint motionBlurMaxSamples = 32;

        // Just literally only used for auto rotation ( can be removed - remove in rendersystem.cpp too)
        GLfloat deltaTime = 0.0f;

        // SAO
        GLfloat saoRadius = 0.762f;
        GLfloat saoBias = 0.001f;
        GLfloat saoScale = 0.713f;
        GLfloat saoContrast = 0.836f;
        GLint saoSamples = 42;
        GLint saoTurns = 6;
        GLint saoBlurSize = 6;

        // Additonal Camera things
        GLfloat cameraAperture = 16.0f;
        GLfloat cameraShutterSpeed = 0.5f;
        GLfloat cameraISO = 1000.0f;

        GLfloat modelRotationSpeed = 1.0f;
        GLfloat fps = 60.f;

        // Mode toggles
        bool pointMode = true;         // Point light 
        bool directionalMode = true;  // Directional Light
        bool iblMode = true;          // Image based lighting 
        bool saoMode = true;          // SAO 
        bool fxaaMode = true;         // FXAA
        bool motionBlurMode = false;   // Motion Blur
        bool dirShadowBool = true;    // Direcitonal shadows toggle
        bool pointShadowBool = true;   // Point light shadows toggle
        bool pbrMode = false;
        bool envMapShow = false;        // Envmap toggle
        bool tiltShiftMode = false;

        bool AAgridShow = true;
        bool TestPBR = false;
        bool USO = false;

        glm::vec4 USOcolor{ 0.0f };
        //bool captured = false;
        //vec2 vp_size = vec2(0.f);
        glm::vec4 clearColor{0.4f, 0.5f, 0.6f, 1.0f};


        glm::vec3 materialF0 = glm::vec3(0.04f);  // UE4 dielectric

        // 1 fixed model first
        glm::vec3 modelPosition = glm::vec3(0.0f);      
        glm::vec3 modelRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 modelScale = glm::vec3(0.1f);


        // Model 1
        ModelPBR objectModel;

        TexturePBR objectAlbedo;
        TexturePBR objectNormal;
        TexturePBR objectRoughness;
        TexturePBR objectMetalness;
        TexturePBR objectAO;
        glm::mat4 model1;
        
        glm::mat4 projViewModel;
        glm::mat4 prevProjViewModel = projViewModel;

        GLfloat materialRoughness = 0.01f;   // Global for now (but should be per object/material).
        GLfloat materialMetallicity = 0.02f; // Global for now (but should be per object/material).
        GLfloat ambientIntensity = 0.005f;   // Global for now (but should be per object/material).

        ShapePBR quadRender; // FBO to draw the scene

        // Enviroment Map 
        ShapePBR envCubeRender; // FBO to draw env
        TexturePBR envMapHDR;
        TexturePBR envMapCube;
        TexturePBR envMapIrradiance;
        TexturePBR envMapPrefilter;
        TexturePBR envMapLUT;
        glm::mat4 envMapProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        
        void iblSetup();

        uint32_t getHeight();
        uint32_t getWidth();


        void LoadTextures();

        bool PREFABMODE = false;

        // ANIMATION TESTING
        //ModelPBR model;

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