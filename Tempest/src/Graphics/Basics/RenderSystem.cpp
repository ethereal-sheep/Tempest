#include "Graphics/Basics/RenderSystem.h"
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
        m_Shaders.emplace_back(std::make_unique<Shader>("Shaders/Basic_vertex.glsl", "Shaders/Basic_fragment.glsl"));
        m_Shaders.emplace_back(std::make_unique<Shader>("Shaders/Line_vertex.glsl", "Shaders/Line_fragment.glsl")); 

        glm::mat4 transform(1.f);
        transform = glm::translate(transform, glm::vec3(0.f, 0.5f, 0.f));
        transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
        transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
        transform = glm::rotate(transform, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
        transform = glm::scale(transform, glm::vec3(1.f, 1.f, 0.f));

        m_Transforms.push_back(transform);
    }

    //void RenderSystem::Submit(MeshCode code, const Transform& transform)
    //{
    //
    //}

    void RenderSystem::Draw()
    {
        BeginFrame();
        Render();
        EndFrame();
        Clear();
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
    }

    void RenderSystem::Render()
    {
        m_Meshes.emplace_back(CreateShape(MeshCode::CUBE));
        m_Shaders[0]->Bind();
        m_Shaders[0]->SetMat4fv(m_Transforms[0], "ModelMatrix");
        m_Shaders[0]->SetMat4fv(m_Camera.GetProjectionMatrix(), "ProjectionMatrix");
        m_Shaders[0]->SetMat4fv(m_Camera.GetViewMatrix(), "ViewMatrix");
        m_Meshes[0].Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
        m_Meshes[0].Unind();
    }

    void RenderSystem::EndFrame()
    {
        Clear();
    }

    void RenderSystem::Clear()
    {
        m_Meshes.clear();
    }

    Camera& RenderSystem::GetCamera()
    {
        return m_Camera;
    }
}