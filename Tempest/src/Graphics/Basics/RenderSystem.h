#pragma once
#include "Graphics/Basics/Mesh.h"
#include "Core.h"
#include "Graphics/OpenGL/Shader.h"
#include "ECS/Components/Components.h"
#include "Graphics/OpenGL/Renderer.h"
#include "Graphics/OpenGL/Camera.h"

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
        using ShaderLibrary = tvector<tuptr<Shader>>;
        using MeshLibrary = tvector<Mesh>;
        //using Transform = Tempest::Components::Transform;

        //FontRenderer m_FontR;
        //LineRenderer m_LineR 
        //GenericRenderer m_GenR;
        //RenderPipeline m_Pipeline;
        ShaderLibrary m_Shaders;
        MeshLibrary m_Meshes; // actually not sure if need this
        Camera m_Camera;
        tvector<glm::mat4> m_Transforms;
        // because we implement share pointers but we never use
        // we can just do factory

        // wait i need think, this maybe need rethink
        // thats okay but the issue is no need for shared pointer
        // only thing is we can copy existing shapes only
        // but new shapes are just allocating new memory
        // unless we store the meshes, vbo or ibo somewhere its useless
        // to have shared ptr
        // nvm we go with current first then we reassess on wed
        // current rendering maintain since we can render our shapes le
        // yeah okay, you can commit but exclude ver 2 from the project first


        //enum struct ShaderCode
        //{
        //    SHADER_ONE,
        //    SHADER_TWO ...
        //}
        //
        //enum struct MeshCode
        //{
        //    CUBE,
        //    CIRCLE,
        //    blabla
        //}
        
        
        Mesh CreateShape(MeshCode code);
        Renderer m_Renderer;

    public:
        RenderSystem(uint32_t width, uint32_t height);

        // submit api
        //void Submit(MeshCode code, const Transform& transform);

        // rendering api
        void Draw();
        void BeginFrame();
        void Render();
        void EndFrame();

        Camera& GetCamera();

    private:        
        
        void Clear();
    };
}