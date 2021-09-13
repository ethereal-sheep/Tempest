#include "UnitSheetOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	void UnitSheetOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;

		auto a = event_cast<OpenUnitSheetTrigger>(e);
		if (a.addUnit)
		{
			IsUnitCreation = true;
			Title = "Unit Creation";
		}
		else
		{
			IsUnitCreation = false;
			Title = "Editing Unit";
		}
	}

	void UnitSheetOverlay::show(Instance& instance)
	{	
		tc::Character* cs = nullptr;

		if(IsUnitCreation)
			cs = &NewCharacter;
		else
			cs = instance.ecs.get_if<tc::Character>(instance.selected);
		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		const ImVec4 GrabCol = { 117.f / 255.f,117.f / 255.f,117.f / 255.f,1.f };
		const ImVec4 HoverCol = { 99.f / 255.f,99.f / 255.f,99.f / 255.f,1.f };
		const ImVec4 ActiveCol = { 65.f / 255.f,65.f / 255.f,65.f / 255.f,1.f };
		

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		
		if (OverlayOpen)
		{
		//	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		//	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, { 0.f,0.f,0.f,255.f });
		//	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, GrabCol);
		//	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, HoverCol);
		//	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ActiveCol);
			if (ImGui::Begin("Edit Unit Sheet", nullptr, window_flags))
			{
				float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;

				UI::Header(ImVec2{ center_x - 100.f,50 }, Title.c_str());

				ImGui::SameLine();
				ImGui::Dummy({ 0, 100.f });

				ImGui::Columns(4, "UnitCreation", true);
				ImGuiWindow* window = ImGui::GetCurrentWindow();
				ImGuiOldColumnFlags ColFlags = ImGuiOldColumnFlags_NoResize;
				ImGuiOldColumns* columns = window->DC.CurrentColumns;
				columns->Flags = ColFlags;
				//auto texture = Service<RenderSystem>::Get().tex->GetID();
				//GLuint tex_id = static_cast<GLuint>(texture);
				//ImGui::Image((void*)static_cast<size_t>(tex_id), ImVec2(ImGui::GetColumnWidth(0), ImGui::GetColumnWidth(0)));
				/*==================================================================
									CHARACTER INFOMATION COLUMN
				==================================================================*/
				ImGui::NextColumn();
				string text = "Character Infomation";
				float FirstColWidth = ImGui::GetColumnWidth(1) / 2;
				float font_size = ImGui::GetFontSize() * text.size() / 2;
				float offsetX = FirstColWidth - font_size + (font_size / 2);
				float frontPadding = 5.f;


				ImGui::Dummy({ offsetX, 0 });
				ImGui::SameLine();
				ImGui::Text(text.c_str());
				ImGui::Dummy({ 0, 10.f });
				ImGui::BeginChild("##CharacterInfo", ImVec2(ImGui::GetColumnWidth(1) - 10.f, viewport->Size.y * 0.5f));
				//Name
				ImGui::PushFont(FONT_BODY);
				ImGui::Dummy({ frontPadding, 0 });
				ImGui::SameLine();
				ImGui::Text("Name");

				ImGui::Dummy({ frontPadding, 0 });
				ImGui::SameLine();
				ImGui::InputText("##CharacterName", &cs->name);

				ImGui::Dummy({ 0, 10.f });

				for (auto i = 0; i < sl->size(); i++)
				{
					string stat = sl->operator[](i) + " :";
					string label = "##" + stat;
					//auto data = std::to_string(cs->get_stat(i));
					string WeaponData = "";
					if (cs->weapon != UNDEFINED)
					{
						auto weap = instance.ecs.get_if<tc::Weapon>(cs->weapon);

						if (weap->get_stat(i) > 0)
						{
							string data = std::to_string(weap->get_stat(i));
							WeaponData = "( +" + data + " )";
						}
						else if (weap->get_stat(i) < 0)
						{
							string data = std::to_string(weap->get_stat(i));
							WeaponData = "( " + data + " )";
						}

					}


					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					ImGui::Text(stat.c_str());
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::Text(WeaponData.c_str());
					ImGui::Dummy({ 0, 10.f });

				}
				ImGui::PopFont();
				ImGui::EndChild();

				if (IsUnitCreation && UI::UIButton_1("Add Stat", "Add Stat", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
				{
					AddStatPopup = true;
					NewStatName = "Stat";
					NewStatValue = 0;
					LOG("ADDING STAT");
				}

				if (AddStatPopup)
				{
					ImGui::OpenPopup("Add Stat");
					ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::SetNextWindowSize(ImVec2(500, 300));
					ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoScrollbar;
					if (ImGui::BeginPopupModal("Add Stat", NULL, flags))
					{
						float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
						string sub = "Add Stat";
						font_size = ImGui::GetFontSize() * sub.size() / 2;
						float center = RegionWidth - font_size;


						UI::SubHeader({ center - font_size , 0 }, sub.c_str());
						ImGui::PushFont(FONT_PARA);
						ImGui::Dummy({0, 50.f });
						
						ImGui::Dummy({ ImGui::GetContentRegionAvailWidth() * 0.25f, 50.f });
						ImGui::SameLine();
						ImGui::Text("Stat Name");
						ImGui::SameLine();
						ImGui::Dummy({ 60.f - ImGui::GetItemRectSize().x, 0 });
						ImGui::SameLine();
						ImGui::PushItemWidth(150.0f);
						ImGui::InputText("##AddStatName", &NewStatName);
						ImGui::PopItemWidth();
						
						
						ImGui::Dummy({ 0, frontPadding });

						ImGui::Dummy({ ImGui::GetContentRegionAvailWidth() * 0.25f, 50.f });
						ImGui::SameLine();
						ImGui::Text("Stat Value");
						ImGui::SameLine();
						ImGui::Dummy({ 60.f - ImGui::GetItemRectSize().x, 0 });
						ImGui::SameLine();
						ImGui::PushItemWidth(150.0f);
						ImGui::InputInt("##AddStatValue", &NewStatValue, 0);
						ImGui::PopItemWidth();
						ImGui::PopFont();

						/*if (UI::UIButton_1("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
						{
							sl->add_stat(NewStatName);
							cs->get_stat(sl->index_of_stat(NewStatName)) = NewStatValue;
							ImGui::CloseCurrentPopup();
							AddStatPopup = false;
							NewStatName = "Stat";
							NewStatValue = 0;
						}*/

						if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
						{
							ImGui::CloseCurrentPopup();

							NewStatName = "Stat";
							NewStatValue = 0;
							AddStatPopup = false;
						}

					}

					ImGui::EndPopup();
				}

				/*==================================================================
									WEAPON AND ITEMS COLUMN
				==================================================================*/
				ImGui::NextColumn();
				string WeapText = "Weapons and Items";
				float SecondColWidth = ImGui::GetColumnWidth(2) / 2;
				font_size = ImGui::GetFontSize() * WeapText.size() / 2;
				float SecondoffsetX = SecondColWidth - font_size + (font_size / 2);

				ImGui::Dummy({ SecondoffsetX, 0 });
				ImGui::SameLine();
				ImGui::Text(WeapText.c_str());
				ImGui::Dummy({ 0, 10.f });
				ImGui::BeginChild("##WeapAndItem", ImVec2(ImGui::GetColumnWidth(2) - 10.f, viewport->Size.y * 0.5f));

				ImGui::Dummy({ frontPadding + 20.0f, 0 });
				ImGui::SameLine();
				ImGui::Text("Equipped");

				if (cs->weapon != UNDEFINED)
				{
					auto& weap = instance.ecs.get<tc::Weapon>(cs->weapon);
					if(UI::UIButton_1(weap.name.c_str(), weap.name.c_str(), { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f - 80.0f, ImGui::GetCursorPosY() + 30.0f }, { 100.f, 10.f }, FONT_PARA))
					{
						EditWeaponPopup = true;
						EditWeap = weap;
					}
					if(EditWeaponPopup)
					{
						ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoScrollbar;
						ImGui::OpenPopup("Edit Weapon");
						ImGui::SetNextWindowSize(ImVec2(800, 300));
						ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
						if (ImGui::BeginPopupModal("Edit Weapon", NULL, flags))
						{
							float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
							string Editsub = "Edit Weapon/Item";
							font_size = ImGui::GetFontSize() * Editsub.size() / 2;
							float center = RegionWidth - font_size;
							UI::SubHeader({ center - 70.f, 0 }, Editsub.c_str());

							ImGui::Dummy({ 0, 20.f });
							ImGui::Columns(2, "Edit Weapon", true);
							window = ImGui::GetCurrentWindow();
							columns = window->DC.CurrentColumns;
							columns->Flags = ColFlags;
							ImGui::SetColumnWidth(0, 332.f);

							ImGui::NextColumn();
							ImGui::PushFont(FONT_PARA);
							//static tc::Weapon newWeap;
							float Padding_x = 60.f;
							ImGui::Text("Name");
							ImGui::SameLine();
							ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
							ImGui::SameLine();
							ImGui::InputText("##EditWeapName", &EditWeap.name);
							bool disabled = EditWeap.name.size() > 9;
							ImGui::SameLine();
							if(disabled)
							{
								ImGui::Text("9 Char only");
							}
							else
							{
								ImGui::Dummy({ 100.f, 10.f });
							}
							ImGui::Dummy({ 0, 10.f });
							ImGui::BeginChild("##EditWeapStats", { ImGui::GetColumnWidth(1) - 10.f, 100.f });
							for (auto i = 0; i < sl->size(); i++)
							{
								ImGui::Text(sl->operator[](i).c_str());
								ImGui::SameLine();
								ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
								ImGui::SameLine();
								ImGui::SetNextItemWidth(80.f);
								string WeapStats = "##WeapStats" + std::to_string(i);
								ImGui::InputInt(WeapStats.c_str(), &EditWeap.get_stat(i), 0);

								if (i % 2 == 0)
								{
									ImGui::SameLine();
									ImGui::Dummy({ 10.f, 0 });
									ImGui::SameLine();
								}
							}

							ImGui::EndChild();
							ImGui::PopFont();
							ImGui::EndColumns();
							if(disabled)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								//ImGui::PushDisabled();
							}
							if (UI::UIButton_1("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
							{
								ImGui::CloseCurrentPopup();
								weap = EditWeap;
								EditWeaponPopup = false;
							}
							if (disabled)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
								//ImGui::PopDisabled();
							}
							if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
							{
								ImGui::CloseCurrentPopup();
								EditWeap = tc::Weapon();
								EditWeaponPopup = false;
							}
							ImGui::EndPopup();
						}
					}
				}
				ImGui::EndChild();
				if (UI::UIButton_1("Add Weapon/Item", "Add Weapon/Item", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
				{
					AddWeaponPopup = true;
					LOG("ADDING WEAPON");
				}
				/*==================================================================
											ADD WEAPON POPUP
				==================================================================*/
				if (AddWeaponPopup)
				{
					ImGui::OpenPopup("Add Weapon");
					ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::SetNextWindowSize(ImVec2(500, 550));
					ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
						ImGuiWindowFlags_NoScrollbar;
					if (ImGui::BeginPopupModal("Add Weapon", NULL, flags))
					{
						float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
						string sub = "Add Weapon";
						font_size = ImGui::GetFontSize() * sub.size() / 2;
						float center = RegionWidth - font_size;
						if (cs->weapon != UNDEFINED)
						{

						}
						

						UI::SubHeader({ center - 100.f, 0 }, "Add Weapon");
						ImGui::Dummy({ 0, 20.f });
						ImGui::Columns(2, "Weapons", true);
						window = ImGui::GetCurrentWindow();

						columns = window->DC.CurrentColumns;
						columns->Flags = ColFlags;

						/*==================================================================
											ADD WEAPON COLUMN
						==================================================================*/
						ImGui::BeginChild("##weap", ImVec2(ImGui::GetColumnWidth(0) - 10.f, 300.0f));
						auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());
						float index = 1;

						const ImVec2 button_pos{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.1f };
						for (auto id : view)
						{
							auto& weap = instance.ecs.get<Components::Weapon>(id);
							bool isSelected = false;
							if (CurSelection == id)
								isSelected = true;

							if (UI::UIButton_1(weap.name.c_str(), weap.name.c_str(), { button_pos.x, button_pos.y + (index - 1) * 60 }, { 100.f, 10.f }, FONT_PARA, isSelected))
							{
								CurSelection = id;
								//LOG("CURRENT SELECTED: {0}", CurSelection);
							}
							index++;
						}
						ImGui::EndChild();
						/*==================================================================
											Create WEAPON COLUMN
						==================================================================*/
						ImGui::NextColumn();
						
						//ImGuiSelectableFlags flags = ImGuiSelectableFlags_
						ImGui::PushFont(FONT_BODY);
						float CreateItmCol = ImGui::GetColumnWidth(1) / 2;
						string CreateItmStr = "Create New Item";
						font_size = ImGui::GetFontSize() * CreateItmStr.size() / 2;
						float CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
						ImGui::Dummy({ CreateItmCenter, 0 });
						ImGui::SameLine();
						if (UI::UISelectable(CreateItmStr.c_str()))
						{
							CreateOpen = true;
						}
						ImGui::Dummy({ 0, 10.f });

						string CreateWeapStr = "Create New Weapon";
						font_size = ImGui::GetFontSize() * CreateWeapStr.size() / 2;
						CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
						ImGui::Dummy({ CreateItmCenter, 0 });
						ImGui::SameLine();

						if (UI::UISelectable(CreateWeapStr.c_str()))
						{
							CreateOpen = true;
						}

						/*==================================================================
											Create New WEAPON POPUP
						==================================================================*/
						if (CreateOpen)
						{
							ImGui::OpenPopup("Add New Weapon");
							ImGui::SetNextWindowSize(ImVec2(800, 300));
							ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
							if (ImGui::BeginPopupModal("Add New Weapon", NULL, flags))
							{
								RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
								sub = "New Weapon/Item";
								font_size = ImGui::GetFontSize() * sub.size() / 2;
								center = RegionWidth - font_size;
								UI::SubHeader({ center - 70.f, 0 }, sub.c_str());

								ImGui::Dummy({ 0, 20.f });

								ImGui::Columns(2, "Add New Weapon", true);
								window = ImGui::GetCurrentWindow();
								columns = window->DC.CurrentColumns;
								columns->Flags = ColFlags;
								ImGui::SetColumnWidth(0, 332.f);

								ImGui::NextColumn();
								ImGui::PushFont(FONT_PARA);
								float Padding_x = 60.f;
								ImGui::Text("Name");
								ImGui::SameLine();
								ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
								ImGui::SameLine();
								ImGui::InputText("##NewWeapName", &NewWeap.name);
								bool disabled = NewWeap.name.size() > 9;
								ImGui::SameLine();
								if(disabled)
								{
									ImGui::Text("9 Char only");
								}
								else
								{
									ImGui::Dummy({ 100.f, 10.f });
								}
								ImGui::Dummy({ 0, 10.f });
								ImGui::BeginChild("##NewWeapStats", { ImGui::GetColumnWidth(1) - 10.f, 100.f }, true);
								for (auto i = 0; i < sl->size(); i++)
								{
									ImGui::Text(sl->operator[](i).c_str());
									ImGui::SameLine();
									ImGui::Dummy({ Padding_x - ImGui::GetItemRectSize().x ,0.f });
									ImGui::SameLine();
									ImGui::SetNextItemWidth(80.f);
									string WeapStats = "##WeapStats" + std::to_string(i);
									ImGui::InputInt(WeapStats.c_str(), &NewWeap.get_stat(i), 0);

									if (i % 2 == 0)
									{
										ImGui::SameLine();
										ImGui::Dummy({ 10.f, 0 });
										ImGui::SameLine();
									}
								}

								ImGui::EndChild();
								ImGui::PopFont();
								ImGui::EndColumns();
								if (disabled)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								if (UI::UIButton_1("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.6f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
								{
									auto entity = instance.ecs.create();
									auto meta = instance.ecs.emplace<tc::Meta>(entity);
									meta->name = NewWeap.name;
									auto weapon = instance.ecs.emplace<tc::Weapon>(entity);
									weapon->name = NewWeap.name;
									for (auto i = 0; i < sl->size(); i++)
									{
										weapon->set_stat(i, NewWeap.get_stat(i));
									}
									ImGui::CloseCurrentPopup();
									CreateOpen = false;
								}
								if (disabled)
								{
									 ImGui::PopItemFlag();
									 ImGui::PopStyleVar();
								}
								if (UI::UIButton_1("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.8f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 80.f, 0.f }, FONT_PARA))
								{
									NewWeap = tc::Weapon();
									ImGui::CloseCurrentPopup();
									CreateOpen = false;
								}
								ImGui::EndPopup();
							}
						}
						ImGui::Dummy({ 0, 10.f });

						string DeleteWeapStr = "Delete Weapon";
						font_size = ImGui::GetFontSize() * DeleteWeapStr.size() / 2;
						CreateItmCenter = CreateItmCol - font_size + (font_size / 2);
						ImGui::Dummy({ CreateItmCenter, 0 });
						ImGui::SameLine();
						if (UI::UISelectable(DeleteWeapStr.c_str()))
						{
							if (cs->weapon == CurSelection)
							{
								cs->weapon = UNDEFINED;
							}
							instance.ecs.emplace<tc::Destroyed>(CurSelection);
							// remove rb from scene
							if (auto rb = instance.ecs.get_if<tc::Rigidbody>(CurSelection))
							{
								instance.po.RemoveActorFromScene(rb->internal_rb.get());
							}
							CurSelection = UNDEFINED;

							ImGui::CloseCurrentPopup();
							AddWeaponPopup = false;
						}
						ImGui::PopFont();
						ImGui::EndColumns();
						ImVec2 ConfirmPos = { 195, 400.f };
						if (UI::UIButton_1("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.8f }, { 50.f, 0.f }, FONT_PARA, false))
						{
							cs->weapon = CurSelection;
							ImGui::CloseCurrentPopup();
							AddWeaponPopup = false;
						}

						ImGui::EndPopup();
					}
				}

				/*==================================================================
										SKILL COLUMN
				==================================================================*/
				ImGui::NextColumn();
				string SkillText = "Skill";
				font_size = ImGui::GetFontSize() * SkillText.size() / 2;
				float ThirdColWidth = ImGui::GetColumnWidth(2) / 2;
				float ThirdoffsetX = ThirdColWidth - font_size + (font_size / 2);

				ImGui::Dummy({ ThirdoffsetX, 0 });
				ImGui::SameLine();
				ImGui::Text(SkillText.c_str());
				ImGui::Dummy({ 0, 10.f });
				ImGui::BeginChild("##SKILL", ImVec2(ImGui::GetColumnWidth(2) - 10.f, viewport->Size.y * 0.5f));


				ImGui::EndChild();
				if(UI::UIButton_1("Add Skill", "Add Skill", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.3f }, { 50.f, 10.f }, FONT_PARA))
				{

				}
				ImGui::EndColumns();

				if (UI::UIButton_1("Save", "Save", { ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 100.0f ,ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 50.0f }, { 90.f, 0.f }, FONT_PARA))
				{
					LOG("SAVED");
					if(IsUnitCreation)
					{
						auto entity = instance.ecs.create();
						auto meta = instance.ecs.emplace<tc::Meta>(entity);
						meta->name = cs->name;
						auto character = instance.ecs.emplace<tc::Character>(entity);
						character->name = cs->name;
						for (auto i = 0; i < sl->size(); i++)
						{
							character->set_stat(i, cs->get_stat(i));
						}
					}
					OverlayOpen = false;
				}
				
			}
			
			ImGui::End();
			
		//	ImGui::PopStyleVar(3);
		//	ImGui::PopStyleColor(4);
			
		}
		
		
	}
}