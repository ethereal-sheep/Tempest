/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "DiagnosticsWindow.h"
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Audio/AudioEngine.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include "Profiler/Profiler.h"

namespace Tempest
{
	void DiagnosticsWindow::init(Instance&)
	{

	}
	void DiagnosticsWindow::show(Instance& instance)
	{
		if (ImGui::Begin(window_name(), &visible, window_flags))
		{
			if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Runtime"))
				{
					Runtime(instance);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Profiler"))
				{
					Profiler();
					ImGui::EndTabItem();
				}


				if (ImGui::BeginTabItem("ECS Usage"))
				{
					ECSUsage(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Memory Usage"))
				{
					MemoryUsage(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Camera"))
				{
					Camera(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Audio"))
				{
					Audio(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Textures"))
				{
					Textures(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Mouse"))
				{
					Mouse(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Light"))
				{
					Light(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Particles"))
				{
					Particles(instance);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

	}

	void DiagnosticsWindow::Runtime(Instance& instance)
	{
		UI::RenderText("Project Name:", 100.f); ImGui::Text(instance.get_name().c_str());
		UI::RenderText("Project Path:", 100.f); ImGui::Text(instance.get_path().string().c_str());
		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("Running FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(io.Framerate).c_str());
		ShowFPSGraph();

		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		if (ImGui::Button("Trigger Error"))
			Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Error triggered by Diagnostics!");
		if (ImGui::Button("Trigger TEvent<string>"))
			Service<EventManager>::Get().instant_dispatch<TEvent<string>>("Error triggered by Diagnostics!");

	}

	void DiagnosticsWindow::ShowFPSGraph()
	{
		const int max = 144;
		const int min = 0;
		ImGuiIO& io = ImGui::GetIO();

		// dummy data
		float newValue = io.Framerate;
		auto& fps_ = Service<RenderSystem>::Get().fps;
		fps_ = newValue;

		fps_values.push_back(newValue);

		if (fps_values.size() > graphsize)
			fps_values.erase(fps_values.begin());


		const float mod = (max - min) * 0.15f;

		// hard coding the min max
		const std::string& text{
			std::to_string(max).c_str() + std::string{"\n\n\n\n\n\n"} + std::to_string(min).c_str()
		};
		ImGui::PlotLines(
			text.c_str(),
			fps_values.data(),
			static_cast<int>(fps_values.size()),
			0,
			nullptr,
			min - mod,
			max + mod,
			ImVec2(0, 100.0f));
	}

	void DiagnosticsWindow::ECSUsage(Instance& instance)
	{
		ImGui::Text("Total entities created:       %u", instance.ecs.size());
		ImGui::DragFloat3("Map Pos", glm::value_ptr(instance.scene.get_map().map_pos), 1.f);
	}


	void DiagnosticsWindow::MemoryUsage(Instance& instance)
	{
		if (!instance.has_debug())
		{
			ImGui::Text("MEMORY DEBUG NOT ENABLED");
			return;
		}

		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text(instance.get_debug()->get_name().c_str());
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 0.25));

		if (ImGui::CollapsingHeader("Statistical Data", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			StatisticalData(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
		if (ImGui::CollapsingHeader("Allocation History", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			AllocationHistory(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
	}

	void DiagnosticsWindow::StatisticalData(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Total bytes allocated:        %u", mr.bytes_allocated());
		ImGui::Text("Total bytes deallocated:      %u", mr.bytes_deallocated());
		ImGui::Text("Highest memory usage:         %u", mr.bytes_highwater());
		ImGui::Text("Outstanding Bytes:            %u", mr.bytes_outstanding());
		ImGui::Text("Outstanding Blocks:           %u", mr.blocks_outstanding());
	}

	void DiagnosticsWindow::AllocationHistory(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Last allocated address:       %p", mr.last_allocated_address());
		ImGui::Text("Last allocated bytes:         %u", mr.last_allocated_num_bytes());
		ImGui::Text("Last allocated alignment:     %u", mr.last_allocated_alignment());

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text("Last deallocated address:     %p", mr.last_deallocated_address());
		ImGui::Text("Last deallocated bytes:       %u", mr.last_deallocated_num_bytes());
		ImGui::Text("Last deallocated alignment:   %u", mr.last_deallocated_alignment());
	}

	void DiagnosticsWindow::Camera(Instance&)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		const auto padding = 80.f;
		{

			auto pos = cam.GetPosition();

			if (UI::DragFloat3ColorBox("Position", "##CameraPosDrag", ImVec2{ padding , 0.f }, value_ptr(pos), 0.f, 0.1f).first)
				cam.SetPosition(pos);
		}

		{
			auto rotation = cam.GetQuatRotation();
			auto eulerDeg = glm::degrees(glm::eulerAngles(rotation));

			if (UI::DragFloat3ColorBox("Rotation", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(eulerDeg), 0.f, 1.f).first)
			{
				//cam.SetRotation(eulerDeg);
			}
		}
		{
			auto q = cam.GetQuatRotation();

			float roll = (float)atan2(2.0f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
			float pitch = (float)asin(-2.0f * (q.x * q.z - q.w * q.y));
			float yaw = (float)atan2(2.0f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);

			ImGui::Selectable("Yaw", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Yaw");
			if (ImGui::DragFloat(" ", &yaw, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Pitch", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Pitch");
			if (ImGui::DragFloat(" ", &pitch, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Roll", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Roll");
			if (ImGui::DragFloat(" ", &roll, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
		}
		{


			auto up = cam.GetUp();
			auto front = cam.GetFront();
			auto left = cam.GetLeft();
			if (UI::DragFloat3ColorBox("up", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(up), 0.f, 1.f).first) {}
			if (UI::DragFloat3ColorBox("front", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(front), 0.f, 1.f).first) {}
			if (UI::DragFloat3ColorBox("left", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(left), 0.f, 1.f).first) {}



		}
	}

	void DiagnosticsWindow::Audio(Instance& )
	{
		if (ImGui::CollapsingHeader("2D Sounds", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Sounds2D/)"))
			{
				auto path = entry.path();
				auto selected = entry.path() == player.GetCurrentTrack();
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					player.SelectNewTrack(path);
				}
			}
		}
		if (ImGui::CollapsingHeader("3D Sounds", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Sounds3D/)"))
			{
				auto path = entry.path();
				auto selected = entry.path() == player.GetCurrentTrack();
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					player.SelectNewTrack(path);
				}
			}
		}

		UI::PaddedSeparator(0.5f);

		ImGui::Text("Current: %s", player.GetCurrentTrack().string().c_str());

		player.Draw();

		UI::PaddedSeparator(0.5f);

		static float a = 0.f;
		ImGui::DragFloat("Test 3D Pos (x)", &a, 0.01f);
		if (ImGui::Button("Play 3D Sound"))
		{
			AudioEngine ae;
			SoundDefinition sd;

			sd.m_Pos = vec3{ a, 0.f, 0.f };
			ae.Play("Sounds3D/Hit.wav", "SFX", sd);
		}
	}

	void DiagnosticsWindow::Mouse(Instance& instance)
	{
		{
			auto& cam = Service<RenderSystem>::Get().GetCamera();
			auto& io = ImGui::GetIO();
			glm::vec4 lRayStart_NDC(
				((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
				((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
				-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
				1.0f
			);
			glm::vec4 lRayEnd_NDC(
				((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f,
				((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f,
				0.0,
				1.0f
			);

			glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
			glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
			glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
			glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
			lRayDir_world = glm::normalize(lRayDir_world);

			auto start = cam.GetPosition();
			auto end = cam.GetPosition() + lRayDir_world * 1000.0f;

			ImGui::Text("Start:       %.3f , %.3f,  %.3f", start.x, start.y, start.z);
			ImGui::Text("End:         %.3f , %.3f,  %.3f", end.x, end.y, end.z);
			ImGui::Text("MousePos:         %.3f , %.3f", io.MousePos.x, io.MousePos.y);

			float dist = 0;
			if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
			{
				auto inter = cam.GetPosition() + lRayDir_world * dist;
				ImGui::Text("Intersect:   %.3f , %.3f,  %.3f", inter.x, inter.y, inter.z);
			}
			else
			{
				ImGui::Text("Not facing plane!");
			}

			//auto [id, check] = instance.po.raycast(cam.GetPosition(), lRayDir_world);
			//if (check)
			//{
			//	ImGui::Text("HIT! id: %u", id);
			//	if (ImGui::IsMouseClicked(0))
			//	{
			//		instance.selected = id;
			//	}
			//}
			//
			//else
			//	ImGui::Text("NO HIT!");

			if (instance.selected != NULL)
			{
				
				auto& test = Service<RenderSystem>::Get().m_Pipeline;
				if (test.m_ModelLibrary.size() > 0)
				{
					int count = 0;
					for (auto mdl : test.m_ModelLibrary)
					{						
						for (uint32_t j = 0; j < mdl.second->colours.size(); ++j)
						{
							std::string label = "Model" + std::to_string(count) + "Color" + std::to_string(j);
							vec3 color = mdl.second->colours[j];
							std::string id = "##Model" + std::to_string(count) + "Color" + std::to_string(j);
							UI::DragFloat3ColorBox(label.data(), id.data(), ImVec2{ 80.f , 0.f }, value_ptr(mdl.second->colours[j]), 0.f, 0.01f, 0.f, 1.f);
						}
						count++;
					}
				}
			}
			else
				ImGui::Text("None Selected!");
		}
	}

	void DiagnosticsWindow::Light(Instance& )
	{
		/// TEST
		if (ImGui::CollapsingHeader("New Graphics"))
		{
			auto& m_pos = Service<RenderSystem>::Get().modelPosition;
			const auto padding = 80.f;
			{
				UI::DragFloat3ColorBox("Position", "##ModelPosDrag", ImVec2{ padding , 0.f }, value_ptr(m_pos), 0.f, 0.1f).first;
			}

			auto& dirShadowBool = Service<RenderSystem>::Get().dirShadowBool;
			ImGui::Checkbox("Directional shadow", &dirShadowBool);
			auto& pointShadowBool = Service<RenderSystem>::Get().pointShadowBool;
			ImGui::Checkbox("Point shadow", &pointShadowBool);
			auto& pbrBool = Service<RenderSystem>::Get().pbrMode;
			ImGui::Checkbox("PBR Mode", &pbrBool);
			auto& envMapShow = Service<RenderSystem>::Get().envMapShow;
			ImGui::Checkbox("Env Map Show", &envMapShow);
			
			auto& TestPBR = Service<RenderSystem>::Get().TestPBR;
			ImGui::Checkbox("Test PBR", &TestPBR);

			auto& AAgridShow = Service<RenderSystem>::Get().AAgridShow;
			ImGui::Checkbox("AA grid Show ", &AAgridShow);

			auto& materialF0 = Service<RenderSystem>::Get().materialF0;
			UI::DragFloat3ColorBox("MaterialF0", "##MaterialF0", ImVec2{ padding , 0.f }, value_ptr(materialF0), 0.f, 0.1f).first;
			
			auto& rmat = Service<RenderSystem>::Get().materialRoughness;
			ImGui::SliderFloat("Material Roughness", &rmat, 0.0f, 4.0f);

			auto& ambientStrength = Service<RenderSystem>::Get().ambientStrength;
			ImGui::SliderFloat("Global Ambient", &ambientStrength, 0.0f, 3.0f);

			auto& clearColor = Service<RenderSystem>::Get().clearColor;
			UI::DragFloat3ColorBox("Clear Color", "##ClearColor", ImVec2{ padding , 0.f }, value_ptr(clearColor), 0.f, 0.1f).first;

			auto& gammaValue = Service<RenderSystem>::Get().gammaValue;
			//UI::DragFloat3ColorBox("Gamme Value", "##GammeValue", ImVec2{ padding , 0.f }, &gammaValue, 0.f, 0.1f,0.0f,4.0f).first;
			ImGui::SliderFloat("Gamme Value", &gammaValue, 0.0f, 4.0f);
			//auto& thumbnail = Service<RenderSystem>::Get().thumbnailName;
			static std::string thumbnailName{ "test.jpg" };
			ImGui::InputText("thumbnail name", &thumbnailName);
			if (ImGui::Button("Capture Image"))
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				vec2 size = vec2(viewport->Size.x, viewport->Size.y);
				//auto& AAgridShow = Service<RenderSystem>::Get().AAgridShow;
				unsigned char* buffer = new unsigned char[static_cast<size_t>(size.x * size.y * 3 / 2)];
				glReadPixels((GLint)size.x / 4, 0, (GLsizei)size.x / 2, (GLsizei)size.y, GL_RGB, GL_UNSIGNED_BYTE, buffer);
				stbi_flip_vertically_on_write(1);
				//stbi_write_jpg("test.jpg", size.y, size.y, 3, buffer, size.y * 3);
				stbi_write_jpg(thumbnailName.data(), (int)size.x / 2, (int)size.y, 3, buffer, (int)size.x * 3);
			}

			if(ImGui::TreeNode("Gbuffer Mode"))
			{
				auto& gBufferView = Service<RenderSystem>::Get().gBufferView;
				ImGui::RadioButton("Final", &gBufferView, 1);
				ImGui::RadioButton("Position", &gBufferView, 2);
				ImGui::RadioButton("Normal", &gBufferView, 3);
				ImGui::RadioButton("Color", &gBufferView, 4);
				ImGui::RadioButton("Roughness", &gBufferView, 5);
				ImGui::RadioButton("Metalness", &gBufferView, 6);
				ImGui::RadioButton("Depth", &gBufferView, 7);
				ImGui::RadioButton("SAO", &gBufferView, 8);
				ImGui::RadioButton("Velocity", &gBufferView, 9);
				ImGui::RadioButton("Bloom", &gBufferView, 10);
				ImGui::RadioButton("Bloom Blur", &gBufferView, 11);
				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Lighting"))
			{
				auto& pointMode = Service<RenderSystem>::Get().pointMode;
				auto& directionalMode = Service<RenderSystem>::Get().directionalMode;
				auto& iblMode = Service<RenderSystem>::Get().iblMode;

				if (ImGui::TreeNode("Mode"))
				{
					ImGui::Checkbox("Point", &pointMode);
					ImGui::Checkbox("Directional", &directionalMode);
					ImGui::Checkbox("Image-Based Lighting(Make sure Env Map Show is ON)", &iblMode);

					ImGui::TreePop();
				}
				

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Point Lights"))
			{
				auto& pt_lights = Service<RenderSystem>::Get().pt_lights;

				if (ImGui::TreeNode("Variables"))
				{
					for(int pt_light_num = 0; pt_light_num < pt_lights.size(); pt_light_num++)
					{

						if (pt_lights[pt_light_num].hide)
						{
							if (ImGui::Button( (std::string("Turn on Point Light ") + std::to_string(pt_light_num)).c_str()))
							{
								pt_lights[pt_light_num].hide = !pt_lights[pt_light_num].hide;
							}

						}
						else
						{
							if (ImGui::Button((std::string("Turn off Point Light ") + std::to_string(pt_light_num)).c_str()))
							{
								pt_lights[pt_light_num].hide = !pt_lights[pt_light_num].hide;
							}
							std::string PointLightPosition = "Point Light Pos" + std::to_string(pt_light_num);
							std::string PointLightPositionID = "##Pos" + std::to_string(pt_light_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(pt_lights[pt_light_num].Position), 0.f, 0.1f, -10.f, 10.f);
							
							PointLightPosition = "Point Light Color" + std::to_string(pt_light_num);
							PointLightPositionID = "##Color" + std::to_string(pt_light_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(pt_lights[pt_light_num].Color), 0.f, 0.1f, -10.f, 10.f);


							ImGui::SliderFloat((std::string("Point Light Radius ") + std::to_string(pt_light_num)).c_str(), &pt_lights[pt_light_num].radius, 0.0f, 10.0f);
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Attenuation"))
				{
					auto& attenuationMode = Service<RenderSystem>::Get().attenuationMode;
					ImGui::RadioButton("Quadratic", &attenuationMode, 1);
					ImGui::RadioButton("UE4", &attenuationMode, 2);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Directional Light"))
			{
				auto& dir_lights = Service<RenderSystem>::Get().dir_lights;
				
				if (ImGui::TreeNode("Direction"))
				{
					for (int dir_lights_num = 0; dir_lights_num < dir_lights.size(); dir_lights_num++)
					{
						if (!dir_lights[dir_lights_num].hide)
						{
							if (ImGui::Button((std::string("Turn off Directional Light ") + std::to_string(dir_lights_num)).c_str()))
							{
								dir_lights[dir_lights_num].hide = !dir_lights[dir_lights_num].hide;
							}
							std::string PointLightPosition = "Directional Light" + std::to_string(dir_lights_num);
							std::string PointLightPositionID = "##Direction" + std::to_string(dir_lights_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(dir_lights[dir_lights_num].Direction), 0.f, 0.1f, -10.f, 10.f);

						}
						else
						{
							if (ImGui::Button((std::string("Turn on Directional Light ") + std::to_string(dir_lights_num)).c_str()))
							{
								dir_lights[dir_lights_num].hide = !dir_lights[dir_lights_num].hide;
							}
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Color"))
				{
					for (int dir_lights_num = 0; dir_lights_num < dir_lights.size(); dir_lights_num++)
					{
						if (!dir_lights[dir_lights_num].hide)
						{
							std::string PointLightPosition = "Directional Light" + std::to_string(dir_lights_num);
							std::string PointLightPositionID = "##Color" + std::to_string(dir_lights_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(dir_lights[dir_lights_num].Color), 0.f, 0.1f, -10.f, 10.f);
						}
					}
					

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Environment map"))
			{
				auto& envMapHDR = Service<RenderSystem>::Get().envMapHDR;
				if (ImGui::Button("Appartment"))
				{
					envMapHDR.setTextureHDR("textures/hdr/appart.hdr", "appartHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Pisa"))
				{
					envMapHDR.setTextureHDR("textures/hdr/pisa.hdr", "pisaHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Canyon"))
				{
					envMapHDR.setTextureHDR("textures/hdr/canyon.hdr", "canyonHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Loft"))
				{
					envMapHDR.setTextureHDR("textures/hdr/loft.hdr", "loftHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Path"))
				{
					envMapHDR.setTextureHDR("textures/hdr/path.hdr", "pathHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Circus"))
				{
					envMapHDR.setTextureHDR("textures/hdr/circus.hdr", "circusHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("lebombo"))
				{
					envMapHDR.setTextureHDR("textures/hdr/lebombo_4k.hdr", "lebombo", true);
					Service<RenderSystem>::Get().iblSetup();
				}
				if (ImGui::Button("spaichingen_hill_4k.hdr"))
				{
					envMapHDR.setTextureHDR("textures/hdr/spaichingen_hill_4k.hdr", "spaichingen_hill", true);
					Service<RenderSystem>::Get().iblSetup();
				}
				if (ImGui::Button("snowy_hillside"))
				{
					envMapHDR.setTextureHDR("textures/hdr/snowy_hillside_4k.hdr", "snowy_hillside", true);
					Service<RenderSystem>::Get().iblSetup();
				}
				if (ImGui::Button("evening_meadow_4k"))
				{
					envMapHDR.setTextureHDR("textures/hdr/evening_meadow_4k.hdr", "evening_meadow_4k", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Post processing"))
			{
				if (ImGui::TreeNode("SAO"))
				{

					auto& saoMode = Service<RenderSystem>::Get().saoMode;
					ImGui::Checkbox("Enable", &saoMode);

					auto& saoSamples = Service<RenderSystem>::Get().saoSamples;
					ImGui::SliderInt("Samples", &saoSamples, 0, 64);
					auto& saoRadius = Service<RenderSystem>::Get().saoRadius;
					ImGui::SliderFloat("Radius", &saoRadius, 0.0f, 3.0f);
					auto& saoTurns = Service<RenderSystem>::Get().saoTurns;
					ImGui::SliderInt("Turns", &saoTurns, 0, 16);
					auto& saoBias = Service<RenderSystem>::Get().saoBias;
					ImGui::SliderFloat("Bias", &saoBias, 0.0f, 0.1f);
					auto& saoScale = Service<RenderSystem>::Get().saoScale;
					ImGui::SliderFloat("Scale", &saoScale, 0.0f, 3.0f);
					auto& saoContrast = Service<RenderSystem>::Get().saoContrast;
					ImGui::SliderFloat("Contrast", &saoContrast, 0.0f, 3.0f);
					auto& saoBlurSize = Service<RenderSystem>::Get().saoBlurSize;
					ImGui::SliderInt("Blur Size", &saoBlurSize, 0, 8);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("FXAA"))
				{

					auto& fxaaMode = Service<RenderSystem>::Get().fxaaMode;
					ImGui::Checkbox("Enable", &fxaaMode);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Motion Blur"))
				{

					auto& motionBlurMaxSamples = Service<RenderSystem>::Get().motionBlurMaxSamples;

					auto& motionBlurMode = Service<RenderSystem>::Get().motionBlurMode;
					ImGui::Checkbox("Enable", &motionBlurMode);
					ImGui::SliderInt("Max Samples", &motionBlurMaxSamples, 1, 128);

					ImGui::TreePop();
				}

				
				if (ImGui::TreeNode("Tonemapping"))
				{
					auto& tonemappingMode = Service<RenderSystem>::Get().tonemappingMode;
					ImGui::RadioButton("Reinhard", &tonemappingMode, 1);
					ImGui::RadioButton("Filmic", &tonemappingMode, 2);
					ImGui::RadioButton("Uncharted", &tonemappingMode, 3);

					ImGui::TreePop();
				}
				
				if (ImGui::TreeNode("Tilt Shift"))
				{
					auto& tiltShiftMode = Service<RenderSystem>::Get().tiltShiftMode;
					ImGui::Checkbox("Enable", &tiltShiftMode);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Object"))
			{

				auto& modelPosition = Service<RenderSystem>::Get().modelPosition;

				auto& modelRotationSpeed = Service<RenderSystem>::Get().modelRotationSpeed;

				auto& modelRotationAxis = Service<RenderSystem>::Get().modelRotationAxis;

				ImGui::SliderFloat3("Position", (float*)&modelPosition, -5.0f, 5.0f);
				ImGui::SliderFloat("Rotation Speed", &modelRotationSpeed, 0.0f, 50.0f);
				ImGui::SliderFloat3("Rotation Axis", (float*)&modelRotationAxis, 0.0f, 1.0f);

				if (ImGui::TreeNode("Model"))
				{

					auto& objectModel = Service<RenderSystem>::Get().objectModel;
					auto& modelScale = Service<RenderSystem>::Get().modelScale;
					if (ImGui::Button("Sphere"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/sphere/sphere.a");
						modelScale = glm::vec3(0.5f);
					}

					if (ImGui::Button("Teapot"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/teapot/teapot.a");
						modelScale = glm::vec3(0.5f);
					}

					if (ImGui::Button("Shader ball"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/shaderball/shaderball.a");
						modelScale = glm::vec3(0.1f);
					}

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Material"))
			{
				auto& objectAlbedo = Service<RenderSystem>::Get().objectAlbedo;
				auto& objectNormal = Service<RenderSystem>::Get().objectNormal;
				auto& objectRoughness = Service<RenderSystem>::Get().objectRoughness;
				auto& objectMetalness = Service<RenderSystem>::Get().objectMetalness;
				auto& objectAO = Service<RenderSystem>::Get().objectAO;

				if (ImGui::Button("Rusted iron"))
				{
					objectAlbedo.setTexture("textures/pbr/rustediron/rustediron_albedo4", "ironAlbedo", true);
					objectNormal.setTexture("textures/pbr/rustediron/rustediron_normal.png", "ironNormal", true);
					objectRoughness.setTexture("textures/pbr/rustediron/rustediron_roughness.png", "ironRoughness", true);
					objectMetalness.setTexture("textures/pbr/rustediron/rustediron_metalness.png", "ironMetalness", true);
					objectAO.setTexture("textures/pbr/rustediron/rustediron_ao.png", "ironAO", true);

					materialF0 = glm::vec3(0.04f);
				}

				if (ImGui::Button("Gold"))
				{
					objectAlbedo.setTexture("textures/pbr/gold/gold_albedo.png", "goldAlbedo", true);
					objectNormal.setTexture("textures/pbr/gold/gold_normal.png", "goldNormal", true);
					objectRoughness.setTexture("textures/pbr/gold/gold_roughness.png", "goldRoughness", true);
					objectMetalness.setTexture("textures/pbr/gold/gold_metalness.png", "goldMetalness", true);
					objectAO.setTexture("textures/pbr/gold/gold_ao.png", "goldAO", true);

					materialF0 = glm::vec3(1.0f, 0.72f, 0.29f);
				}

				if (ImGui::Button("Woodfloor"))
				{
					objectAlbedo.setTexture("textures/pbr/woodfloor/woodfloor_albedo.png", "woodfloorAlbedo", true);
					objectNormal.setTexture("textures/pbr/woodfloor/woodfloor_normal.png", "woodfloorNormal", true);
					objectRoughness.setTexture("textures/pbr/woodfloor/woodfloor_roughness.png", "woodfloorRoughness", true);
					objectMetalness.setTexture("textures/pbr/woodfloor/woodfloor_metalness.png", "woodfloorMetalness", true);
					objectAO.setTexture("textures/pbr/woodfloor/woodfloor_ao.png", "woodfloorAO", true);

					materialF0 = glm::vec3(0.04f);
				}

				if (ImGui::Button("Terrazzo"))
				{
					objectAlbedo.setTexture("textures/pbr/terrazzo/terrazzo_albedo.png", "terrazzoAlbedo", true);
					objectNormal.setTexture("textures/pbr/terrazzo/terrazzo_normal.png", "terrazzoNormal", true);
					objectRoughness.setTexture("textures/pbr/terrazzo/terrazzo_roughness.png", "terrazzoRoughness", true);
					objectMetalness.setTexture("textures/pbr/terrazzo/terrazzo_metalness.png", "terrazzoMetalness", true);
					objectAO.setTexture("textures/pbr/terrazzo/terrazzo_ao.png", "terrazzoAO", true);

					materialF0 = glm::vec3(0.04f);
				}


				if (ImGui::Button("Porcelain"))
				{
					objectAlbedo.setTexture("textures/pbr/porcelain/porcelain_albedo.png", "porcelainAlbedo", true);
					objectNormal.setTexture("textures/pbr/porcelain/porcelain_normal.png", "porcelainNormal", true);
					objectRoughness.setTexture("textures/pbr/porcelain/porcelain_roughness.png", "porcelainRoughness", true);
					objectMetalness.setTexture("textures/pbr/porcelain/porcelain_metalness.png", "porcelainMetalness", true);
					objectAO.setTexture("textures/pbr/porcelain/porcelain_ao.png", "porcelainAO", true);


					materialF0 = glm::vec3(0.04f);
				}
				ImGui::TreePop();
			}
		}		
	}

	void DiagnosticsWindow::Textures(Instance&)
	{
		if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Assets/)"))
			{
				auto path = entry.path();
				auto selected = curr_tex ? curr_tex->GetName() == path.string() : false;
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					curr_tex = tex_map[path];
				}
			}
		}
		
		// example
		// everything in Resources/Assets will load into this map
		// tex_map["Assets/test_photo.png"] (gets shared_ptr to texture, interface might change in future)
		// if doesn't exist, its nullptr

		UI::PaddedSeparator(0.5f);

		if(curr_tex)
			ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2((float)curr_tex->GetWidth(), (float)curr_tex->GetHeight()));


	}

	void DiagnosticsWindow::Particles(Instance&)
	{
		if (ImGui::CollapsingHeader("Emitters"))
		{
			auto m = EmitterSystem_3D::GetInstance().GetEmitter();
			for (auto& x : m)
			{
				ImGui::Text("Duration: %.2f", x->m_MM.m_duration);
				if (ImGui::TreeNode("Particles"))
				{
					for (auto& y : x->m_particles)
					{
						if (y.m_isActive)
							ImGui::Text("Active: Yes");
						else
							continue;
						glm::vec3 pos = y.m_position;
						glm::vec3 clr = y.m_colour;
						ImGui::Text("Position: %.2f, %.2f, %.2f ", pos.x, pos.y, pos.z);
						ImGui::Text("Life Remaining: %.2f", y.m_lifeRemaining);
						ImGui::Text("Color: %.2f, %.2f, %.2f ", clr.x, clr.y, clr.z);

						UI::PaddedSeparator(0.5f);
					}
					ImGui::TreePop();
				}

			UI::PaddedSeparator(0.5f);
			UI::PaddedSeparator(0.5f);
			}
		}
		if (ImGui::CollapsingHeader("Particle Debugging"))
		{
			auto& p_test = Service<RenderSystem>::Get().p_testing;
			ImGui::Checkbox("Enable Testing", &p_test);

			const auto padding = 80.f;
			auto& p_scale = Service<RenderSystem>::Get().p_scalings;
			std::string pScaling = "Particle Scalings";
			std::string pScalingID = "##Pscale";
			UI::DragFloat3ColorBox(pScaling.data(), pScalingID.data(), ImVec2{ padding , 0.f }, value_ptr(p_scale), 0.f, 0.1f, -10.f, 10.f);

			auto& p_angle = Service<RenderSystem>::Get().p_angles;
			std::string pAngles = "Particle Angles";
			std::string pAnglesID = "##Pangle";
			UI::DragFloat3ColorBox(pAngles.data(), pAnglesID.data(), ImVec2{ padding , 0.f }, value_ptr(p_angle), 0.f, 0.1f, -10.f, 10.f);
		}
	}

	ImVec4 ConvertUint32TOVec4(uint32_t color)
	{
		float s = 1.0f / 255.0f;
		return ImVec4(
			((color >> IM_COL32_A_SHIFT) & 0xFF) * s,
			((color >> IM_COL32_B_SHIFT) & 0xFF) * s,
			((color >> IM_COL32_G_SHIFT) & 0xFF) * s,
			((color >> IM_COL32_R_SHIFT) & 0xFF) * s);
	}


	void DiagnosticsWindow::Profiler()
	{
		Profile::Profiler profile;
		size_t frameCount = profile.GetTotalFrameCount();
		//float dt = profile.GetDeltaTime().count() / 1000000.f;
		float elapsed = profile.GetTimeElapsed().count() / 1000000000.f;

		float barHeight = 25.f;


		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetColorU32(ImGuiCol_Text));

		if (!profile.IsRecording())
		{
			ImGui::Selectable("STOPPED", false, ImGuiSelectableFlags_Disabled, ImVec2(100, barHeight));
		}
		else
		{
			ImGui::Selectable("RECORDING", false, ImGuiSelectableFlags_Disabled, ImVec2(100, barHeight));
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::SameLine();

		if (!profile.IsRecording())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.3f));
			if (ImGui::Button(ICON_FA_PLAY, ImVec2(barHeight, barHeight)))
			{
				profile.StartRecording();
			}
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(2);
		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_PlotLinesHovered));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.3f));
			if (ImGui::Button(ICON_FA_STOP, ImVec2(barHeight, barHeight)))
			{
				profile.StopRecording();
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar(2);

			m_CurrentFrame = frameCount;
		}
		ImGui::SameLine(0, 25);
		ImGui::Text("Time: %6.2fs", elapsed);
		ImGui::SameLine(0, 25);


		ImGui::Text("Frame: ");
		ImGui::SameLine();

		if (!profile.IsRecording())
		{
			if (frameCount)
			{
				int temp = static_cast<int>(m_CurrentFrame);
				ImGui::PushItemWidth(40);
				if (ImGui::DragInt("##frameCounterCurrentFrame", &temp, 1.f, 1, static_cast<int>(frameCount), "%d", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
				{
					m_CurrentFrame = static_cast<size_t>(temp);
				}
				ImGui::PopItemWidth();

			}
			else
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetColorU32(ImGuiCol_Text));
				ImGui::Selectable(std::to_string(0).c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(40, 0));
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}
		}
		else
		{

			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetColorU32(ImGuiCol_Text));
			ImGui::Selectable(std::to_string(m_CurrentFrame).c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(40, 0));
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

		}


		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetColorU32(ImGuiCol_Text));
		ImGui::Text("/");
		ImGui::SameLine();
		ImGui::Selectable(std::to_string(frameCount).c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(40, 0));
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::SameLine(0, 10);

		bool temp = profile.IsStartOnPlay();
		if (ImGui::Checkbox("Record on Play", &temp)) profile.SetStartOnPlay(!profile.IsStartOnPlay());

		ImGui::SameLine(0, 10);

		if (!profile.IsRecording())
		{
			temp = profile.IsHistoryDisabled();
			if (ImGui::Checkbox("Disable History (WIP)", &temp)) profile.SetHistoryCapture(!profile.IsHistoryDisabled());
		}
		else
		{
			const char* str = profile.IsHistoryDisabled() ? ICON_FA_CHECK_SQUARE " History Disabled" : ICON_FA_SQUARE " History Enabled";
			ImGui::TextDisabled(str);
		}

		ImGui::Separator();
		// profiling chart

		ShowProfilingChart();

		ImGui::Separator();
		ImGui::Columns(7, "profileFunctions", true);
		ImGui::Text("Zone"); ImGui::NextColumn();
		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("Location"); ImGui::NextColumn();
		ImGui::Text("Calls"); ImGui::NextColumn();
		ImGui::Text("Time (ms)"); ImGui::NextColumn();
		ImGui::Text("Call Time (ms)"); ImGui::NextColumn();
		ImGui::Text("%%"); ImGui::NextColumn();
		ImGui::Separator();

		for (auto fn : profile.GetFunctionList())
		{
			auto& data = profile.GetFunctionData(fn);

			ImGui::PushStyleColor(ImGuiCol_Text, ConvertUint32TOVec4(profile.GetZoneColor(fn->m_Zone)));
			ImGui::Selectable(ICON_FA_MARKER, false, ImGuiSelectableFlags_SpanAllColumns);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text(fn->m_Zone.c_str());
			ImGui::NextColumn();

			ImGui::Text(fn->m_FunctionName.c_str()); ImGui::NextColumn();
			ImGui::Text("%s:%u", fn->m_Filename.c_str(), fn->m_LineNumber); ImGui::NextColumn();

			float avgCalls = 1.f * data.m_Calls / frameCount;
			float avgRuntime = data.m_Duration.count() / (1000000.f * frameCount);
			float avgCalltime = avgRuntime / avgCalls;
			float percent = 100.f * data.m_Duration.count() / profile.GetTotalTime().count();

			ImGui::Text("%.3f", avgCalls); ImGui::NextColumn();
			ImGui::Text("%f", avgRuntime); ImGui::NextColumn();
			ImGui::Text("%f", avgCalltime); ImGui::NextColumn();
			ImGui::Text("%.3f%%", percent); ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}


	void DiagnosticsWindow::ShowProfilingChart()
	{
		Profile::Profiler profile;

		float defaultDivisor = 1000000.f;

		struct Bar
		{
			Profile::Function key;
			long long start = 0;
			long long duration = 0;
			size_t row = 0;
		};

		if (m_CurrentFrame)
		{
			const auto& [frameStart, dt, sequence] = profile.GetFrameSnapshot(m_CurrentFrame);

			std::vector<Profile::SequenceData> sequenceStack;
			std::vector<Bar> chartData;

			size_t maxRow = 1;
			size_t row = 1;

			for (auto& sd : sequence)
			{
				if (sd.m_Open)
				{
					sequenceStack.push_back(sd);
					++row;
				}
				else
				{
					auto& front = sequenceStack.back();
					LOG_ASSERT(front.m_Key == sd.m_Key); // "Function Keys not the same!");

					Bar newBar{
						front.m_Key,
						(front.m_TimePoint - frameStart).count(),
						(sd.m_TimePoint - frameStart).count(),
						--row
					};
					sequenceStack.pop_back();
					chartData.push_back(std::move(newBar));
				}

				if (row > maxRow)
					maxRow = row;

			}
			LOG_ASSERT(sequenceStack.size() == 0); // , "Seq Stack Corrupted!");

			if(profile.IsRecording())
				ImPlot::SetNextPlotLimits(0, dt.count() / defaultDivisor, 0, 1.0 * maxRow, ImGuiCond_Always);
			else
				ImPlot::SetNextPlotLimits(0, dt.count() / defaultDivisor, 0, 1.0 * maxRow);

			if (ImPlot::BeginPlot("##ProfilerChart", "microseconds", "stack", ImVec2{ -1, maxRow * m_BarHeight + 50.f },
				ImPlotFlags_NoLegend | ImPlotFlags_NoMousePos, ImPlotAxisFlags_None, 
				ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_LockMax | 
				ImPlotAxisFlags_LockMin | ImPlotAxisFlags_NoDecorations))
			{
				const int BUFFER_SIZE = 64;
				char buffer[BUFFER_SIZE] = { 0 };

				auto printToChart = [&](
					const Bar& bar) {
						float r1[2] = { static_cast<float>(bar.row), static_cast<float>(bar.row) };
						float r2[2] = { static_cast<float>(bar.row) + 1.f, static_cast<float>(bar.row) + 1.f };

						float v[2] = { static_cast<float>(bar.start / defaultDivisor), static_cast<float>(bar.duration / defaultDivisor) };

						sprintf_s(buffer, BUFFER_SIZE, "%p", bar.key);

						ImPlot::PushStyleColor(ImPlotCol_Fill, ConvertUint32TOVec4(profile.GetZoneColor(bar.key->m_Zone)));
						ImPlot::PlotShaded(buffer, v, r1, r2, 2);
						ImPlot::PopStyleColor();

						auto [x,y] = ImPlot::GetPlotMousePos();
						if (ImPlot::IsPlotHovered())
						{
							if (x > v[0] && x < v[1] && y > r1[0] && y < r2[0])
							{
								ImGui::BeginTooltip();
								ImGui::Text("%s", bar.key->m_FunctionName.c_str());
								ImGui::PushStyleColor(ImGuiCol_Text, ConvertUint32TOVec4(profile.GetZoneColor(bar.key->m_Zone)));
								ImGui::Text(ICON_FA_MARKER);
								ImGui::PopStyleColor();
								ImGui::SameLine();
								ImGui::Text("%s", bar.key->m_Zone.c_str());

								ImGui::Text("Call time: %f%s", (bar.duration - bar.start) / 1000000000.f, "ms");

								ImGui::EndTooltip();

								return true;
							}
						}
						return false;
				};

				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, .99f);
				Profile::FunctionKey temp{ "NONE", "Root", "", 0 };
				printToChart(Bar{ &temp, 0, dt.count(), 0 });

				for (auto& bar : chartData)
				{
					printToChart(bar);
				}
				ImPlot::PopStyleVar();

				ImPlot::EndPlot();
			}
		}
		else
		{
			ImPlot::SetNextPlotLimits(0, 17000000 / defaultDivisor, 0, 5);
			if (ImPlot::BeginPlot("##ProfilerChart", "microseconds", "stack", ImVec2{ -1, 200 }, ImPlotFlags_CanvasOnly, ImPlotAxisFlags_None, 
				ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_LockMax | ImPlotAxisFlags_LockMin))
			{
				ImPlot::EndPlot();
			}
		}
	}

}