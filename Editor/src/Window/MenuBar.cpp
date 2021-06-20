
#include "MenuBar.h"
#include "imgui/imgui.h"
#include "WindowManager.h"

namespace UI
{
	void MenuBar::Show()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 5.f));

		if (ImGui::BeginMainMenuBar())
		{
			ShowProject();
			ShowEdit();
			ShowWindow();
			ShowHelp();
			GameLoopButton();
			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();

		if (m_ShowInvalidFilePopup)
		{
			ShowInvalidFilePopup();
		}
	}

	void MenuBar::UpdateWindowItems(std::string windowName, bool* toggle, bool* lock)
	{
		m_WindowItems.insert({ std::move(windowName), toggle });
		m_LockWindow = lock;
	}

	void MenuBar::ShowProject()
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Save Project", "CTRL+S"))
			{
			}

			if (ImGui::MenuItem("Load Project"))
			{

			}

			ImGui::Separator();

			ImGui::MenuItem("Export Game", nullptr, false, false);
			ImGui::Separator();

			ImGui::MenuItem("Exit", nullptr, false, false);
			ImGui::EndMenu();
		}
	}

	void MenuBar::ShowEdit()
	{
		if (ImGui::BeginMenu("Edit"))
		{
			const size_t BUFFERSIZE = 32;
			char buf[BUFFERSIZE];

			ImGui::Dummy(ImVec2(0, 0.25));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 0.25));

			//Cardinal::System::Service<Utilities::Selector>::Get().IsAnySelected()
			sprintf_s(buf, BUFFERSIZE, "%s %s", "Copy", "Copy");
			if (ImGui::MenuItem(buf, "   Ctrl+C", nullptr, false))
			{

			}
			//sprintf_s(buf, BUFFERSIZE, "%s %s", ICON_FA_PASTE, "Paste");
			//if (ImGui::MenuItem(buf, "   Ctrl+V", nullptr, !Cardinal::System::Service<Clipboard>::Get().IsEmpty()))
			//{
			//	//ActionHistory::Commit in Cardinal::System::Service<UI::Clipboard>::Get().Paste()
			//	Cardinal::System::Service<Clipboard>::Get().Paste();
			//}
			sprintf_s(buf, BUFFERSIZE, "%s %s", "Duplicate", "Duplicate");
			if (ImGui::MenuItem(buf, "   Ctrl+D", nullptr, false))
			{

				//Cardinal::System::Service<Clipboard>::Get().Paste();

			}
			sprintf_s(buf, BUFFERSIZE, "%s %s", "Delete", "Delete");
			if (ImGui::MenuItem(buf, "   Del", nullptr, false))
			{

			}

			ImGui::EndMenu();
		}
	}

	void MenuBar::ShowWindow()
	{
		if (ImGui::BeginMenu("Window"))
		{
			for (const auto& [name, show] : m_WindowItems)
			{
				ImGui::MenuItem(name.c_str(), nullptr, show);
			}

			ImGui::Separator();
			ImGui::MenuItem("Lock Windows", nullptr, m_LockWindow);

			ImGui::EndMenu();
		}
	}

	void MenuBar::ShowHelp()
	{
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Show ImGui Demo", nullptr, &m_ShowDemo);
			ImGui::MenuItem("Show ImPlot Demo", nullptr, &m_ShowImPlotDemo);
			ImGui::EndMenu();
		}

		if (m_ShowDemo) ImGui::ShowDemoWindow();
	}

	void MenuBar::GameLoopButton()
	{
	//	auto& project = Cardinal::System::Service<Cardinal::System::ProjectManager>::Get().GetProject();
	//	auto& WM = Cardinal::System::Service<WindowManager>::Get();
	//	//auto& BM = Cardinal::System::Service<NodeEditor::BlueprintManager>::Get();

	//	const auto width = ImGui::GetWindowContentRegionWidth();
	//	ImGui::Dummy({ width / 3.5f, 0 });
	//	ImGui::SameLine();
	//	auto& gsm = Cardinal::System::Service<Cardinal::GameStateManager>::Get();
	//	if (gsm.GetCurrentState() == Cardinal::GameStateManager::State::STOPPED)
	//	{
	//		m_state = ButtonState::STOP;
	//	}
	//	switch (m_state)
	//	{
	//	case ButtonState::PLAY:
	//		ImGui::SameLine();
	//		ImGui::TextDisabled("Play");
	//		ImGui::SameLine();
	//		if (ImGui::Button(ICON_FA_PAUSE, { 25.f, 25.f }))
	//		{
	//			m_state = UI::ButtonState::PAUSE;
	//			gsm.Pause();
	//		}
	//		ImGui::SameLine();
	//		if (ImGui::Button(ICON_FA_STOP, { 25.f, 25.f }))
	//		{
	//			//project.LoadScene(project.GetCurrentScene().GetName());
	//			//project.;
	//			WM.ShowAllWindow();
	//			m_state = UI::ButtonState::STOP;
	//			gsm.Stop();
	//		}
	//		break;

	//	case ButtonState::PAUSE:
	//		if (ImGui::Button(ICON_FA_PLAY, { 25.f, 25.f }))
	//		{
	//			m_state = UI::ButtonState::PLAY;
	//			gsm.Play();
	//		}
	//		ImGui::SameLine();
	//		ImGui::TextDisabled(ICON_FA_PAUSE);
	//		ImGui::SameLine();
	//		if (ImGui::Button(ICON_FA_STOP, { 25.f, 25.f }))
	//		{
	//			//project.LoadScene(project.GetCurrentScene().GetName());
	//			WM.ShowAllWindow();
	//			m_state = UI::ButtonState::STOP;
	//			gsm.Stop();
	//		}
	//		break;

	//	default:
	//	case ButtonState::STOP:
	//		ImGui::SameLine();
	//		if (ImGui::Button(ICON_FA_PLAY, { 25.f, 25.f }))
	//		{
	//			Cardinal::System::Service<Utilities::Selector<Cardinal::System::EntityPool>>::Get().ClearSelected();
	//			Cardinal::System::Service<Utilities::Selector<Cardinal::PrefabManager>>::Get().ClearSelected();
	//			m_state = UI::ButtonState::PLAY;

	//			//BM.Save();
	//			//project.Save();
	//			//project.LoadScene(project.GetCurrentScene().GetName());
	//			project.SaveCurrentScene();
	//			//WM.ShowOnlyViewport();
	//			gsm.Play();
	//		}
	//		ImGui::SameLine();
	//		ImGui::TextDisabled(ICON_FA_PAUSE);
	//		ImGui::SameLine();
	//		ImGui::TextDisabled(ICON_FA_STOP);
	//		break;
	//	}
	}

	void MenuBar::SaveEntityState()
	{
		//auto& project = Cardinal::System::Service<Cardinal::System::ProjectManager>::Get().GetProject();
	}

	void MenuBar::ShowInvalidFilePopup()
	{
		ImGui::OpenPopup("Invalid File");

		if (ImGui::BeginPopupModal("Invalid File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Invalid File Type Loaded!");

			ImGui::Columns(3, nullptr, false);

			ImGui::NextColumn();

			if (ImGui::Button("Return"))
			{
				m_ShowInvalidFilePopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::NextColumn();
			ImGui::NextColumn();

			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}
	}
}
