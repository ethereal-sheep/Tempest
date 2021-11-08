/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "../Util/LineDrawing.h"
#include <unordered_map>

namespace Tempest
{
	class AssetManagerWindow : public Window
	{
		LineDrawing line;

		const char* window_name() override
		{
			return ICON_FA_COLUMNS " Prototypes";
		}

		void init(Instance&) override
		{
			window_flags |= ImGuiWindowFlags_MenuBar;
		}

		void show(Instance& instance) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
				{
					if (ImGui::BeginTabBar("##categorytab"))
					{
						// each folder is a category
						draw_category(instance, cat_name, proto_cat);
						ImGui::EndTabBar();
					}
				}


			}
			ImGui::End();

		}

		void draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat)
		{
			if (ImGui::BeginTabItem(cat_name.c_str()))
			{
				for (auto& pair : proto_cat)
				{

					auto& proto_name = pair.first;
					auto& proto = pair.second;
					// each of this json files

					//// check if loaded
					//if (!categories[cat].has(name))
					//{
					//	// try to load
					//	try
					//	{
					//		categories[cat].load_file(entry.path());
					//	}
					//	catch (const std::exception&)
					//	{
					//		// do nothing if we can't
					//		// we can clean if we want as well
					//		return;
					//	}
					//}

					// display
					ImGui::PushID(proto_name.c_str());
					ImGui::BeginGroup();
					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(static_cast<uint64_t>(0)),
						{ icon_size, icon_size },
						ImVec2(0, 1), ImVec2(1, 0), 2
					))
						/*if(ImGui::Button(name.c_str()))*/
					{
						//inspector.select(entry.path(), cat);
					}

					ImGui::Text(proto_name.c_str());
					ImGui::EndGroup();
					ImGui::PopID();

					//UI::DrawLine();
					if (line.draw(cat_name + proto_name,
						[](){}, [&]()
						{
							auto& cam = Service<RenderSystem>::Get().GetCamera();
							auto vp = cam.GetViewport();
							auto width = vp.z;
							auto height = vp.w;

							auto& io = ImGui::GetIO();
							if (io.WantCaptureMouse)
								return;

							glm::vec4 lRayStart_NDC(
								((float)io.MousePos.x / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
								((float)io.MousePos.y / (float)height - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
								-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
								1.0f
							);
							glm::vec4 lRayEnd_NDC(
								((float)io.MousePos.x / (float)width - 0.5f) * 2.0f,
								((float)io.MousePos.y / (float)height - 0.5f) * -2.0f,
								0.0,
								1.0f
							);

							glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
							glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
							glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
							glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
							lRayDir_world = glm::normalize(lRayDir_world);

							auto start = cam.GetPosition();
							float dist = 0;
							if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
							{


								auto [it, b] = instance.scene.get_map().create(proto);
								instance.selected = it->first;
								if (auto transform = it->second.force_if<tc::Transform>())
								{
									auto inter = cam.GetPosition() + lRayDir_world * dist;

									if (auto shape = it->second.get_if<tc::Shape>())
									{
										const int& x = shape->x;
										const int& y = shape->y;

										int a_x = x, a_y = y, e_x = 0, e_y = 0;
										if (a_x % 2 != a_y % 2)
										{
											a_x = a_y = std::min(x, y);
											e_x = x - a_x;
											e_y = y - a_y;

										}

										inter.x = a_x % 2 ? std::floor(inter.x) + .5f : std::round(inter.x);
										inter.y = 0;
										inter.z = a_y % 2 ? std::floor(inter.z) + .5f : std::round(inter.z);


									}

									transform->position = inter;
								}
								instance.action_history.Commit<CreatePrefab>(it->first);


							}
						}
					))
					{
						auto& cam = Service<RenderSystem>::Get().GetCamera();
						auto vp = cam.GetViewport();
						auto width = vp.z;
						auto height = vp.w;

						auto& io = ImGui::GetIO();

						glm::vec4 lRayStart_NDC(
							((float)io.MousePos.x / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
							((float)io.MousePos.y / (float)height - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
							-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
							1.0f
						);
						glm::vec4 lRayEnd_NDC(
							((float)io.MousePos.x / (float)width - 0.5f) * 2.0f,
							((float)io.MousePos.y / (float)height - 0.5f) * -2.0f,
							0.0,
							1.0f
						);

						glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
						glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
						glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
						glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
						lRayDir_world = glm::normalize(lRayDir_world);

						auto start = cam.GetPosition();
						float dist = 0;
						if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
						{

							auto inter = cam.GetPosition() + lRayDir_world * dist;

							if (auto shape = proto.get_if<tc::Shape>())
							{
								const int& x = shape->x;
								const int& y = shape->y;
								const int one = 1;

								AABB box;

								int a_x = x, a_y = y, e_x = 0, e_y = 0;
								if (a_x % 2 != a_y % 2)
								{
									a_x = a_y = std::min(x, y);
									e_x = x - a_x;
									e_y = y - a_y;

								}

								inter.x = a_x % 2 ? std::floor(inter.x) + .5f : std::round(inter.x);
								inter.y = 0;
								inter.z = a_y % 2 ? std::floor(inter.z) + .5f : std::round(inter.z);

								box.min.x = inter.x -.5f - (a_x - 1) / 2.f;
								box.min.z = inter.z -.5f - (a_y - 1) / 2.f;
								box.min.y = 0;

								box.max.x = inter.x + .5f + (a_x - 1) / 2.f + e_x;
								box.max.z = inter.z + .5f + (a_y - 1) / 2.f + e_y;
								box.max.y = 0;

								Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
							}
						}
					}


					/*if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
					{
						

						if (io.MouseReleased[0])
						{
							ImGui::Text("Hi");
						}

						ImGui::EndDragDropSource();
					}*/




					if (ImGui::BeginPopupContextItem(proto_name.c_str()))
					{
						/*if (ImGui::MenuItem(std::string(ICON_FA_TRASH).append(" Delete").c_str()))
						{
							m_ScenePath = scene.GetPath().string();

							if (project.IsStartScene(scene.GetName()))
								m_OpenInvalidDeletePopup = true;
							else
								m_OpenDeleteScenePopup = true;
						}*/

						if (ImGui::MenuItem(ICON_FA_EDIT " Instance"))
						{
							auto [it, b] = instance.scene.get_map().create(proto);
							instance.selected = it->first;
						}

						if (auto meta = proto.get_if<tc::Meta>())
						{
							proto.specialize<tc::Meta>();
							ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
						}

						ImGui::EndPopup();

					}


					ImGui::SameLine();

				}


				ImGui::EndTabItem();
			}


		}

	private:
		float icon_size = 50.f;
	};
}

//#include "../Window.h"
//#include <unordered_map>
//#include "AssetInfo.h"
//
//
//namespace UI
//{
//	class AssetManagerWindow final : public Window
//	{
//	public:
//		AssetManagerWindow()
//			: Window{ "Asset Manager" }
//		{
//		}
//
//		void Init() override;
//		void Show() override;
//		void Shutdown() override;
//		void Unload();
//	private:
//		std::string _WindowName;
//		ImVec2 m_AssetsWinSize;
//		bool _subActive;
//		std::string _AssetIndex;
//		std::string _FileIndex;
//		std::vector<std::string> _fullPathName;
//		std::vector<AssetsInfo> _assetsInfo;
//		std::vector<std::string> _FolderNames;
//
//		std::unordered_map<std::string, std::string> _TreeContents;
//		std::unordered_map<std::string, std::vector<AssetsInfo>> _FolderListContents;
//		std::unordered_map<std::string, std::vector<AssetsInfo>> _SubFolderListContents;
//	};
//}