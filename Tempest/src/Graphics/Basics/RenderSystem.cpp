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
        m_Pipeline.m_Shaders.emplace(ShaderCode::LINE, std::make_unique<Shader>("Shaders/Line_vertex.glsl", "Shaders/Line_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::GROUND, std::make_unique<Shader>("Shaders/GroundPlane_vertex.glsl", "Shaders/GroundPlane_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::LIGHTING, std::make_unique<Shader>("Shaders/Lighting_vertex.glsl", "Shaders/Lighting_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::DIRECTIONAL_SHADOW_MAP, std::make_unique<Shader>("Shaders/DirShadowMap_vertex.glsl", "Shaders/DirShadowMap_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::POINT_LIGHT_DEPTH, std::make_unique<Shader>("Shaders/point_shadows_depth_vertex.glsl", "Shaders/point_shadows_depth_fragment.glsl","Shaders/point_shadows_depth_geom.glsl" ));
        m_Pipeline.m_Shaders.emplace(ShaderCode::MODEL, std::make_unique<Shader>("Shaders/Model_vertex.glsl", "Shaders/Model_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::MODEL_TEXTURE, std::make_unique<Shader>("Shaders/Model2_vertex.glsl", "Shaders/Model2_fragment.glsl"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::MODEL_LIGHT, std::make_unique<Shader>("Shaders/Model3_vertex.glsl", "Shaders/Model3_fragment.glsl"));
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
        if (!m_Pipeline.m_ModelLibrary.count(path))
        {
            m_Pipeline.m_ModelLibrary.insert(std::make_pair(path, std::make_shared<Model>(path.c_str())));
        }
        ModelObj model;
        model.m_Transform = to_Model_Matrix(transform);
        model.m_Model = m_Pipeline.m_ModelLibrary[path];
        m_Pipeline.m_Models.push_back(model);
    }

    void RenderSystem::DrawLine(const Line& line, const glm::vec4& color)
    {
        m_LineRenderer.Submit(line, color);
    }

    void RenderSystem::DrawLine(const AABB& box, const glm::vec4& color)
    {
        m_LineRenderer.Submit(box, color);
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
        /*  Updating line renderer  */
        m_LineRenderer.SubmitBuffer();
        m_LineRenderer.ClearBuffer();
        
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
            dir_lights[0].Bind();

            DrawSprites(MeshCode::CUBE,        ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::SPHERE,      ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::PLANE,       ShaderCode::DIRECTIONAL_SHADOW_MAP);
            DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::DIRECTIONAL_SHADOW_MAP);


            for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
            {
                for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
                {
                    // Send in uniform values
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Bind();
                    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
                    glm::vec3 cam = m_Pipeline.m_Cameras.front().GetPosition();
                    glm::vec3 target = cam + dir_lights[0].Direction;
                    lightView = glm::lookAt( 10.f * dir_lights[0].Direction,
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f)); 
                    lightSpaceMatrix = lightProjection * lightView;
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(6, "shadowMap"); // Set Shadow map for directional light to be slot 6 

                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(1, "meshDrawing"); // 1 for meshdrawing
                    m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                    //glActiveTexture(GL_TEXTURE6);
                    mesh.Bind();
                    glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
                    mesh.Unbind();
                }             
            }       
            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

    
        
        // render all pt lights to depth buffer
        if (GetActivePt_lightsNum())
        {
            for (int numPt = 0; numPt < pt_lights.size(); numPt++)
            {
                if (pt_lights[numPt].hide)
                    continue;
                    
                // Bind Point Light FBO
                pt_lights[numPt].Bind();

                DrawSprites(MeshCode::CUBE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
                DrawSprites(MeshCode::SPHERE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
                DrawSprites(MeshCode::PLANE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
                DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::POINT_LIGHT_DEPTH, numPt);

                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Bind();
                // Send in uniform values
                glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_ShadowBuffer.m_Width / (float)m_ShadowBuffer.m_Height, near_plane, far_plane);
                std::vector<glm::mat4> shadowTransforms;
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                for (unsigned int shadowxform = 0; shadowxform < 6; ++shadowxform)
                    m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetMat4fv(shadowTransforms[shadowxform], ("shadowMatrices[" + std::to_string(shadowxform) + "]").c_str());
                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1f(far_plane, "far_plane");
                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetVec3f(pt_lights[numPt].Position, "lightPos");
                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1i(5, "depthMap"); // Set Shadow map for directional light to be slot 5
                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1i(1, "meshDrawing"); // 1 for meshdrawing

                for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
                {
                    for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
                    {
                        m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                        mesh.Bind();
                        glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
                        mesh.Unbind();
                    }
                }     
            }
            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        // Bind default FBO
        m_FrameBuffer.Bind();
        m_FrameBuffer.SetFrameBufferSize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderAAGrid();

        // Drawing Polygons
         DrawSprites(MeshCode::CUBE,         ShaderCode::LIGHTING);
         DrawSprites(MeshCode::SPHERE,       ShaderCode::LIGHTING);
         DrawSprites(MeshCode::PLANE,        ShaderCode::LIGHTING);
         DrawSprites(MeshCode::ICOSAHEDRON,  ShaderCode::LIGHTING);

         glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 

        //glClear(GL_COLOR_BUFFER_BIT);
        //DrawSprites(MeshCode::CUBE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::SPHERE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::PLANE, ShaderCode::BASIC);
        //DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::BASIC);

         m_FrameBuffer.Bind();
         glActiveTexture(GL_TEXTURE0);

        // Drawing Models
        for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
        {
            for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
            {
                if (material->DiffuseMap)
                {
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Bind();
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
                    material->DiffuseMap->Bind(0);
                }
                
                else if (material->BaseTexture)
                {
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Bind();
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
                    material->BaseTexture->Bind(0);
                    //const GLfloat near_plane = 1.0f, far_plane = 25.0f;
                    
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(dir_lights[0].Color, "LightColor");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(dir_lights[0].Direction, "LightDirection");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(dir_lights[0].Intensity, "LightIntensity");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(far_plane, "far_plane");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i((int)GetActivePt_lightsNum(), "PointLightNumber");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
                    
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
                    for (unsigned int ptLight = 0; ptLight < (unsigned int)GetActivePt_lightsNum(); ++ptLight)
                    {
                        std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
                        std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
                        std::string pointLightColors = "pointLightColors[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(pt_lights[ptLight].Color, pointLightColors.data());
                        std::string pointLightConsts = "pointLightConsts[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightConsts, pointLightConsts.data());
                        std::string pointLightLinears = "pointLightLinears[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightLinears, pointLightLinears.data());
                        std::string pointLightQuads = "pointLightQuads[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightQuads, pointLightQuads.data());
                    }
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(0, "diffuseTexture");   // Set Point light depth to be slot 
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on
                    
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(ambientStrength, "ambientStrength");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(shininess, "shininess");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(specularStrength, "specularStrength");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(dir_lights[0].hide ? 0 : 1, "DirectionalLightOn");
                }
                
                //else
                //{
                //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->Bind();
                //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
                //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
                //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
                //}

                //mesh.Bind();
                //glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
                else
                {
                    // Dir + Point Light
                   // const GLfloat near_plane = 1.0f, far_plane = 25.0f;
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Bind();
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(dir_lights[0].Color, "LightColor");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(dir_lights[0].Direction, "LightDirection");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(dir_lights[0].Intensity, "LightIntensity");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(far_plane, "far_plane");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i((int)GetActivePt_lightsNum(), "PointLightNumber");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");

                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
                    for (unsigned int ptLight = 0; ptLight < (unsigned int)GetActivePt_lightsNum(); ++ptLight)
                    {
                        std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
                        std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
                        std::string pointLightColors = "pointLightColors[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(pt_lights[ptLight].Color, pointLightColors.data());
                        std::string pointLightConsts = "pointLightConsts[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightConsts, pointLightConsts.data());
                        std::string pointLightLinears = "pointLightLinears[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightLinears, pointLightLinears.data());
                        std::string pointLightQuads = "pointLightQuads[" + std::to_string(ptLight) + "]";
                        m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightQuads, pointLightQuads.data());
                    }
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(0, "diffuseTexture");   // Set Point light depth to be slot 
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on

                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(ambientStrength, "ambientStrength");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(shininess, "shininess");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(specularStrength, "specularStrength");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(dir_lights[0].hide ? 0 : 1, "DirectionalLightOn");
                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
                }
                mesh.Bind();
                glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
                if (material->BaseTexture)
                {
                    material->BaseTexture->Unbind(0);
                }
            }
        }
        m_LineRenderer.Render(m_Pipeline.m_Cameras[0].GetViewProjectionMatrix(), m_Pipeline.m_Shaders[ShaderCode::LINE]);
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

        switch (shaderType)
        {
        case (ShaderCode::LIGHTING):

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
            shader->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
            shader->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
            shader->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on

            shader->Set1f(ambientStrength,            "ambientStrength");
            shader->Set1f(shininess,                  "shininess");
            shader->Set1f(specularStrength,           "specularStrength");
            shader->Set1i(dir_lights[0].hide ? 0 : 1 ,"DirectionalLightOn");

            break;
        case (ShaderCode::DIRECTIONAL_SHADOW_MAP):
            {
                // Send in uniform values
                lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
                glm::vec3 cam = m_Pipeline.m_Cameras.front().GetPosition();
                glm::vec3 target = cam + dir_lights[0].Direction;
                lightView = glm::lookAt( 10.f * dir_lights[0].Direction,
                                         glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(0.0f, 1.0f, 0.0f)); 
                lightSpaceMatrix = lightProjection * lightView;
                shader->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
                shader->Set1i(6, "shadowMap"); // Set Shadow map for directional light to be slot 1 (note: point light shadow at slot 0 )
                shader->Set1i(0, "meshDrawing"); // 1 for meshdrawing
            }
            break;
        case (ShaderCode::POINT_LIGHT_DEPTH):
        {
            if(pt_light_num != -1)
            {
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
                shader->Set1i(5, "depthMap"); // Set Shadow map for directional light to be slot 5 (note: point light shadow at slot 0 )
                shader->Set1i(0, "meshDrawing"); // 1 for meshdrawing
            }

        }
            break;
        default:
            //m_FrameBuffer.Bind();
            //m_FrameBuffer.SetFrameBufferSize();
            return;
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
        m_Pipeline.m_Models.clear();
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