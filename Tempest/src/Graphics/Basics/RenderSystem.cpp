/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"
#include "Logger/Log.h"

namespace Tempest
{
    glm::mat4 envMapView[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

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

        // TEST

        m_Pipeline.m_Shaders.emplace(ShaderCode::gBufferShader, std::make_unique<Shader>("Shaders/gBuffer.vert", "Shaders/gBuffer.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::latlongToCubeShader, std::make_unique<Shader>("Shaders/latlongToCube.vert", "Shaders/latlongToCube.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::simpleShader, std::make_unique<Shader>("Shaders/simple.vert", "Shaders/simple.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::lightingBRDFShader, std::make_unique<Shader>("Shaders/lightingBRDF.vert", "Shaders/lightingBRDF.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::irradianceIBLShader, std::make_unique<Shader>("Shaders/irradianceIBL.vert", "Shaders/irradianceIBL.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::prefilterIBLShader, std::make_unique<Shader>("Shaders/prefilterIBL.vert", "Shaders/prefilterIBL.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::integrateIBLShader, std::make_unique<Shader>("Shaders/integrateIBL.vert", "Shaders/integrateIBL.frag"));

        m_Pipeline.m_Shaders.emplace(ShaderCode::firstpassPPShader, std::make_unique<Shader>("Shaders/postprocess.vert", "Shaders/firstpass.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::saoShader, std::make_unique<Shader>("Shaders/sao.vert", "Shaders/sao.frag"));
        m_Pipeline.m_Shaders.emplace(ShaderCode::saoBlurShader, std::make_unique<Shader>("Shaders/sao.vert", "Shaders/saoBlur.frag"));

        


    }

    void RenderSystem::gBufferSetup()
    {
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        int WIDTH = getWidth(), HEIGHT = getHeight();
        // Position
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        // Albedo + Roughness
        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedo, 0);

        // Normals + Metalness
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

        // Effects (AO + Velocity)
        glGenTextures(1, &gEffects);
        glBindTexture(GL_TEXTURE_2D, gEffects);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gEffects, 0);

        // Define the COLOR_ATTACHMENTS for the G-Buffer
        GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(4, attachments);

        // Z-Buffer
        glGenRenderbuffers(1, &zBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, zBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, zBuffer);

        // Check if the framebuffer is complete before continuing
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete !");
    }

    void RenderSystem::saoSetup()
    {
        int WIDTH = getWidth(), HEIGHT = getHeight();
        // SAO Buffer
        glGenFramebuffers(1, &saoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, saoFBO);
        glGenTextures(1, &saoBuffer);
        glBindTexture(GL_TEXTURE_2D, saoBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, saoBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SAO Framebuffer not complete !" << std::endl;

        // SAO Blur Buffer
        glGenFramebuffers(1, &saoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, saoBlurFBO);
        glGenTextures(1, &saoBlurBuffer);
        glBindTexture(GL_TEXTURE_2D, saoBlurBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, saoBlurBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SAO Blur Framebuffer not complete !" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void RenderSystem::postprocessSetup()
    {
        int WIDTH = getWidth(), HEIGHT = getHeight();
        // Post-processing Buffer
        glGenFramebuffers(1, &postprocessFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, postprocessFBO);

        glGenTextures(1, &postprocessBuffer);
        glBindTexture(GL_TEXTURE_2D, postprocessBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postprocessBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Postprocess Framebuffer not complete !" << std::endl;
    }



    void RenderSystem::iblSetup()
    {
        int WIDTH = getWidth(), HEIGHT = getHeight();
        // Latlong to Cubemap conversion
        glGenFramebuffers(1, &envToCubeFBO);
        glGenRenderbuffers(1, &envToCubeRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, envToCubeFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, envToCubeRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, envMapCube.getTexWidth(), envMapCube.getTexHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, envToCubeRBO);

        
        m_Pipeline.m_Shaders[ShaderCode::latlongToCubeShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::latlongToCubeShader]->SetMat4fv(envMapProjection, "projection");
        glActiveTexture(GL_TEXTURE0);
        envMapHDR.useTexture();

        glViewport(0, 0, envMapCube.getTexWidth(), envMapCube.getTexHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, envToCubeFBO);

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_Pipeline.m_Shaders[ShaderCode::latlongToCubeShader]->SetMat4fv(envMapView[i], "view");
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMapCube.getTexID(), 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            envCubeRender.drawShape();
        }

        envMapCube.computeTexMipmap();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Diffuse irradiance capture
        glGenFramebuffers(1, &irradianceFBO);
        glGenRenderbuffers(1, &irradianceRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, irradianceFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, irradianceRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, envMapIrradiance.getTexWidth(), envMapIrradiance.getTexHeight());

        //irradianceIBLShader.useShader();

        m_Pipeline.m_Shaders[ShaderCode::irradianceIBLShader]->Bind();
        //glUniformMatrix4fv(glGetUniformLocation(irradianceIBLShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(envMapProjection));
        m_Pipeline.m_Shaders[ShaderCode::irradianceIBLShader]->SetMat4fv(envMapProjection, "projection");

        glActiveTexture(GL_TEXTURE0);
        envMapCube.useTexture();

        glViewport(0, 0, envMapIrradiance.getTexWidth(), envMapIrradiance.getTexHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, irradianceFBO);

        for (unsigned int i = 0; i < 6; ++i)
        {
            //glUniformMatrix4fv(glGetUniformLocation(irradianceIBLShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(envMapView[i]));
            m_Pipeline.m_Shaders[ShaderCode::irradianceIBLShader]->SetMat4fv(envMapView[i], "view");
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMapIrradiance.getTexID(), 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            envCubeRender.drawShape();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Prefilter cubemap
        //prefilterIBLShader.useShader();
        m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Bind();
        //glUniformMatrix4fv(glGetUniformLocation(prefilterIBLShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(envMapProjection));
        m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->SetMat4fv(envMapProjection, "projection");
        envMapCube.useTexture();

        glGenFramebuffers(1, &prefilterFBO);
        glGenRenderbuffers(1, &prefilterRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, prefilterFBO);

        unsigned int maxMipLevels = 5;

        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            unsigned int mipWidth = (unsigned int)(envMapPrefilter.getTexWidth() * std::pow(0.5, mip));
            unsigned int mipHeight = (unsigned int)(envMapPrefilter.getTexHeight() * std::pow(0.5, mip));

            glBindRenderbuffer(GL_RENDERBUFFER, prefilterRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);

            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            

            m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Set1f(roughness, "roughness");
            m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Set1f((float)envMapPrefilter.getTexWidth(), "cubeResolutionWidth");
            m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Set1f((float)envMapPrefilter.getTexHeight(), "cubeResolutionHeight");


            for (unsigned int i = 0; i < 6; ++i)
            {
                //glUniformMatrix4fv(glGetUniformLocation(prefilterIBLShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(envMapView[i]));
                m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->SetMat4fv(envMapView[i], "view");
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMapPrefilter.getTexID(), mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                envCubeRender.drawShape();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // BRDF LUT
        glGenFramebuffers(1, &brdfLUTFBO);
        glGenRenderbuffers(1, &brdfLUTRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, brdfLUTFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, brdfLUTRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, envMapLUT.getTexWidth(), envMapLUT.getTexHeight());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, envMapLUT.getTexID(), 0);

        glViewport(0, 0, envMapLUT.getTexWidth(), envMapLUT.getTexHeight());
        //integrateIBLShader.useShader();
        m_Pipeline.m_Shaders[ShaderCode::integrateIBLShader]->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quadRender.drawShape();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, WIDTH, HEIGHT);
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
        pt_lights[0].hide = false;

        objectAlbedo.setTexture("textures/pbr/gold/gold_albedo.png", "goldAlbedo", true);
        //objectAlbedo.setTextureDDS( "Assets/dds_test.dds" , "goldAlbedo", true);
        objectNormal.setTexture("textures/pbr/gold/gold_normal.png", "goldNormal", true);
        objectRoughness.setTexture("textures/pbr/gold/gold_roughness.png", "goldRoughness", true);
        objectMetalness.setTexture("textures/pbr/gold/gold_metalness.png", "goldMetalness", true);
        objectAO.setTexture("textures/pbr/gold/gold_ao.png", "goldAO", true);

        envMapHDR.setTextureHDR("textures/hdr/appart.hdr", "appartHDR", true);

        //envMapHDR.setTexture( "Assets/dds_test.dds" , "appartHDR", true);

        envMapCube.setTextureCube(512, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
        envMapIrradiance.setTextureCube(32, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR);
        envMapPrefilter.setTextureCube(128, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
        envMapPrefilter.computeTexMipmap();
        envMapLUT.setTextureHDR(512, 512, GL_RG, GL_RG16F, GL_FLOAT, GL_LINEAR);


        objectModel.loadModel("models/shaderball/shaderball.a");
        
        objectAlbedo2.setTexture("textures/pbr/rustediron/rustediron_albedo.png", "ironAlbedo", true);
        objectNormal2.setTexture("textures/pbr/rustediron/rustediron_normal.png", "ironNormal", true);
        objectRoughness2.setTexture("textures/pbr/rustediron/rustediron_roughness.png", "ironRoughness", true);
        objectMetalness2.setTexture("textures/pbr/rustediron/rustediron_metalness.png", "ironMetalness", true);
        objectAO2.setTexture("textures/pbr/rustediron/rustediron_ao.png", "ironAO", true);

        objectModel2.loadModel("models/shaderball/shaderball.a");

       // materialF0 = glm::vec3(1.0f, 0.72f, 0.29f);
        materialF0 = glm::vec3(0.04f);
        //materialF0 = glm::vec3(.0f, .0f, .0f);
        //---------------
        // Shape(s)
        //---------------
        envCubeRender.setShape("cube", glm::vec3(0.0f));
        quadRender.setShape("quad", glm::vec3(0.0f));

        //---------------------------------------------------------
        // Set the samplers for the lighting/post-processing passes
        //---------------------------------------------------------

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(0, "gPosition");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(1, "gAlbedo");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(2, "gNormal");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(3, "gEffects");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(4, "sao");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(5, "envMap");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(6, "envMapIrradiance");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(7, "envMapPrefilter");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(8, "envMapLUT");

        m_Pipeline.m_Shaders[ShaderCode::saoShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(0, "gPosition");
        m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(1, "gNormal");

        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(1, "sao");
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(2, "gEffects");

        m_Pipeline.m_Shaders[ShaderCode::latlongToCubeShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::latlongToCubeShader]->Set1i(0, "envMap");

        m_Pipeline.m_Shaders[ShaderCode::irradianceIBLShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::irradianceIBLShader]->Set1i(0, "envMap");

        m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::prefilterIBLShader]->Set1i(0, "envMap");
            
        gBufferSetup();

        //------------
        // SAO setup
        //------------
        saoSetup();


        //---------------------
        // Postprocessing setup
        //---------------------
        postprocessSetup();


        //----------
        // IBL setup
        //----------
        iblSetup();
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
            std::shared_ptr<ModelPBR> temp = std::make_shared<ModelPBR>();
            temp->loadModel(path);
            m_Pipeline.m_ModelLibrary.insert(std::make_pair(path, std::move(temp)));
        }
        ModelObj model;
        model.m_Transform = to_Model_Matrix(transform);
        model.m_Model = m_Pipeline.m_ModelLibrary[path];
        m_Pipeline.m_Models.push_back(model);
    }

    void RenderSystem::SubmitModel(const string& path, const glm::mat4& model_matrix)
    {
        if (!m_Pipeline.m_ModelLibrary.count(path))
        {
            std::shared_ptr<ModelPBR> temp = std::make_shared<ModelPBR>();
            temp->loadModel(path);
            m_Pipeline.m_ModelLibrary.insert(std::make_pair(path, std::move(temp)));
        }
        ModelObj model;
        model.m_Transform = model_matrix;
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

        m_Renderer.ClearColour(clearColor.x,clearColor.y,clearColor.z,clearColor.w);
        m_Renderer.ClearColorDepth();
        if (PREFABMODE)
        {
            m_Renderer.EnableDepthMask(true);
            m_Renderer.EnableDepthTest(true);
            m_Renderer.EnableCulling(false, false, false);
            m_Renderer.EnableBlend(true);
        }
        else
        {
            int WIDTH = getWidth(), HEIGHT = getHeight();
            glViewport(0, 0, WIDTH, HEIGHT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }
    }

    void RenderSystem::Render()
    {
        //if (PREFABMODE)
        //{ 
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //    // Directional light depth map
        //    if (!dir_lights[0].hide)
        //    {
        //        dir_lights[0].Bind();
        //
        //        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //        lightView = glm::lookAt(10.f * dir_lights[0].Direction,
        //            glm::vec3(0.0f, 0.0f, 0.0f),
        //            glm::vec3(0.0f, 1.0f, 0.0f));
        //        lightSpaceMatrix = lightProjection * lightView;
        //
        //        DrawSprites(MeshCode::CUBE, ShaderCode::DIRECTIONAL_SHADOW_MAP);
        //        DrawSprites(MeshCode::SPHERE, ShaderCode::DIRECTIONAL_SHADOW_MAP);
        //        DrawSprites(MeshCode::PLANE, ShaderCode::DIRECTIONAL_SHADOW_MAP);
        //        DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::DIRECTIONAL_SHADOW_MAP);
        //
        //        for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
        //        {
        //            for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
        //            {
        //                // Send in uniform values
        //                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Bind();
        //                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(6, "shadowMap"); // Set Shadow map for directional light to be slot 6 
        //                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(1, "meshDrawing"); // 1 for meshdrawing
        //                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //                mesh.Bind();
        //                glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //                mesh.Unbind();
        //            }
        //        }
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    }
        //
        //
        //
        //    // render all pt lights to depth buffer
        //    if (GetActivePt_lightsNum())
        //    {
        //        for (int numPt = 0; numPt < pt_lights.size(); numPt++)
        //        {
        //            if (pt_lights[numPt].hide)
        //                continue;
        //
        //            // Bind Point Light FBO
        //            pt_lights[numPt].Bind();
        //
        //            DrawSprites(MeshCode::CUBE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
        //            DrawSprites(MeshCode::SPHERE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
        //            DrawSprites(MeshCode::PLANE, ShaderCode::POINT_LIGHT_DEPTH, numPt);
        //            DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::POINT_LIGHT_DEPTH, numPt);
        //
        //            m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Bind();
        //            // Send in uniform values
        //            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_ShadowBuffer.m_Width / (float)m_ShadowBuffer.m_Height, near_plane, far_plane);
        //            std::vector<glm::mat4> shadowTransforms;
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        //            shadowTransforms.push_back(shadowProj * glm::lookAt(pt_lights[numPt].Position, pt_lights[numPt].Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        //            for (unsigned int shadowxform = 0; shadowxform < 6; ++shadowxform)
        //                m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetMat4fv(shadowTransforms[shadowxform], ("shadowMatrices[" + std::to_string(shadowxform) + "]").c_str());
        //            m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1f(far_plane, "far_plane");
        //            m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetVec3f(pt_lights[numPt].Position, "lightPos");
        //            m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1i(5, "depthMap"); // Set Shadow map for directional light to be slot 5
        //            m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->Set1i(1, "meshDrawing"); // 1 for meshdrawing
        //
        //            for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
        //            {
        //                for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
        //                {
        //                    m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //                    mesh.Bind();
        //                    glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //                    mesh.Unbind();
        //                }
        //            }
        //        }
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    }
        //
        //    // Bind default FBO
        //    m_FrameBuffer.Bind();
        //    m_FrameBuffer.SetFrameBufferSize();
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    m_Renderer.EnableBlend(true);
        //
        //    RenderAAGrid();
        //
        //    // Drawing Polygons
        //    DrawSprites(MeshCode::CUBE, ShaderCode::LIGHTING);
        //    DrawSprites(MeshCode::SPHERE, ShaderCode::LIGHTING);
        //    DrawSprites(MeshCode::PLANE, ShaderCode::LIGHTING);
        //    DrawSprites(MeshCode::ICOSAHEDRON, ShaderCode::LIGHTING);
        //
        //    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
        //    m_FrameBuffer.Bind();
        //    glActiveTexture(GL_TEXTURE0);
        //
        //    // Drawing Models
        //    for (size_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
        //    {
        //        for (auto& [mesh, material] : m_Pipeline.m_Models[i].m_Model->GetMeshes())
        //        {
        //            if (material->DiffuseMap)
        //            {
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Bind();
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(glm::transpose(glm::inverse(m_Pipeline.m_Models[i].m_Transform)), "NormMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f((material->Diffuse), "DiffuseColour");
        //                material->DiffuseMap->Bind(0);
        //            }
        //
        //            else if (material->BaseTexture)
        //            {
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Bind();
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(glm::transpose(glm::inverse(m_Pipeline.m_Models[i].m_Transform)), "NormMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f((material->Diffuse), "DiffuseColour");
        //                material->BaseTexture->Bind(0);
        //                //const GLfloat near_plane = 1.0f, far_plane = 25.0f;
        //
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(dir_lights[0].Color, "LightColor");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(dir_lights[0].Direction, "LightDirection");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(dir_lights[0].Intensity, "LightIntensity");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(far_plane, "far_plane");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i((int)pt_lights.size(), "PointLightNumber");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        //
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
        //                for (unsigned int ptLight = 0; ptLight < (unsigned int)pt_lights.size(); ++ptLight)
        //                {
        //                    std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
        //                    std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
        //                    std::string pointLightColors = "pointLightColors[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->SetVec3f(pt_lights[ptLight].Color, pointLightColors.data());
        //                    std::string pointLightConsts = "pointLightConsts[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightConsts, pointLightConsts.data());
        //                    std::string pointLightLinears = "pointLightLinears[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightLinears, pointLightLinears.data());
        //                    std::string pointLightQuads = "pointLightQuads[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(pt_lights[ptLight].pointLightQuads, pointLightQuads.data());
        //
        //                    std::string pointLightHide = "pointLightHide[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(pt_lights[ptLight].hide, pointLightHide.data());
        //                }
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(0, "diffuseTexture");   // Set Point light depth to be slot 
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on
        //
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(ambientStrength, "ambientStrength");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(shininess, "shininess");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1f(specularStrength, "specularStrength");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_TEXTURE]->Set1i(dir_lights[0].hide ? 0 : 1, "DirectionalLightOn");
        //
        //            }
        //
        //            //else
        //            //{
        //            //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->Bind();
        //            //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //            //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Cameras[0].GetProjectionMatrix(), "ProjectionMatrix");
        //            //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetMat4fv(m_Pipeline.m_Cameras[0].GetViewMatrix(), "ViewMatrix");
        //            //    m_Pipeline.m_Shaders[ShaderCode::MODEL]->SetVec3f(to_glvec3(material->Diffuse), "DiffuseColour");
        //            //}
        //
        //            //mesh.Bind();
        //            //glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //            else
        //            {
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Bind();
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetProjectionMatrix(), "ProjectionMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Cameras.front().GetViewMatrix(), "ViewMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(glm::transpose(glm::inverse(m_Pipeline.m_Models[i].m_Transform)), "NormMatrix");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(dir_lights[0].Color, "LightColor");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(dir_lights[0].Direction, "LightDirection");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(dir_lights[0].Intensity, "LightIntensity");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(far_plane, "far_plane");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i((int)pt_lights.size(), "PointLightNumber");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        //
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
        //                for (unsigned int ptLight = 0; ptLight < (unsigned int)pt_lights.size(); ++ptLight)
        //                {
        //                    std::string PointLightPositions = "PointLightPositions[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(pt_lights[ptLight].Position, PointLightPositions.data());
        //                    std::string PointLightIntensity = "PointLightIntensity[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].Intensity, PointLightIntensity.data());
        //                    std::string pointLightColors = "pointLightColors[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f(pt_lights[ptLight].Color, pointLightColors.data());
        //                    std::string pointLightConsts = "pointLightConsts[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightConsts, pointLightConsts.data());
        //                    std::string pointLightLinears = "pointLightLinears[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightLinears, pointLightLinears.data());
        //                    std::string pointLightQuads = "pointLightQuads[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(pt_lights[ptLight].pointLightQuads, pointLightQuads.data());
        //
        //                    std::string pointLightHide = "pointLightHide[" + std::to_string(ptLight) + "]";
        //                    m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(pt_lights[ptLight].hide, pointLightHide.data());
        //                }
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(0, "diffuseTexture");   // Set Point light depth to be slot 
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on
        //
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(ambientStrength, "ambientStrength");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(shininess, "shininess");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1f(specularStrength, "specularStrength");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->Set1i(dir_lights[0].hide ? 0 : 1, "DirectionalLightOn");
        //                m_Pipeline.m_Shaders[ShaderCode::MODEL_LIGHT]->SetVec3f((material->Diffuse), "DiffuseColour");
        //            }
        //
        //            mesh.Bind();
        //            glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
        //            if (material->BaseTexture)
        //            {
        //                material->BaseTexture->Unbind(0);
        //            }
        //        }
        //    }
        //    m_LineRenderer.Render(m_Pipeline.m_Cameras[0].GetViewProjectionMatrix(), m_Pipeline.m_Shaders[ShaderCode::LINE]);
        //    m_FrameBuffer.Unbind();
        //}

        //else
        //{
        SubmitModel("models/shaderball/shaderball.a", glm::mat4{1});
        //SubmitModel("models/shaderball/shaderball.a", glm::mat4{1});

        int WIDTH = getWidth(), HEIGHT = getHeight();
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (uint32_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
        {
            projViewModel = GetCamera().GetProjectionMatrix() * GetCamera().GetViewMatrix() * m_Pipeline.m_Models[i].m_Transform;

            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetMat4fv(GetCamera().GetProjectionMatrix(), "projection");
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetMat4fv(GetCamera().GetViewMatrix(), "view");
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetMat4fv(projViewModel, "projViewModel");
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetMat4fv(prevProjViewModel, "prevProjViewModel");
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "model");
            //m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->SetVec3f(albedoColor, "albedoColor");
            prevProjViewModel = projViewModel;

            glActiveTexture(GL_TEXTURE0);
            objectAlbedo.useTexture();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Set1i(0, "texAlbedo");

            glActiveTexture(GL_TEXTURE1);
            objectNormal.useTexture();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Set1i(1, "texNormal");

            glActiveTexture(GL_TEXTURE2);
            objectRoughness.useTexture();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Set1i(2, "texRoughness");

            glActiveTexture(GL_TEXTURE3);
            objectMetalness.useTexture();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Set1i(3, "texMetalness");

            glActiveTexture(GL_TEXTURE4);
            objectAO.useTexture();
            m_Pipeline.m_Shaders[ShaderCode::gBufferShader]->Set1i(4, "texAO");

            m_Pipeline.m_Models[i].m_Model->Draw();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //---------------
        // sao rendering
        //---------------
        //glQueryCounter(queryIDSAO[0], GL_TIMESTAMP);
        glBindFramebuffer(GL_FRAMEBUFFER, saoFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        if (saoMode)
        {
            // SAO noisy texture
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);

            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(saoSamples, "saoSamples");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1f(saoRadius, "saoRadius");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(saoTurns, "saoTurns");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1f(saoBias, "saoBias");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1f(saoScale, "saoScale");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1f(saoContrast, "saoContrast");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(WIDTH, "viewportWidth");
            m_Pipeline.m_Shaders[ShaderCode::saoShader]->Set1i(HEIGHT, "viewportHeight");

            quadRender.drawShape();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // SAO blur pass
            glBindFramebuffer(GL_FRAMEBUFFER, saoBlurFBO);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Pipeline.m_Shaders[ShaderCode::saoBlurShader]->Bind();
            m_Pipeline.m_Shaders[ShaderCode::saoBlurShader]->Set1i(saoBlurSize, "saoBlurSize");
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, saoBuffer);

            quadRender.drawShape();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Directional light depth map

        if (!dir_lights[0].hide)
        {
            dir_lights[0].Bind();

            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightView = glm::lookAt(10.f * -dir_lights[0].Direction,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            lightSpaceMatrix = lightProjection * lightView;

            for (uint32_t j = 0; j < m_Pipeline.m_Models.size(); ++j)
            {
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Bind();
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(9, "shadowMap"); // Set Shadow map for directional light to be slot 6 
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->Set1i(1, "meshDrawing"); // 1 for meshdrawing
                m_Pipeline.m_Shaders[ShaderCode::DIRECTIONAL_SHADOW_MAP]->SetMat4fv(m_Pipeline.m_Models[j].m_Transform, "ModelMatrix");

                m_Pipeline.m_Models[j].m_Model->Draw();

                glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
                glViewport(0, 0, WIDTH, HEIGHT);
            }          
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

                for (uint32_t i = 0; i < m_Pipeline.m_Models.size(); ++i)
                {
                    m_Pipeline.m_Shaders[ShaderCode::POINT_LIGHT_DEPTH]->SetMat4fv(m_Pipeline.m_Models[i].m_Transform, "ModelMatrix");
                    m_Pipeline.m_Models[i].m_Model->Draw();
                }

            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind 
            glViewport(0, 0, WIDTH, HEIGHT);
        }

        //------------------------
        // Lighting Pass rendering
        //------------------------

        glBindFramebuffer(GL_FRAMEBUFFER, postprocessFBO);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gEffects);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, saoBlurBuffer);
        glActiveTexture(GL_TEXTURE5);
        envMapHDR.useTexture();
        glActiveTexture(GL_TEXTURE6);
        envMapIrradiance.useTexture();
        glActiveTexture(GL_TEXTURE7);
        envMapPrefilter.useTexture();
        glActiveTexture(GL_TEXTURE8);
        envMapLUT.useTexture();

        glActiveTexture(GL_TEXTURE9);
        glBindTexture(GL_TEXTURE_2D, dir_lights[0].m_depthmap);
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(9, "shadowMap");

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_CUBE_MAP, pt_lights[0].m_cubemap);
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(10, "shadowCube");

        for (int pt_light_ = 0; pt_light_ < pt_lights.size(); pt_light_++) //pt_lights.size()
        {
            glm::vec3 lightPositionViewSpace = glm::vec3(GetCamera().GetViewMatrix() * glm::vec4(pt_lights[pt_light_].Position, 1.0f));

            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec3f(lightPositionViewSpace, ("lightPointArray[" + std::to_string(pt_light_) + "].position").c_str());
            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec4f(pt_lights[pt_light_].Color, ("lightPointArray[" + std::to_string(pt_light_) + "].color").c_str());
            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1f(pt_lights[pt_light_].radius, ("lightPointArray[" + std::to_string(pt_light_) + "].radius").c_str());

            std::string pointLightHide = "pointLightHide[" + std::to_string(pt_light_) + "]";
            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(pt_lights[pt_light_].hide, pointLightHide.data());
        }

        for (int dLight = 0; dLight < dir_lights.size(); dLight++)
        {
            glm::vec3 lightDirectionViewSpace = glm::vec3(GetCamera().GetViewMatrix() * glm::vec4(dir_lights[dLight].Direction, 0.0f));

            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec3f(lightDirectionViewSpace, ("lightDirectionalArray[" + std::to_string(dLight) + "].direction").c_str());
            m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec4f(dir_lights[dLight].Color, ("lightDirectionalArray[" + std::to_string(dLight) + "].color").c_str());
        }

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetMat4fv(glm::transpose(GetCamera().GetViewMatrix()), "inverseView");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetMat4fv(glm::inverse(GetCamera().GetProjectionMatrix()), "inverseProj");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetMat4fv(GetCamera().GetViewMatrix(), "view");

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1f(materialRoughness, "materialRoughness");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1f(materialMetallicity, "materialMetallicity");

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec3f(materialF0, "materialF0");

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1f(ambientIntensity, "ambientIntensity");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(gBufferView, "gBufferView");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(pointMode, "pointMode");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(directionalMode, "directionalMode");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(iblMode, "iblMode");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(attenuationMode, "attenuationMode");


        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetMat4fv(GetCamera().GetProjectionMatrix(), "proj");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec3f(GetCamera().GetPosition(), "camPos");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1f(far_plane, "far_plane");

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(dirShadowBool, "dirShadowBool");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(pointShadowBool, "pointShadowBool");
        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->SetVec4f(clearColor, "clearColor");

        m_Pipeline.m_Shaders[ShaderCode::lightingBRDFShader]->Set1i(envMapShow, "envMapShow");
        
        quadRender.drawShape();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //glClear(GL_COLOR_BUFFER_BIT);
        //-------------------------------
        // Post-processing Pass rendering
        //-------------------------------

        glClear(GL_COLOR_BUFFER_BIT);

        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Bind();
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(gBufferView, "gBufferView");;


        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->SetVec2f(glm::vec2{ 1.0f / (float)WIDTH , 1.0f / (float)HEIGHT }, "screenTextureSize");;

        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1f(cameraAperture, "cameraAperture");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1f(cameraShutterSpeed, "cameraShutterSpeed");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1f(cameraISO, "cameraISO");;

        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(saoMode, "saoMode");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(fxaaMode, "fxaaMode");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(motionBlurMode, "motionBlurMode");; //wtf is this like come on dude
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1f(fps / 144.f, "motionBlurScale");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(motionBlurMaxSamples, "motionBlurMaxSamples");;
        m_Pipeline.m_Shaders[ShaderCode::firstpassPPShader]->Set1i(tonemappingMode, "tonemappingMode");;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postprocessBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, saoBlurBuffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gEffects);

        quadRender.drawShape();


        //-----------------------
        // Forward Pass rendering
        //-----------------------
        //glQueryCounter(queryIDForward[0], GL_TIMESTAMP);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        // Copy the depth informations from the Geometry Pass into the default framebuffer
        glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //}



        
        // Shape(s) rendering
        //if (pointMode)
        //{
        //   /* simpleShader.useShader();*/

        //    m_Pipeline.m_Shaders[ShaderCode::simpleShader]->Bind();
        //    glUniformMatrix4fv(glGetUniformLocation(simpleShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //    glUniformMatrix4fv(glGetUniformLocation(simpleShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //    m_Pipeline.m_Shaders[ShaderCode::simpleShader]->SetMat4fv(GetCamera().GetProjectionMatrix(), "projection");
        //    m_Pipeline.m_Shaders[ShaderCode::simpleShader]->SetMat4fv(GetCamera().GetViewMatrix(), "view");

        //    for (int i = 0; i < Light::lightPointList.size(); i++)
        //    {
        //        glUniform4f(glGetUniformLocation(simpleShader.Program, "lightColor"), Light::lightPointList[i].getLightColor().r, Light::lightPointList[i].getLightColor().g, Light::lightPointList[i].getLightColor().b, Light::lightPointList[i].getLightColor().a);

        //        if (Light::lightPointList[i].isMesh())
        //            Light::lightPointList[i].lightMesh.drawShape(simpleShader, view, projection, camera);
        //    }
        //}  
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
            shader->Set1i((int)pt_lights.size(), "PointLightNumber");
            shader->SetMat4fv(lightSpaceMatrix,         "lightSpaceMatrix");

            shader->SetVec3f(m_Pipeline.m_Cameras.front().GetPosition(), "CamPosition");
            for (unsigned int ptLight = 0; ptLight < (unsigned int)pt_lights.size(); ++ptLight)
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

                std::string pointLightHide     = "pointLightHide[" + std::to_string(ptLight) + "]";
                shader->Set1i(pt_lights[ptLight].hide, pointLightHide.data());
            }
            shader->Set1i(5, "depthMap");   // Set Point light depth to be slot 5
            shader->Set1i(6, "shadowMap");  // Set Dir Light depthh to be slot 6
            shader->Set1i(GammaCorrection, "GammaCorrection"); // Send in if Gamma correction is on

            shader->Set1f(ambientStrength,            "ambientStrength");
            shader->Set1f(shininess,                  "shininess");
            shader->Set1f(specularStrength,           "specularStrength");
            shader->Set1i(dir_lights[0].hide ? 0 : 1 ,"DirectionalLightOn");
           // shader->SetMat3fv()

            break;
        case (ShaderCode::DIRECTIONAL_SHADOW_MAP):
        {
            // Send in uniform values

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
        gBufferSetup();
        saoSetup();
        postprocessSetup();
        iblSetup();
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

    uint32_t RenderSystem::getHeight()
    {
       return m_FrameBuffer.GetHeight();
    }
    uint32_t RenderSystem::getWidth()
    {
       return m_FrameBuffer.GetWidth();
    }
}