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
#include "../Util/UIElements.h"
#include <unordered_map>
#include "ECS/Util/prototype_container.h"
#include "ECS/Prototypes/Prototype_Category.h"
#include "PrototypeInspector.h"
#include "../Util/RenameUtil.h"
namespace Tempest
{
	class PrototypeExplorer : public Window
	{
		const char* window_name() override
		{
			return ICON_FA_COLUMNS " Prototypes";
		}

		void init(Instance& ) override
		{
			window_flags |= ImGuiWindowFlags_MenuBar;
		}

		void show(Instance& instance) override
		{
			auto dir = instance.get_full_path() / "Prototypes";
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu(ICON_FA_PLUS "Add Prototype"))
					{
						for (auto& [name, proto] : get_prototype_categories())
						{
							if (ImGui::MenuItem(name.c_str()))
							{
								auto p = categories[name].emplace(proto.create());

								if (!p)
								{
									LOG_WARN("Failed to create {} prototype", name);
									continue;
								}

								// check if folder exist
								auto folder = dir / name;
								if (!fs::exists(folder))
									fs::create_directories(folder);

								// try save
								p->save(folder);
							}
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
				// check if prototype folder exists


				if (fs::exists(dir))
				{
					for (auto entry : fs::directory_iterator(dir))
					{
						if (!fs::is_directory(entry))
							continue;
						// each of this are folders

						if (ImGui::BeginTabBar("##categorytab"))
						{
							// each folder is a category
							draw_category(entry.path());
							ImGui::EndTabBar();
						}

					}
				}

			}
			ImGui::End();

			inspector.show(dir);
		}

		void draw_category(tpath path)
		{
			auto cat = path.filename().string();
			// assume path exists

			if(ImGui::BeginTabItem(cat.c_str()))
			{
				for (auto entry : fs::directory_iterator(path))
				{
					if (entry.path().extension() != ".json")
						continue;
					// each of this json files
					auto name = entry.path().stem().string();

					// check if loaded
					if (!categories[cat].has(name))
					{
						// try to load
						try
						{
							categories[cat].load_file(entry.path());
						}
						catch (const std::exception&)
						{
							// do nothing if we can't
							// we can clean if we want as well
							return;
						}
					}

					// display
					auto ptr = categories[cat].get_if(name);
					LOG_ASSERT(name.c_str());

					ImGui::PushID(ptr);
					ImGui::BeginGroup();
					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(static_cast<uint64_t>(0)),
						{ icon_size, icon_size },
						ImVec2(0, 1), ImVec2(1, 0), 2
					))
						/*if(ImGui::Button(name.c_str()))*/
					{
						inspector.select(categories[cat].get_if(name), cat);
					}

					rename_util.show(name, cat + name, icon_size, [](const string& new_name) {
						LOG(new_name);
					});

					//ImGui::Text(name.c_str());
					ImGui::EndGroup();
					ImGui::PopID();

					if (ImGui::BeginPopupContextItem(name.c_str()))
					{
						/*if (ImGui::MenuItem(std::string(ICON_FA_TRASH).append(" Delete").c_str()))
						{
							m_ScenePath = scene.GetPath().string();

							if (project.IsStartScene(scene.GetName()))
								m_OpenInvalidDeletePopup = true;
							else
								m_OpenDeleteScenePopup = true;
						}*/

						if (ImGui::MenuItem(ICON_FA_EDIT " Rename"))
						{
							rename_util.rename(name, cat + name);
						}

						ImGui::EndPopup();

					}

					ImGui::SameLine();
				}

				// if the file is gone, delete
				categories[cat].erase_if([&](const string& s) {
					return !fs::exists(path / (s + ".json"));
					});

				ImGui::EndTabItem();
			}

			


		}

	private:
		tmap<string, prototype_container> categories;
		PrototypeInspector inspector;
		RenameUtil rename_util;

		float icon_size = 50.f;
	};
}
