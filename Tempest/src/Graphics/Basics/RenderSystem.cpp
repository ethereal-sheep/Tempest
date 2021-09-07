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

        m_Pipeline.m_Cameras.emplace_back(Camera{});
        dir_lights.emplace_back(Directional_Light{});

        Transform transform;
        transform.position = vec3(-1.f, 0.f, 0.f);
        transform.rotation = quat(1.f, 0.f, 0.f, 0.f);
        transform.scale = vec3(0.15f, 0.15f, 0.15f);
        Submit(MeshCode::PLANE, transform);

        Transform transform2;
        transform2.position = vec3(0.f, 0.f, 0.f);
        transform2.rotation = quat(0.f, 0.f, 0.f, 0.f);
        transform2.scale = vec3(0.15f, 0.15f, 0.15f);
        Submit(MeshCode::SPHERE, transform2);
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

        //m_FrameBuffer.Bind();

        m_Renderer.EnableDepthMask(true);
        m_Renderer.EnableDepthTest(true);
        m_Renderer.EnableCulling(false, false, false);
        m_Renderer.EnableBlend(true);
        //m_Renderer.EnableCulling(true, true, true);
        //m_Renderer.SetPolygonMode(PolyMode::FILL);
        //for (auto i : dir_lights)
        //{
        //    m_ShadowBuffer.Bind();
        //    m_ShadowBuffer.ClearAttachments();
        //    m_ShadowBuffer.AttachDepthAttachment(i.DepthMap);
        //    //m_ShadowBuffer.Validate();
        //    m_ShadowBuffer.ResizeViewport(i.DepthMap);
        //    m_ShadowBuffer.Draw(i.Direction, m_Pipeline.m_Sprites, m_Pipeline.m_Transforms, m_Pipeline.m_Meshes);
        //    m_ShadowBuffer.Unbind();
        //}

        m_Renderer.ClearColour(0.4f, 0.5f, 0.6f, 0.0f);
        m_Renderer.ClearColorDepth();      
    }
    void RenderSystem::Render()
    {
        //{
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->Bind();
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetVec3f(m_Pipeline.m_Cameras[0].GetFront(), "front");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetVec3f(m_Pipeline.m_Cameras[0].GetPosition(), "camera");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetVec2f(m_Pipeline.m_Cameras[0].GetResolution(), "resolution");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "view");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "proj");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Transforms[0], "model");
        //    m_Pipeline.m_Shaders[ShaderCode::GROUND]->SetMat4fv(m_Pipeline.m_Cameras[0].GetInverseViewProjectionMatrix(), "ivpm");
        //    m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[0])->Bind();
        //    glDrawElements(GL_TRIANGLES, m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[0])->GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //    glClear(GL_DEPTH_BUFFER_BIT);
        //}

        

        {
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Transforms[0], "ModelMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
            m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        
            m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[1])->Bind();
            glDrawElements(GL_TRIANGLES, m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[1])->GetVertexCount(), GL_UNSIGNED_INT, NULL);
        }
        
        {
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Transforms[1], "ModelMatrix");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Color, "LightColor");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Direction, "LightDirection");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1f(dir_lights[0].Intensity, "LightIntensity");
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(m_Pipeline.m_Cameras[0].GetPosition(), "CamPosition");
            
            m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[1])->Bind();
            glDrawElements(GL_TRIANGLES, m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[1])->GetVertexCount(), GL_UNSIGNED_INT, NULL);
        }
       
        //Texture tex("Assets/test_photo.png");
        
        //for (size_t i = 0; i < m_Pipeline.m_Sprites.size(); ++i)
        //{
        //    m_Pipeline.m_Shaders[ShaderCode::BASIC]->Bind();
        //    m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Transforms[0], "ModelMatrix");
        //    m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
        //    m_Pipeline.m_Shaders[ShaderCode::BASIC]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        //    
        //    m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[i])->Bind();
        //    glDrawElements(GL_TRIANGLES, m_Pipeline.m_Meshes.at(m_Pipeline.m_Sprites[i])->GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //}
        //
        //  //Model Drawing
        //for (auto& [mesh, material] : model.GetMeshes())
        //{
        //    m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->Bind();
        //    m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(Model(transform), "ModelMatrix");
        //    m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
        //    m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        //    m_Pipeline.m_Shaders[ShaderCode::TEXTURE]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
        //    if(material->DiffuseMap)
        //        material->DiffuseMap->Bind(0);
        //    mesh.Bind();
        //    glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //}
        

        //m_LineRenderer.Render(m_Pipeline.m_Cameras[0].GetViewProjectionMatrix());

        //m_FrameBuffer.Unbind();       
    }

    void RenderSystem::EndFrame()
    {
        //m_FrameBuffer.Draw();
    }

    void RenderSystem::Clear()
    {
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