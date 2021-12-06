#include "BuildModeOverlay.h"
#include "ECS/Prototypes/Prototype_Category.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Triggers/SimulationTriggers.h"
#include "Events/Events/ScriptEvents.h"
#include <Editor/src/Actions/EditorAction.h>

namespace Tempest
{
	void BuildModeOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
	}

	void BuildModeOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		//ImGui::SetNextWindowPos(viewport->Pos);
		//ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowPos({ viewport->WorkPos.x,viewport->WorkPos.y + viewport->Size.y * 0.7f });
		ImGui::SetNextWindowSize({ viewport->WorkSize.x ,viewport->Size.y * 0.3f });

		if (OverlayOpen)
		{
			
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

			if (ImGui::Begin("Build Mode Screen", nullptr, window_flags))
			{
				auto pos = ImGui::GetWindowPos();
				//ImGui::SetCursorPosY(ImGui::GetWindowPos().y + ImGui::GetWindowHeight() * 0.3f);
				ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.2f });
				ImGui::Dummy({ 10.f, 0.f });
				ImGui::SameLine();
				ImGui::BeginChild("PrototypeDisplay", { ImGui::GetWindowWidth() - 40.f, ImGui::GetWindowHeight() * 0.7f }, true);
				for (auto& [cat_name, proto_cat] : instance.scene.get_prototype_categories())
				{
					if (ImGui::BeginTabBar("##categorytab"))
					{
						// each folder is a category
						draw_category(instance, cat_name, proto_cat);
						ImGui::EndTabBar();
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();

			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
		}
	}


	void BuildModeOverlay::draw_category(Instance& instance, const string& cat_name, prototype_container& proto_cat)
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
					[]() {}, [&]()
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

					if (ImGui::BeginPopupContextItem(proto_name.c_str()))
					{
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
}