#include "AssetManagerWindow.h"

namespace Tempest
{
    void AssetManagerWindow::show(Instance& instance)
    {
        if (ImGui::Begin(window_name(), &visible, window_flags))
        {
			assets_winsize = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			if (ImGui::BeginChild("AssetsList", ImVec2(assets_winsize.x * 0.3f, assets_winsize.y), true))
			{
				std::string name = "Folder";
				name += "Assets";

				auto search = tree_contents.find(name);

				if (search == tree_contents.end())
					tree_contents[name] = name;

				// This is The topMost
				if (ImGui::TreeNode(name.c_str()))
				{
					asset_index = "Assets";

					ImGui::SameLine();

					if (ImGui::Button("Refresh"))
					{
					//	Unload();
					//	Init();
						//IMGUI_LOG_MSG("Assets folder refreshed and updated.");
					}
					ImGui::TreePop();
				}
				else
				{
					file_index.clear();
					asset_index.clear();
				}

				if (ImGui::Button("Fold All"))
				{
					for (auto const& [key, val] : tree_contents)
					{
						ImGuiID id = ImGui::GetID(val.c_str());
						if (id != 0)
							ImGui::GetStateStorage()->SetInt(id, 0);
					}
				}
			}
			ImGui::EndChild();
			ImGui::SameLine();

			if (ImGui::BeginChild("AssetsContent", ImVec2(assets_winsize.x * 0.4f, assets_winsize.y), true))
			{
				if (ImGui::BeginChild("AssetsDirectory", ImVec2(assets_winsize.x * 0.38f, assets_winsize.y * 0.1f), true))
				{
					ImGui::Text(asset_index.c_str());
				}

				ImGui::EndChild();
				ImGui::Separator();

				//DisplayFiles(_AssetIndex);
			}
			ImGui::EndChild();

			ImGui::SameLine();

			if (ImGui::BeginChild("Preview", ImVec2(assets_winsize.x * 0.28f, assets_winsize.y), true))
			{
				ImVec2 previewWindowSize = { assets_winsize.x * 0.25f, assets_winsize.y * 0.88f };
				ImGui::Text("Preview:");
				//PreviewFiles(_FileIndex, previewWindowSize);
			}
			ImGui::EndChild();

			ImGui::PopStyleVar();
        }
        ImGui::End();
    }
}

//#include <filesystem>
//#include <string>
//
//namespace UI
//{
//	void AssetManagerWindow::Init()
//	{
//
//	}
//
//	void AssetManagerWindow::Show()
//	{
//		/// <Testing >
//		std::vector<std::string> test;
//		for (int i = 0; i < 5; i++)
//		{
//			std::string temp = "Sample";
//			temp += std::to_string(i);
//			test.push_back(temp);
//		}
//		/// </Testing>		
//
//		if (m_toggle)
//		{
//			if (ImGui::Begin(m_Name.c_str(), nullptr, m_WindowFlags))
//			{
//				m_AssetsWinSize = ImGui::GetContentRegionAvail();
//
//				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
//				if (ImGui::BeginChild("AssetsList", ImVec2(m_AssetsWinSize.x * 0.3f, m_AssetsWinSize.y), true))
//				{
//					std::string name = "Folder";
//					name += "Assets";
//
//					auto search = _TreeContents.find(name);
//
//					if (search == _TreeContents.end())
//						_TreeContents[name] = name;
//
//					// This is The topMost
//					if (ImGui::TreeNode(name.c_str()))
//					{
//						_AssetIndex = "Assets";
//
//						ImGui::SameLine();
//
//						if (ImGui::Button("Refresh"))
//						{
//							Unload();
//							Init();
//							//IMGUI_LOG_MSG("Assets folder refreshed and updated.");
//						}
//
//						////////////////Testing Part
//						/*for (auto& t : test)
//						{
//							if (ImGui::TreeNode(t.c_str()))
//							{
//								if (ImGui::TreeNode("AssetInfo"))
//								{
//									for (auto& t : test)
//									{
//										if (ImGui::TreeNode(t.c_str()))
//										{
//											ImGui::TreePop();
//										}
//									}
//									ImGui::TreePop();
//								}
//								ImGui::TreePop();
//							}
//						}*/
//
//						////////////////////////
//						/*for (const auto& str : _FolderNames)
//						{
//							name = "Folder";
//							name += str;
//
//							search = _TreeContents.find(name);
//
//							if (search == _TreeContents.end())
//								_TreeContents[name] = name;
//
//							if (ImGui::TreeNode(name.c_str()))
//							{
//								_AssetIndex = str;
//
//								
//
//								for (const auto& subobj : _assetsInfo)
//								{
//									if (subobj._folder == str && subobj._file.empty() && !subobj._subFolder.empty())
//									{
//										name = "Folder";
//										name += subobj._subFolder;
//
//										search = _TreeContents.find(name);
//
//										if (search == _TreeContents.end())
//											_TreeContents[name] = name;
//
//										if (ImGui::TreeNode(name.c_str()))
//										{
//											_AssetIndex = subobj._subFolder;
//											ImGui::TreePop();
//										}
//									}
//								}
//								ImGui::TreePop();
//							}
//						}*/
//						ImGui::TreePop();
//					}
//					else
//					{
//						_FileIndex.clear();
//						_AssetIndex.clear();
//					}
//
//					if (ImGui::Button("Fold All"))
//					{
//						for (auto const& [key, val] : _TreeContents)
//						{
//							ImGuiID id = ImGui::GetID(val.c_str());
//							if (id != 0)
//								ImGui::GetStateStorage()->SetInt(id, 0);
//						}
//					}
//				}
//				ImGui::EndChild();
//				ImGui::SameLine();
//
//				if (ImGui::BeginChild("AssetsContent", ImVec2(m_AssetsWinSize.x * 0.4f, m_AssetsWinSize.y), true))
//				{
//					if (ImGui::BeginChild("AssetsDirectory", ImVec2(m_AssetsWinSize.x * 0.38f, m_AssetsWinSize.y * 0.1f), true))
//					{
//						ImGui::Text(_AssetIndex.c_str());
//					}
//
//					ImGui::EndChild();
//					ImGui::Separator();
//
//					//DisplayFiles(_AssetIndex);
//				}
//				ImGui::EndChild();
//				
//				ImGui::SameLine();
//
//				if (ImGui::BeginChild("Preview", ImVec2(m_AssetsWinSize.x * 0.28f, m_AssetsWinSize.y), true))
//				{
//					ImVec2 previewWindowSize = { m_AssetsWinSize.x * 0.25f, m_AssetsWinSize.y * 0.88f };
//					ImGui::Text("Preview:");
//					//PreviewFiles(_FileIndex, previewWindowSize);
//				}
//				ImGui::EndChild();
//
//				ImGui::PopStyleVar();
//			}
//			ImGui::End();
//		}
//	}
//
//	void AssetManagerWindow::Shutdown()
//	{
//	}
//
//	void AssetManagerWindow::Unload()
//	{
//	}
//
//	
//}
//


