#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"


#include "Logger/Log.h"


namespace Tempest
{
    tuptr<Mesh> RenderSystem::CreateShape(MeshCode code)
    {
        switch (code)
        {
            case MeshCode::CUBE:            return std::make_unique<Mesh>(GeometryFactory::GenerateIndexedCube(1, 1));
            case MeshCode::PLANE:           return std::make_unique<Mesh>(GeometryFactory::GenerateIndexedPlane(1, 1));
            case MeshCode::SPHERE:          return std::make_unique<Mesh>(GeometryFactory::GenerateIndexedSphere(1, 16, 16));
            case MeshCode::ICOSAHEDRON:     return std::make_unique<Mesh>(GeometryFactory::GenerateIndexedIcosahedron());
            default: LOG_ASSERT("Unknown Shape");
        }
    }

    RenderSystem::RenderSystem(uint32_t width, uint32_t height)
    {
        m_Renderer.SetViewport(0, 0, width, height);

        m_Pipeline.m_Shaders.emplace(ShaderCode::BASIC, std::make_unique<Shader>("Shaders/Basic_vertex.glsl", "Shaders/Basic_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::TEXTURE, std::make_unique<Shader>("Shaders/Texture_vertex.glsl", "Shaders/Texture_fragment.glsl")); 
        m_Pipeline.m_Shaders.emplace(ShaderCode::GROUND, std::make_unique<Shader>("Shaders/GroundPlane_vertex.glsl", "Shaders/GroundPlane_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::DIRECTIONAL_LIGHT, std::make_unique<Shader>("Shaders/DirectionalLight_vertex.glsl", "Shaders/DirectionalLight_fragment.glsl"));
        
        if(m_Pipeline.m_Cameras.empty())
            m_Pipeline.m_Cameras.emplace_back(Camera{});

        //dir_lights.emplace_back(Directional_Light{});
    }

    void RenderSystem::Submit(MeshCode code, const Transform& transform)
    {
        SpriteObj sprite;
        sprite.m_Code = code;
        sprite.m_Transform = to_Model_Matrix(transform);
        m_Pipeline.m_Sprites.emplace_back(sprite);

        if (m_Pipeline.m_Meshes.find(code) == m_Pipeline.m_Meshes.end())
        {
            m_Pipeline.m_Meshes.emplace(std::make_pair(code, CreateShape(code)));
        }
    }

    void RenderSystem::SubmitModel(const string& path, const Transform& transform)
    {
        m_Pipeline.m_Models.emplace_back(std::make_shared<Model>(path.c_str()));
        m_Pipeline.m_ModelTransforms.emplace_back(to_Model_Matrix(transform));
    }

    glm::mat4 RenderSystem::to_Model_Matrix(const Transform& transform)
    {
        glm::mat4 mdl(1.f);
        
        glm::mat4 translate = glm::translate(glm::vec3(transform.position.x, transform.position.y, transform.position.z));
        glm::mat4 rotate(transform.rotation);
        glm::mat4 scale = glm::scale(glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
        
        mdl = translate * rotate * scale;
        return mdl;
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
        /*  Line Testing
        glm::vec3 min{ 0.f, 0.f, 0.f };
        glm::vec3 max{ 0.5f, 0.5f, 0.5f };
        AABB aabb;
        aabb.max = max;
        aabb.min = min;
        m_LineRenderer.Submit(aabb, glm::vec4(1.f, 0.f, 0.f, 1.f));

        Line test_line{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.f, 0.f) };
        Line test_line2{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.1f, 0.f) };
        Line test_line3{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.1f) };
        m_LineRenderer.Submit(test_line2, glm::vec4(0.f, 1.f, 0.f, 1.f));
        m_LineRenderer.Submit(test_line3, glm::vec4(0.f, 0.f, 1.f, 1.f));
        
        m_LineRenderer.SubmitBuffer();
        m_LineRenderer.ClearBuffer();
        */

        m_FrameBuffer.Bind();

        m_Renderer.EnableDepthMask(true);
        m_Renderer.EnableDepthTest(true);
        m_Renderer.EnableCulling(false, false, false);
        m_Renderer.EnableBlend(true);

        m_Renderer.ClearColour(0.4f, 0.5f, 0.6f, 0.0f);
        m_Renderer.ClearColorDepth();      
    }
    void RenderSystem::Render()
    {
        
        if (GridActive)
            RenderAAGrid();

        // Drawing Polygons
        for (auto& i : m_Pipeline.m_Sprites)
        {
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(i.m_Transform, "ModelMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
            m_Pipeline.m_Meshes[i.m_Code]->Bind();
            m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.m_Meshes[i.m_Code]);
        }
        
          //Drawing Models
        for (size_t i = 0; i < m_Pipeline.m_ModelTransforms.size(); ++i)
        {
            for (auto& [mesh, material] : m_Pipeline.m_Models[i]->GetMeshes())
            {
                m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->Bind();
                m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(m_Pipeline.m_ModelTransforms[i], "ModelMatrix");
                m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
                m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
                m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
                if (material->DiffuseMap)
                    material->DiffuseMap->Bind(0);
                mesh.Bind();
                glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
            }
        }

        m_FrameBuffer.Unbind();       
    }

    void RenderSystem::EndFrame()
    {
        m_FrameBuffer.Draw();
    }

    void RenderSystem::Clear()
    {
        m_Pipeline.m_Sprites.clear();
        //m_Pipeline.m_Models.clear();
        //m_Pipeline.m_ModelTransforms.clear();
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

    void RenderSystem::RenderGrid(bool state)
    {
        GridActive = state;
    }

    void RenderSystem::RenderAAGrid()
    {   
        m_Pipeline.m_Shaders[ShaderCode::GROUND]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "view");
        m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "proj");
        m_Pipeline.Grid.m_Mesh.Bind();
        m_Renderer.DrawElements(DrawMode::TRIANGLES, m_Pipeline.Grid.m_Mesh);
        m_Renderer.ClearDepth();
    }
}