#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Actions/EditorAction.h"

namespace Tempest
{
	class PrefabList : public Window
	{

		const char* window_name() override
		{
			return ICON_FA_SHAPES " Prefabs";
		}

		void show(Instance& instance) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				// filter
				{
					filter.Draw("##hierarchy_filter");
					ImGui::SameLine();
					UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Filter usage:\n"
						"  \"\"         display all lines\n"
						"  \"xxx\"      display lines containing \"xxx\"\n"
						"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
						"  \"-xxx\"     hide lines containing \"xxx\"", false);

				}

				{
					ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
					ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.f, 5.f));
					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(.5f, .5f));
					if (ImGui::BeginTable("HierarchyTable", 5, flags, { 0.f, ImGui::GetContentRegionAvail().y }))
					{
						ImGui::TableSetupScrollFreeze(0, 1);
						ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Prototype", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("Actions ", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableHeadersRow();


						std::vector<Entity> destroyed_list;
						for (auto& [id, pf] : instance.scene.get_map())
						{
							if (!filter.PassFilter(pf.get_prototype().c_str()) && !filter.PassFilter(pf.get_category().c_str())) continue;

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);

							auto pos = pf.get<tc::Transform>().position;
							//auto check = instance.scene.get_map().find((int)pos.x, (int)pos.z);

							auto c = UI::CreateLabelWithId("", id);
							if (ImGui::Selectable(c.c_str(), id == instance.selected,
								ImGuiSelectableFlags_SpanAllColumns |
								ImGuiSelectableFlags_AllowItemOverlap))
							{
								if (instance.selected == id)
									instance.selected = INVALID;
								else
									instance.selected = id;
							}

							ImGui::TableSetColumnIndex(1);
							ImGui::Selectable(std::to_string(id).c_str(), false, ImGuiSelectableFlags_Disabled);
							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%s", pf.get_prototype().c_str());
							ImGui::TableSetColumnIndex(3);
							ImGui::Text("%s", pf.get_category().c_str());
							ImGui::TableSetColumnIndex(4);
							if (ImGui::SmallButton((std::string(ICON_FA_TIMES_CIRCLE) + "##destroyme" + std::to_string(id)).c_str()))
							{
								destroyed_list.emplace_back(id);
							}
						}

						for (auto id : destroyed_list)
						{
							instance.action_history.Commit<DeletePrefab>(instance.scene.get_map().extract(id));
						}

						ImGui::EndTable();
					}
					ImGui::PopStyleVar(2);
				}
			}
			ImGui::End();
		}

		ImGuiTextFilter filter;
	};
}