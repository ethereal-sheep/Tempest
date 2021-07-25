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
        Renderer m_Renderer;
        FrameBuffer m_FrameBuffer{ 1600, 900 };
        Model model{ "Models/Sword.fbx" };

    public:

        RenderSystem(uint32_t width, uint32_t height);

        // submit api
        void Submit(MeshCode code, const Transform& transform);
        
        // rendering api
        void Draw();
        void DrawFrameBuffer();
        void BeginFrame();
        void Render();
        void EndFrame();

        void Resize(uint32_t width, uint32_t height);

        Camera& GetCamera();

    private:        
        
        tuptr<Mesh> CreateShape(MeshCode code);
        glm::mat4 Model(const Transform& transform);
        void Clear();
    };
}