
#include "WeaponSheetOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	// pass in instance 
	void WeaponSheetOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		Tabs[TABS_TYPE::WEAPON].current_state = TabImageData::STATE::UNHOVER;
		Tabs[TABS_TYPE::USE].current_state = TabImageData::STATE::UNHOVER;

		auto a = event_cast<OpenWeaponSheetTrigger>(e);

		if (a.addUnit)
		{
			// TODO: shift this to a function
			tc::Weapon* weap = &NewWeapon;
			auto entity = a.instance.ecs.create();
			auto meta = a.instance.ecs.emplace<tc::Meta>(entity);
			meta->name = weap->name;
			auto character = a.instance.ecs.emplace<tc::Weapon>(entity);
			character->name = weap->name;

			for (auto i = 0; i < tc::STAT_TOTAL; i++)
				character->set_stat(i, weap->get_stat(i));

			NewWeapon = tc::Weapon();
			SelectedID = entity;
		}

		// get the first weapon
		else
		{
			auto view = a.instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());
			for (auto id : view)
			{
				SelectedID = id;
				break;
			}
		}

	}

	void WeaponSheetOverlay::show(Instance& instance)
	{
		tc::Weapon* weap = nullptr;
		weap = instance.ecs.get_if<tc::Weapon>(SelectedID);

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		if (OverlayOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Edit Weapon Sheet", nullptr, window_flags))
			{
				auto tex = tex_map["Assets/WeaponsBG.png"];

				ImVec2 point{ 0,0 };
				{
					ImVec2 Min{ point.x, point.y };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Editing Weapon");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// Display the created units
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.03f, viewport->Size.y * 0.15f });
				ImGui::BeginChild("##WeaponsDisplay", { viewport->Size.x * 0.15f, viewport->Size.y * 0.65f }, true);

				{
					unsigned i = 0;
					auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());

					// TODO: store selected item
					const ImVec2 cursor{ ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() + 60 };
					for (auto id : view)
					{
						auto& weapon = instance.ecs.get<tc::Weapon>(id);
						if (UI::UIButton_1(weapon.name.c_str(), weapon.name.c_str(), { cursor.x , cursor.y + i++ * 100 }, { 120,10 }, FONT_PARA))
						{
							weap = &weapon;
						}


						if (UI::UIButton_1("+", "+", { cursor.x , cursor.y + i * 100 }, { 160,10 }, FONT_PARA))
						{
							// add new weapon
						}
					}
				}

				ImGui::EndChild();

				// display weapon picture here

				// display information
				// tabs 
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.35f, viewport->Size.y * 0.15f });

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

					render_tabs(TABS_TYPE::WEAPON, [&]() {
						WeaponSheetOverlay::display_weapon_stats(*viewport, instance, *weap);
					});

					render_tabs(TABS_TYPE::USE, [&]() {
						WeaponSheetOverlay::display_use_stats(*viewport, instance, *weap);
					});

					ImGui::PopStyleColor(3);
				}

				if (UI::UIButton_2("Back", "Back", { viewport->Size.x * 0.1f, viewport->Size.y * 0.05f }, { 0,0 }, FONT_PARA))
				{
					OverlayOpen = false;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::PopStyleVar();
			ImGui::End();
		}
	}

	void WeaponSheetOverlay::initialise_tabs()
	{
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponInfoTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/WeaponInfoTabLit.png"]->GetID());
		Tabs[TABS_TYPE::WEAPON].size = ImVec2{ static_cast<float>(tex_map["Assets/WeaponInfoTabUnlit.png"]->GetWidth()),
											 static_cast<float>(tex_map["Assets/WeaponInfoTabUnlit.png"]->GetHeight()) };

		Tabs[TABS_TYPE::USE].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/UseInfoTabUnlit.png"]->GetID());
		Tabs[TABS_TYPE::USE].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/UseInfoTabLit.png"]->GetID());
		Tabs[TABS_TYPE::USE].size = ImVec2{ static_cast<float>(tex_map["Assets/UseInfoTabUnlit.png"]->GetWidth()),
											   static_cast<float>(tex_map["Assets/UseInfoTabUnlit.png"]->GetHeight()) };
	}

	template<typename F>
	void WeaponSheetOverlay::render_tabs(TABS_TYPE type, F&& func)
	{
		ImVec2 prev_cursor_pos{ 0.f,0.f };

		if (ImGui::ImageButton(Tabs[type].image_id[Tabs[type].current_state], Tabs[type].size))
		{
			Tabs[CurrentTab].current_state = TabImageData::STATE::UNHOVER;
			Tabs[CurrentTab].is_active = false;
			Tabs[type].is_active = true;

			CurrentTab = type;
		}

		if (ImGui::IsItemHovered())
			Tabs[type].current_state = TabImageData::STATE::HOVER;
		else
			Tabs[type].current_state = TabImageData::STATE::UNHOVER;

		if (type != TABS_TYPE::USE)
		{
			ImGui::SameLine();
			prev_cursor_pos = ImGui::GetCursorPos();
		}

		if (Tabs[type].is_active)
			func();

		// Get ready for next render of tab
		ImGui::SetCursorPos(prev_cursor_pos);
	}

	void WeaponSheetOverlay::display_weapon_stats(const ImGuiViewport& viewport, Instance& instance, tc::Weapon& weap) const
	{
		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		ImGui::SetCursorPos(ImVec2{ viewport.Size.x * 0.35f, viewport.Size.y * 0.25f });
		ImGui::BeginChild("##WeaponsInformationDisplay", { viewport.Size.x * 0.6f, viewport.Size.y * 0.55f }, true);

		// display the weapon info
		float frontPadding = 5.f;
		ImGui::PushFont(FONT_BODY);
		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::InputText("##WeaponName", &weap.name);
		bool NameDisabled = weap.name.size() > 15;
		ImGui::SameLine();
		if (NameDisabled)
		{
			ImGui::Text("15 Char only");
		}
		else
		{
			ImGui::Dummy({ 100.f, 10.f });
		}
		ImGui::Dummy({ 0, 10.f });

		for (auto i = 0; i < sl->size(); i++)
		{
			if ((*sl)(i))
			{
				string stat = sl->operator[](i) + " :";
				string label = "##" + stat;
				string WeaponData = std::to_string(weap.get_stat(i));

				ImGui::Dummy({ frontPadding, 0 });
				ImGui::SameLine();
				ImGui::Text(stat.c_str());
				ImGui::Dummy({ frontPadding, 0 });
				ImGui::SameLine();
				ImGui::PushItemWidth(100.f);
				ImGui::InputInt(label.c_str(), &weap.get_stat(i), 0);
				ImGui::PopItemWidth();
				ImGui::Dummy({ 0, 10.f });
			}

		}
		ImGui::PopFont();

		ImGui::EndChild();
	}

	void WeaponSheetOverlay::display_use_stats(const ImGuiViewport& viewport, Instance& instance, tc::Weapon& weap) const
	{

	}
}