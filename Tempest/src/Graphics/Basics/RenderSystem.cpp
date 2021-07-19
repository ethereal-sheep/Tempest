#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"
#include "Logger/Log.h"


namespace Tempest
{
    Mesh RenderSystem::CreateShape(MeshCode code)
    {
        switch (code)
        {
            case MeshCode::CUBE:            return Mesh(GeometryFactory::GenerateIndexedCube(1, 1));
            case MeshCode::PLANE:           return Mesh(GeometryFactory::GenerateIndexedPlane(1, 1));
            case MeshCode::SPHERE:          return Mesh(GeometryFactory::GenerateIndexedSphere(1, 16, 16));
            case MeshCode::ICOSAHEDRON:     return Mesh(GeometryFactory::GenerateIndexedIcosahedron());
            default: LOG_ASSERT("Unknown Shape");
        }
    }

    RenderSystem::RenderSystem(uint32_t width, uint32_t height)
    {
        m_Renderer.SetViewport(0, 0, width, height);

        m_Pipeline.m_Shaders.emplace(ShaderCode::BASIC, std::make_unique<Shader>("Shaders/Basic_vertex.glsl", "Shaders/Basic_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::LINE, std::make_unique<Shader>("Shaders/Line_vertex.glsl", "Shaders/Line_fragment.glsl")); 
        m_Pipeline.m_Shaders.emplace(ShaderCode::FRAMEBUFFER, std::make_unique<Shader>("Shaders/FrameBuffer_vertex.glsl", "Shaders/FrameBuffer_fragment.glsl"));

        m_Pipeline.m_Cameras.emplace_back(Camera{});
    }

    void RenderSystem::Submit(MeshCode code, const Transform& transform)
    {
        m_Pipeline.m_Transforms.push_back(Model(transform));

        if (m_Pipeline.m_Meshes.find(code) == m_Pipeline.m_Meshes.end())
        {
            m_Pipeline.m_Meshes.emplace(std::make_pair(code, CreateShape(code)));
        }

        m_Pipeline.m_Sprites.push_back(code);
    }

    glm::mat4 RenderSystem::Model(const Transform& transform)
    {
        glm::mat4 model(1.f);
        
        glm::mat4 translate = glm::translate(glm::vec3(transform.position.x, transform.position.y, transform.position.z));
        glm::mat4 rotate(transform.rotation);
        glm::mat4 scale = glm::scale(glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
        
        model = translate * rotate * scale;
        return model;
    }

    void RenderSystem::Draw()
    {
        BeginFrame();
        Render();
        EndFrame();
        Clear();
    }

    void RenderSystem::DrawFrameBuffer()
    {
        m_Pipeline.m_Shaders[ShaderCode::FRAMEBUFFER]->Bind();
        glBindVertexArray(m_FrameBuffer.GetVertexArray());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FrameBuffer.GetIndexBuffer());
        glBindTexture(GL_TEXTURE_2D, m_FrameBuffer.GetColourBuffer());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void RenderSystem::BeginFrame()
    {
        m_Renderer.EnableDepthMask(true);
        m_Renderer.EnableDepthTest(true);
        m_Renderer.EnableCulling(false, false, false);
        m_Renderer.EnableBlend(true);
        //m_Renderer.EnableCulling(true, true, true);
        //m_Renderer.SetPolygonMode(PolyMode::FILL);
        
        m_Renderer.ClearColour(0.5f, 0.5f, 0.5f, 0.0f);
        m_Renderer.ClearColorDepth();

        //m_FrameBuffer.Bind();
    }

    void RenderSystem::Render()
    {
        Transform transform;
        transform.position = vec3(-0.5f, 0.f, 0.f);
        transform.rotation = quat(0.f, 0.f, 0.f, 0.f);
        transform.scale = vec3(0.1f, 0.1f, 0.1f);
        Submit(MeshCode::ICOSAHEDRON, transform);

        for (size_t i = 0; i < m_Pipeline.m_Sprites.size(); ++i)
        {
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Transforms[0], "ModelMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
            m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[i]).Bind();
            glDrawElements(GL_TRIANGLES, m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[i]).GetVertexCount(), GL_UNSIGNED_INT, NULL);
        }

        //m_FrameBuffer.Unbind();
    }

    void RenderSystem::EndFrame()
    {
        //m_FrameBuffer.Draw();
    }

    void RenderSystem::Clear()
    {
        m_Pipeline.m_Meshes.clear();
    }

    void RenderSystem::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
            return;

        m_FrameBuffer.Resize(width, height);
        m_Renderer.SetViewport(0, 0, width, height);

        for (auto& i : m_Pipeline.m_Cameras)
        {
            i.SetViewport(0, 0, width, height);
        }
    }

    Camera& RenderSystem::GetCamera()
    {
        return m_Pipeline.m_Cameras.front();
    }
}