
#include "WeaponSheetOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Tempest/src/Graphics/Basics/RenderSystem.h"

namespace Tempest
{
	// pass in instance 
	void WeaponSheetOverlay::open_popup(const Event& e)
	{
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

		WeaponInfo = tex_map["Assets/WeaponInfoTabUnlit.png"];
		UseInfo = tex_map["Assets/UseInfoTabUnlit.png"];
		OverlayOpen = true;

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

			}

			ImGui::PopStyleVar();
			ImGui::End();
		}

	}
}