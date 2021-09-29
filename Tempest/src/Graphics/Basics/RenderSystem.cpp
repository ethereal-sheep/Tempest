#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"
#include "Logger/Log.h"


namespace Tempest
{
    void RenderSystem::InitMeshes()
    {
        m_Pipeline.m_Meshes.emplace(std::make_pair(MeshCode::CUBE, std::make_unique<Mesh>(GeometryFactory::GenerateIndexedCube(1, 1))));
        m_Pipeline.m_Meshes.emplace(std::make_pair(MeshCode::PLANE, std::make_unique<Mesh>(GeometryFactory::GenerateIndexedPlane(1, 1))));
        m_Pipeline.m_Meshes.emplace(std::make_pair(MeshCode::SPHERE, std::make_unique<Mesh>(GeometryFactory::GenerateIndexedSphere(1, 16, 16))));
        m_Pipeline.m_Meshes.emplace(std::make_pair(MeshCode::ICOSAHEDRON, std::make_unique<Mesh>(GeometryFactory::GenerateIndexedIcosahedron())));
    }

    void RenderSystem::InitShaders()
    {
        m_Pipeline.m_Shaders.emplace(ShaderCode::BASIC, std::make_unique<Shader>("Shaders/Basic_vertex.glsl", "Shaders/Basic_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::TEXTURE, std::make_unique<Shader>("Shaders/Texture_vertex.glsl", "Shaders/Texture_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::GROUND, std::make_unique<Shader>("Shaders/GroundPlane_vertex.glsl", "Shaders/GroundPlane_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::DIRECTIONAL_LIGHT, std::make_unique<Shader>("Shaders/DirectionalLight_vertex.glsl", "Shaders/DirectionalLight_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::SHADOW_MAP, std::make_unique<Shader>("Shaders/ShadowDepth_vertex.glsl", "Shaders/ShadowDepth_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::DEBUG, std::make_unique<Shader>("Shaders/DEBUG_vertex.glsl", "Shaders/DEBUG_fragment.glsl"));
    }

    void RenderSystem::InitBuffers()
    {
        const BufferLayout Instanced_Layout{
            Layout_Format::AOS,
            {
                VertexType::Mat4,
                VertexType::Mat3,
            } };

        m_Pipeline.s_Mesh.m_Instanced.SetData(nullptr, 0, BufferType::ARRAY_BUFFER);
        m_Pipeline.m_Meshes[MeshCode::CUBE]->GetVertexArray()->AttachVertexBufferInstanced(m_Pipeline.s_Mesh.m_Instanced, Instanced_Layout);
        m_Pipeline.m_Meshes[MeshCode::SPHERE]->GetVertexArray()->AttachVertexBufferInstanced(m_Pipeline.s_Mesh.m_Instanced, Instanced_Layout);
        m_Pipeline.m_Meshes[MeshCode::PLANE]->GetVertexArray()->AttachVertexBufferInstanced(m_Pipeline.s_Mesh.m_Instanced, Instanced_Layout);
        m_Pipeline.m_Meshes[MeshCode::ICOSAHEDRON]->GetVertexArray()->AttachVertexBufferInstanced(m_Pipeline.s_Mesh.m_Instanced, Instanced_Layout);

        m_Pipeline.m_Indirect.SetData(nullptr, 20, BufferType::INDIRECT_BUFFER);
    }

    RenderSystem::RenderSystem(uint32_t width, uint32_t height)
    {
        m_Renderer.SetViewport(0, 0, width, height);

        InitMeshes();
        InitShaders();
        InitBuffers();
        
        if(m_Pipeline.m_Cameras.empty())
            m_Pipeline.m_Cameras.emplace_back(Camera{});

        dir_lights.emplace_back(Directional_Light{});
        pt_lights.emplace_back(Point_Light{});
        pt_lights[0].Position = glm::vec3(0.5f, 0.5f, 0.5f);

        pt_lights.emplace_back(Point_Light{});
        pt_lights[1].Position = glm::vec3(-1.f, 0.5f, 0.5f);


        /// TEST DEPTH MAP
        //unsigned int depthMapFBO;
        glGenFramebuffers(1, &m_ShadowBuffer.m_depthMapFBO);
        // create depth texture
        //unsigned int depthMap;
        glGenTextures(1, &m_ShadowBuffer.depthMap);
        glBindTexture(GL_TEXTURE_2D, m_ShadowBuffer.depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowBuffer.m_depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowBuffer.depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        m_Pipeline.m_Shaders[ShaderCode::DEBUG]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::DEBUG]->Set1i(0, "depthMap");
        // END DEPTH MAP
    }

    void RenderSystem::Submit(MeshCode code, const Transform& transform)
    {
        SpriteObj sprite;
        sprite.m_Transform = to_Model_Matrix(transform);
        sprite.m_Normal = glm::inverse(glm::transpose(glm::mat3(sprite.m_Transform)));
        switch (code)
        {
            case MeshCode::CUBE:            m_Pipeline.m_Cubes.emplace_back(sprite);            break;
            case MeshCode::SPHERE:          m_Pipeline.m_Spheres.emplace_back(sprite);          break;
            case MeshCode::PLANE:           m_Pipeline.m_Planes.emplace_back(sprite);           break;
            case MeshCode::ICOSAHEDRON:     m_Pipeline.m_Icosahedrons.emplace_back(sprite);     break;
        }
    }

    void RenderSystem::SubmitModel(const string& path, const Transform& transform)
    {
        m_Pipeline.m_Models.emplace_back(std::make_shared<Model>(path.c_str()));
        m_Pipeline.m_ModelTransforms.emplace_back(to_Model_Matrix(transform));
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

        ///// TEST

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        glViewport(0, 0, m_ShadowBuffer.m_Width, m_ShadowBuffer.m_Height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowBuffer.m_depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, woodTexture);
        //renderScene(simpleDepthShader);

        DrawSprites(MeshCode::CUBE, m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]);
        DrawSprites(MeshCode::SPHERE, m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]);
        DrawSprites(MeshCode::PLANE, m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]);
        DrawSprites(MeshCode::ICOSAHEDRON, m_Pipeline.m_Shaders[ShaderCode::SHADOW_MAP]);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, 1600, 900);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        m_Pipeline.m_Shaders[ShaderCode::DEBUG]->Bind();


        m_Pipeline.m_Shaders[ShaderCode::DEBUG]->Set1f(near_plane, "near_plane");
        m_Pipeline.m_Shaders[ShaderCode::DEBUG]->Set1f(far_plane, "far_plane");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ShadowBuffer.depthMap);

        DrawSprites(MeshCode::CUBE, m_Pipeline.m_Shaders[ShaderCode::DEBUG]);
        DrawSprites(MeshCode::SPHERE, m_Pipeline.m_Shaders[ShaderCode::DEBUG]);
        DrawSprites(MeshCode::PLANE, m_Pipeline.m_Shaders[ShaderCode::DEBUG]);
        DrawSprites(MeshCode::ICOSAHEDRON, m_Pipeline.m_Shaders[ShaderCode::DEBUG]);


        m_FrameBuffer.Bind();
        /// END TEST

        if (GridActive)
            RenderAAGrid();

        // Drawing Polygons
        DrawSprites(MeshCode::CUBE,         m_Pipeline.m_Shaders[ShaderCode::BASIC]);
        DrawSprites(MeshCode::SPHERE,       m_Pipeline.m_Shaders[ShaderCode::BASIC]);
        DrawSprites(MeshCode::PLANE,        m_Pipeline.m_Shaders[ShaderCode::BASIC]);
        DrawSprites(MeshCode::ICOSAHEDRON,  m_Pipeline.m_Shaders[ShaderCode::BASIC]);
        
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

                // Dir + Point Light
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Bind();
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Color, "LightColor");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Direction, "LightDirection");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1f(dir_lights[0].Intensity, "LightIntensity");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");

                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1i((int)pt_lights.size(), "PointLightNumber");

                for (unsigned int ptLight = 0; ptLight < pt_lights.size(); ++ptLight)
                {
                    std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
                    std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
                }
                glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
            }
        }

        m_FrameBuffer.Unbind();       
    }

    void RenderSystem::DrawSprites(MeshCode code, const tuptr<Shader>& shader)
    {
        switch (code)
        {
            case MeshCode::CUBE:            DrawSprites(shader, m_Pipeline.m_Cubes, code);            break;
            case MeshCode::SPHERE:          DrawSprites(shader, m_Pipeline.m_Spheres, code);          break;
            case MeshCode::PLANE:           DrawSprites(shader, m_Pipeline.m_Planes, code);           break;
            case MeshCode::ICOSAHEDRON:     DrawSprites(shader, m_Pipeline.m_Icosahedrons, code);     break;
        }
    }

    void RenderSystem::DrawSprites(const tuptr<Shader>& shader, const tvector<SpriteObj>& sprites, MeshCode code)
    {
        if (sprites.empty()) return;

        shader->Bind();
        shader->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
        shader->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");

        auto& spriteMesh = m_Pipeline.s_Mesh;
        spriteMesh.m_Instanced.SetSubDataResize((void*)sprites.data(), static_cast<int32_t>(sprites.size() * sizeof(SpriteObj)));
        spriteMesh.m_Indirect.vertexCount = m_Pipeline.m_Meshes[code]->GetVertexCount();
        spriteMesh.m_Indirect.instanceCount = static_cast<uint32_t>(sprites.size());
        m_Pipeline.m_Indirect.SetSubData(static_cast<void*>(&spriteMesh.m_Indirect), sizeof(DrawElementsIndirect));

        m_Pipeline.m_Meshes[code]->Bind();
        m_Pipeline.m_Indirect.Bind();

        // Dir + Point Light
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Color, "LightColor");
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(dir_lights[0].Direction, "LightDirection");
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1f(dir_lights[0].Intensity, "LightIntensity");
        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");

        m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1i((int)pt_lights.size(), "PointLightNumber");

        for (unsigned int ptLight = 0; ptLight < pt_lights.size(); ++ptLight)
        {
            std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
            std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
            m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_LIGHT]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
        }

        // Render
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_Pipeline.m_Indirect.GetSize() / sizeof(DrawElementsIndirect), 0);
        m_Pipeline.m_Indirect.Unbind();
        m_Pipeline.m_Meshes[code]->Unbind();
    }

    void RenderSystem::EndFrame()
    {
        m_FrameBuffer.Draw();
    }

    void RenderSystem::Clear()
    {
        m_Pipeline.m_Cubes.clear();
        m_Pipeline.m_Planes.clear();
        m_Pipeline.m_Spheres.clear();
        m_Pipeline.m_Icosahedrons.clear();
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

    glm::mat4 RenderSystem::to_Model_Matrix(const Transform& transform)
    {
        glm::mat4 mdl(1.f);

        glm::mat4 translate = glm::translate(glm::vec3(transform.position.x, transform.position.y, transform.position.z));
        glm::mat4 rotate(transform.rotation);
        glm::mat4 scale = glm::scale(glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

        mdl = translate * rotate * scale;
        return mdl;
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