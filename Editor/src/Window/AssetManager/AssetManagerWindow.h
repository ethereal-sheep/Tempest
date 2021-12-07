/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
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
#include "Util/shape_manip.h"

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
			// skip units
			if (cat_name == "Unit")
				return;

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

							auto& io = ImGui::GetIO();
							if (io.WantCaptureMouse)
								return;

							auto ray = cam.GetMouseRay();
							auto start = cam.GetPosition();
							float dist = 0;
							if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
							{
								auto [it, b] = instance.scene.get_map().create(proto);
								instance.selected = it->first;
								if (auto transform = it->second.force_if<tc::Transform>())
								{
									auto inter = cam.GetPosition() + ray * dist;

									if (auto shape = it->second.get_if<tc::Shape>())
									{
										const int& x = shape->x;
										const int& y = shape->y;

										auto [a_x, a_y, e_x, e_y, o_x, o_y, p_x, p_y] = shape_data_from_position(x, y, inter.x, inter.z);

										inter.x = o_x;
										inter.y = 0;
										inter.z = o_y;
									}

									transform->position = inter;
								}
								instance.action_history.Commit<CreatePrefab>(it->first);


							}
						}
					))
					{
						auto& cam = Service<RenderSystem>::Get().GetCamera();
						
						auto ray = cam.GetMouseRay();
						auto start = cam.GetPosition();
						float dist = 0;
						if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
						{

							auto inter = cam.GetPosition() + ray * dist;

							if (auto shape = proto.get_if<tc::Shape>())
							{
								const int& x = shape->x;
								const int& y = shape->y;

								AABB box;

								auto [a_x, a_y, b_x, b_y] = shape_bounding_with_position(x, y, inter.x, inter.z);

								box.min.x = a_x;
								box.min.z = a_y;
								box.min.y = 0;

								box.max.x = b_x;
								box.max.z = b_y;
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