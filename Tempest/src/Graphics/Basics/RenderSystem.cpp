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
        m_Pipeline.m_Shaders.emplace(ShaderCode::LIGHTING, std::make_unique<Shader>("Shaders/Lighting_vertex.glsl", "Shaders/Lighting_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::DIRECTIONAL_SHADOW_MAP, std::make_unique<Shader>("Shaders/DirShadowMap_vertex.glsl", "Shaders/DirShadowMap_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::POINT_LIGHT_DEPTH, std::make_unique<Shader>("Shaders/point_shadows_depth_vertex.glsl", "Shaders/point_shadows_depth_fragment.glsl","Shaders/point_shadows_depth_geom.glsl" ));
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

        // Make 1 Directional Light
        dir_lights.emplace_back(Directional_Light{});
        
        // Make all FBO for all point Lights
        for(int makePointLight = 0 ; makePointLight < (int)MAX_POINT_LIGHT ; makePointLight++)
        {
            pt_lights.emplace_back(Point_Light{});
            pt_lights[makePointLight].Position = glm::vec3( (float)makePointLight * 0.5f, 0.5f, 0.5f);
        }

     /*  if(pt_lights.size())
            pt_lights[0].hide = false;*/
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Directional light depth map
        if(!dir_lights[0].hide)
        {
            DrawSprites(MeshCode::CUBE,        ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::SPHERE,      ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::PLANE,       ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::DIRECTIONAL_SHADOW_MAP);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render all pt lights to depth buffer
        for(int numPt = 0 ; numPt < GetActivePt_lightsNum(); numPt++)
        {
            DrawSprites(MeshCode::CUBE,        ShaderCode::POINT_LIGHT_DEPTH, numPt);
            DrawSprites(MeshCode::SPHERE,      ShaderCode::POINT_LIGHT_DEPTH, numPt);
            DrawSprites(MeshCode::PLANE,       ShaderCode::POINT_LIGHT_DEPTH, numPt);
            DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::POINT_LIGHT_DEPTH, numPt);
        }
        
        m_FrameBuffer.SetFrameBufferSize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_FrameBuffer.Bind();

        if (GridActive)
            RenderAAGrid();

        // Drawing Polygons
        DrawSprites(MeshCode::CUBE,         ShaderCode::LIGHTING);
        DrawSprites(MeshCode::SPHERE,       ShaderCode::LIGHTING);
        DrawSprites(MeshCode::PLANE,        ShaderCode::LIGHTING);
        DrawSprites(MeshCode::ICOSAHEDRON,  ShaderCode::LIGHTING);
       

        //DrawSprites(MeshCode::CUBE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::SPHERE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::PLANE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::BASIC);


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
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->Bind();
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetVec3f(dir_lights[0].Color, "LightColor");
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetVec3f(dir_lights[0].Direction, "LightDirection");
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->Set1f(dir_lights[0].Intensity, "LightIntensity");
                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");

                m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->Set1i((int)pt_lights.size(), "PointLightNumber");

                for (unsigned int ptLight = 0; ptLight < pt_lights.size(); ++ptLight)
                {
                    std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
                    m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
                    std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
                    m_Pipeline.m_Shaders[ShaderCode::LIGHTING]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
                }
                glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
            }
        }

        m_FrameBuffer.Unbind();       
    }

    void RenderSystem::DrawSprites(MeshCode code, ShaderCode shaderType, int pt_light_num)
    {
        switch (code)
        {
            case MeshCode::CUBE:            DrawSprites(m_Pipeline.m_Shaders[shaderType], m_Pipeline.m_Cubes, code, shaderType, pt_light_num);            break;
            case MeshCode::SPHERE:          DrawSprites(m_Pipeline.m_Shaders[shaderType], m_Pipeline.m_Spheres, code, shaderType, pt_light_num);          break;
            case MeshCode::PLANE:           DrawSprites(m_Pipeline.m_Shaders[shaderType], m_Pipeline.m_Planes, code, shaderType, pt_light_num);           break;
            case MeshCode::ICOSAHEDRON:     DrawSprites(m_Pipeline.m_Shaders[shaderType], m_Pipeline.m_Icosahedrons, code, shaderType, pt_light_num);     break;
        }
    }

    void RenderSystem::DrawSprites(const tuptr<Shader>& shader, const tvector<SpriteObj>& sprites, MeshCode code, ShaderCode shaderType , int pt_light_num)
    {
        if (sprites.empty()) return;

        shader->Bind();
        shader->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
        shader->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");

        const GLfloat near_plane = 1.0f, far_plane = 25.0f;
        switch (shaderType)
        {
        case (ShaderCode::LIGHTING):
            // Bind default FBO
            m_FrameBuffer.Bind();

            // Send in uniform values
            shader->SetVec3f(dir_lights[0].Color,       "LightColor");
            shader->SetVec3f(dir_lights[0].Direction,   "LightDirection");
            shader->Set1f(dir_lights[0].Intensity,      "LightIntensity");
            shader->Set1f(far_plane,                    "far_plane");
            shader->Set1i((int)GetActivePt_lightsNum(), "PointLightNumber");
            shader->SetMat4fv(lightSpaceMatrix,         "lightSpaceMatrix");
            
            shader->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
            for (unsigned int ptLight = 0; ptLight < (unsigned int)GetActivePt_lightsNum(); ++ptLight)
            {
                std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
                shader->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
                std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
                shader->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
                std::string pointLightColors    = "pointLightColors[" + std::to_string(ptLight) + "]";
                shader->SetVec3f(pt_lights[ptLight].Color, pointLightColors.data());
                std::string pointLightConsts    = "pointLightConsts[" + std::to_string(ptLight) + "]";
                shader->Set1f(pt_lights[ptLight].pointLightConsts, pointLightConsts.data());
                std::string pointLightLinears   = "pointLightLinears[" + std::to_string(ptLight) + "]";
                shader->Set1f(pt_lights[ptLight].pointLightLinears, pointLightLinears.data());
                std::string pointLightQuads     = "pointLightQuads[" + std::to_string(ptLight) + "]";
                shader->Set1f(pt_lights[ptLight].pointLightQuads, pointLightQuads.data());
            }
            shader->Set1i(0, "depthMap");   // Set Point light depth to be slot 0
            shader->Set1i(1, "shadowMap");  // Set Dir Light depthh to be slot 1
            shader->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on

            shader->Set1f(ambientStrength, "ambientStrength");
            shader->Set1f(shininess,       "shininess");
            shader->Set1f(specularStrength,       "specularStrength");
            shader->Set1i(dir_lights[0].hide ? 0 : 1 ,"DirectionalLightOn");

            break;
        case (ShaderCode::DIRECTIONAL_SHADOW_MAP):
            {
                // Bind Directional Light FBO
                dir_lights[0].Bind(); 

                // Send in uniform values
                lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
                glm::vec3 cam = m_Pipeline.m_Cameras.front().GetPosition();
                glm::vec3 target = cam + dir_lights[0].Direction;
                lightView = glm::lookAt( 10.f * dir_lights[0].Direction,
                                         glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(0.0f, 1.0f, 0.0f)); 
                lightSpaceMatrix = lightProjection * lightView;
                shader->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
                shader->Set1i(1, "shadowMap"); // Set Shadow map for directional light to be slot 1 (note: point light shadow at slot 0 )
            }
            break;
        case (ShaderCode::POINT_LIGHT_DEPTH):
        {
            if(pt_light_num != -1)
            {
                // Bind Point Light FBO
                pt_lights[pt_light_num].Bind(); 

                // Send in uniform values
                glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_ShadowBuffer.m_Width / (float)m_ShadowBuffer.m_Height, near_plane, far_plane);
                std::vector<glm::mat4> shadowTransforms;
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[pt_light_num].Position, pt_lights[pt_light_num].Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                
                for (unsigned int i = 0; i < 6; ++i)
                    shader->SetMat4fv(shadowTransforms[i], ("shadowMatrices[" + std::to_string(i) + "]").c_str());
                shader->Set1f(far_plane, "far_plane");
                shader->SetVec3f(pt_lights[pt_light_num].Position, "lightPos");
            }

        }
            break;
        default:
            m_FrameBuffer.Bind();
            m_FrameBuffer.SetFrameBufferSize();
            break;
        
        }

        auto& spriteMesh = m_Pipeline.s_Mesh;
        spriteMesh.m_Instanced.SetSubDataResize((void*)sprites.data(), static_cast<int32_t>(sprites.size() * sizeof(SpriteObj)));
        spriteMesh.m_Indirect.vertexCount = m_Pipeline.m_Meshes[code]->GetVertexCount();
        spriteMesh.m_Indirect.instanceCount = static_cast<uint32_t>(sprites.size());
        m_Pipeline.m_Indirect.SetSubData(static_cast<void*>(&spriteMesh.m_Indirect), sizeof(DrawElementsIndirect));

        m_Pipeline.m_Meshes[code]->Bind();
        m_Pipeline.m_Indirect.Bind();
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_Pipeline.m_Indirect.GetSize() / sizeof(DrawElementsIndirect), 0);
        m_Pipeline.m_Indirect.Unbind();
        m_Pipeline.m_Meshes[code]->Unbind();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
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


    int RenderSystem::GetActivePt_lightsNum()
    {
        int counter = 0;
        for(auto& point : pt_lights)
            if(point.hide == false)
                counter++;
        return counter;
    }

    int RenderSystem::GetGammaCorrection()
    {
        return GammaCorrection;
    }

    void RenderSystem::SetGammaCorrection(int gamma)
    {
        GammaCorrection = gamma;
    }
}