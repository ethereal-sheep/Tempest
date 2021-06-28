#pragma once


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
        using ShaderLibrary = tvector<Shader>;

        FontRenderer m_FontR;
        LineRenderer m_LineR 
        GenericRenderer m_GenR;
        RenderPipeline m_Pipeline;
        ShaderLibrary m_Shaders;
        MeshLibrary m_Meshes; // actually not sure if need this
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
        tsptr<Mesh> CreateShape()


        enum struct ShaderCode
        {
            SHADER_ONE,
            SHADER_TWO ...
        }

        enum struct MeshCode
        {
            CUBE,
            CIRCLE,
            blabla
        }

    public:
        RenderSystem()
        {
            // initialize shaders
            m_Shaders.push_back("",""); // shader 1
            m_Shaders.push_back("",""); // shader 2
            m_Shaders.push_back("",""); // shader 3
            m_Shaders.push_back("",""); // shader 4
            m_Shaders.push_back("",""); // shader 5

            m_Meshes.pushback(some fk shit)
        }

        // submit api
        void Submit(some data);
        void Submit(some data1);
        void Submit(some data2);
        void Submit(some data3);

        // rendering api
        void Draw()
        {
            BeginFrame();
            ProcessFkShit();
            EndFrame();
            Clear();
        }
    private:        
        void Clear()
        {

        }
    };
}