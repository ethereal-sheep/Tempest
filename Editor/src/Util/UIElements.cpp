/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "UIElements.h"
#include "Audio/AudioEngine.h"


namespace Tempest::UI
{

	void PaddedSeparator(float padding)
	{
		ImGui::Dummy({ 0.f, padding });
		ImGui::Separator();
		ImGui::Dummy({ 0.f, padding });
	}
	void PaddedSeparator(float up, float down)
	{
		ImGui::Dummy({ 0.f, up });
		ImGui::Separator();
		ImGui::Dummy({ 0.f, down });
	}

	void HelpMarker(const char* str)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(str);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Tooltip(const char* label, const char* str, bool enabled)
	{
		if (enabled)
			ImGui::Text(label);
		else
			ImGui::TextDisabled(label);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(str);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void DrawLine()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsItemActive())
			ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos,
				ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		// Draw a line between the button and the mouse cursor
	}

	bool ConfirmDeletePopup(const char* popupName, string str)
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(600, 300));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
		ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		if (ImGui::BeginPopupModal(popupName, nullptr, flags))
		{
			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };

			if (ImGui::IsWindowFocused() == false)
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}
			auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
			auto warnImg = tex_map["Assets/WarningIco.dds"];
			string te = "CONFIRMATION";
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
			, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
			ImGui::PopFont();


			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.2f });
			ImGui::PushFont(FONT_SHEAD);
			auto windowWidth = ImGui::GetWindowSize().x;
			string warningstr ="WARNING!";
			auto warningSize = ImGui::CalcTextSize(warningstr.c_str()).x;
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.792f,0.22f,0.22f,1.f });
			ImGui::SetCursorPosX((windowWidth - warningSize) * 0.5f - ((float)warnImg->GetWidth() * 0.7f));
			ImGui::Image((void*)static_cast<size_t>(warnImg->GetID()), { (float)warnImg->GetWidth() * 0.7f, (float)warnImg->GetHeight() * 0.7f });
			ImGui::SameLine();
			ImGui::Text(warningstr.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();

			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.1f });
			ImGui::PushFont(FONT_BODY);
			auto strSize = ImGui::CalcTextSize(str.c_str()).x;
			ImGui::SetCursorPosX((windowWidth - strSize) * 0.5f);
			ImGui::Text(str.c_str());
			ImGui::PopFont();

			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.355f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.645f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return false;
			}
			ImGui::EndPopup();
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor();
		return false;
	}
	bool ConfirmDeletePopup_DefineStat(const char* popupName)
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(600, 300));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

		ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		if (ImGui::BeginPopupModal(popupName, nullptr, flags))
		{
			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };
			if (ImGui::IsWindowFocused() == false)
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}

			auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
			auto warnImg = tex_map["Assets/WarningIco.dds"];
			string te = "CONFIRMATION";
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
			//ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
			, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
			ImGui::PopFont();



			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.2f });
			ImGui::PushFont(FONT_SHEAD);
			auto windowWidth = ImGui::GetWindowSize().x;
			string warningstr = /*string(ICON_FA_EXCLAMATION_TRIANGLE) +*/ " WARNING!";
			auto warningSize = ImGui::CalcTextSize(warningstr.c_str()).x;
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.792f,0.22f,0.22f,1.f });
			ImGui::SetCursorPosX((windowWidth - warningSize) * 0.5f - ((float)warnImg->GetWidth() * 0.7f));
			ImGui::Image((void*)static_cast<size_t>(warnImg->GetID()), { (float)warnImg->GetWidth() * 0.7f, (float)warnImg->GetHeight() * 0.7f });
			ImGui::SameLine();
			ImGui::Text(warningstr.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();

			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.1f });
			ImGui::PushFont(FONT_BODY);
			string str1 = "Unchecking this stat will remove it from all unit profiles";
			string str2 = "and action graph!";
			auto str1Size = ImGui::CalcTextSize(str1.c_str()).x;
			auto str2Size = ImGui::CalcTextSize(str2.c_str()).x;
			ImGui::SetCursorPosX((windowWidth - str1Size) * 0.5f);
			ImGui::Text(str1.c_str());
			ImGui::SetCursorPosX((windowWidth - str2Size) * 0.5f);
			ImGui::Text(str2.c_str());
			ImGui::PopFont();

			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.355f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.645f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return false;
			}
			ImGui::EndPopup();
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor();
		return false;
	}

	bool ConfirmInputNamePopup(const char* popupName, string &str)
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(600, 300));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
		ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		if (ImGui::BeginPopupModal(popupName, nullptr, flags))
		{
			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };

			if (ImGui::IsWindowFocused() == false)
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}
			auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
			string te = popupName;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
			, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
			ImGui::PopFont();

			ImGui::PushFont(FONT_BODY);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Name").x) * 0.25f);
			ImGui::SetCursorPosY((ImGui::GetWindowSize().y - ImGui::CalcTextSize("Name").y) * 0.5f);
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
			ImGui::SameLine();
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("##InputNewMapName", &str);
			ImGui::PopItemWidth();
			ImGui::PopFont();

			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.355f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Cancel", "Cancel", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.645f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return false;
			}
			ImGui::EndPopup();
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor();
		return false;
	}

	bool ConfirmTutorialPopup(const char* popupName, string str, bool isExit, std::function<void(void)> cancel_function)
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(600, 300));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
		ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		if (ImGui::BeginPopupModal(popupName, nullptr, flags))
		{
			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };

			if (ImGui::IsWindowFocused() == false)
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}
			auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
			string te = "CONFIRMATION";
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
			, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
			ImGui::PopFont();

			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.4f });
			ImGui::PushFont(FONT_BODY);
			auto strSize = ImGui::CalcTextSize(str.c_str()).x;
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - strSize) * 0.5f);
			ImGui::Text(str.c_str());
			ImGui::PopFont();

			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2(isExit ? "Confirm" : "Yes", isExit ? "Confirm" : "Yes", {ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.355f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f}, {-30.f, 0.f}, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2(isExit ? "Cancel" : "No", isExit ? "Cancel" : "No", {ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.645f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f}, {-30.f, 0.f}, FONT_PARA))
			{
				ImGui::PopStyleVar(3);
				ImGui::PopStyleColor();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				cancel_function();
				return false;
			}
			ImGui::EndPopup();
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor();
		return false;
	}

	void ShowLabel(const char* label, ImColor color)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
		const ImVec2 size = ImGui::CalcTextSize(label);

		const ImVec2& padding = ImGui::GetStyle().FramePadding;
		const ImVec2& spacing = ImGui::GetStyle().ItemSpacing;

		const ImVec2& cursorPos = ImGui::GetCursorPos();
		const ImVec2& cursorScreenPos = ImGui::GetCursorScreenPos();

		ImGui::SetCursorPos(ImVec2(cursorPos.x + spacing.x, cursorPos.y - spacing.y));

		const ImVec2 rectMin = ImVec2(cursorScreenPos.x - padding.x, cursorScreenPos.y - padding.y);
		const ImVec2 rectMax = ImVec2(cursorScreenPos.x + size.x + padding.x, cursorScreenPos.y + size.y + padding.y);

		auto* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
		ImGui::TextUnformatted(label);
	}

	bool Selectable(const char* label, const char* ID, bool selected, ImGuiSelectableFlags flags, ImVec2 size)
	{
		const unsigned BUFFER_SIZE = 64;
		char buffer[BUFFER_SIZE];
		sprintf_s(buffer, BUFFER_SIZE, "%s##%s", label, ID);
		return ImGui::Selectable(buffer, selected, flags, size);
	}

	void RenderText(const std::string& label, float padding)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::Dummy(ImVec2{ padding - ImGui::GetItemRectSize().x, 0.f });
		ImGui::SameLine();
	}
	std::pair<bool, bool> DragFloat3(const char* str, const char* ID, ImVec2 padding, float v[3], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat3(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat2(const char* str, const char* ID, ImVec2 padding, float v[2], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat2(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat(const char* str, const char* ID, ImVec2 padding, float* val, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(ID, val, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt(const char* label, const char* ID, ImVec2 padding, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragInt(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt2(const char* label, const char* ID, ImVec2 padding, int v[2], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= valueChangeOnEdit = ImGui::DragInt2(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt3(const char* label, const char* ID, ImVec2 padding, int v[3], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragInt3(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat3ColorBox(const char* str, const char* ID, ImVec2 padding, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Setting Text to the left
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();

		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 38.f);
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		ImGui::IsItemDeactivatedAfterEdit();
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Y ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Z ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button(std::string("Z").append(ID).c_str(), buttonSize))
		{
			v[2] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Z").c_str(), &v[2], v_speed, v_min, v_max, format, flags);
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat2ColorBox(const char* label, const char* ID, ImVec2 padding, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Setting Text to the left
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();


		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================

		// Setting Y ======================================================
		ImGui::SameLine();
		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat3ColorBox_NoText(const char* ID, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 37.f);
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Y ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Z ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button(std::string("Z").append(ID).c_str(), buttonSize))
		{
			v[2] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Z").c_str(), &v[2], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> DragFloat2ColorBox_NoText(const char* ID, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================

		// Setting Y ======================================================
		ImGui::SameLine();
		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> UniformScaleFloat3(const char* label, const char* ID, ImVec2 padding, bool* b, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::Text(label);
		ImGui::SameLine(padding.x - 26.f);
		ImGui::Text(ICON_FA_LINK);
		ImGui::SameLine();
		ImGui::Checkbox(ID, b);
		ImGui::SameLine();

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;

		if (*b)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
			vec3 temp = { v[0], v[1], v[2] };
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 37.f);
			//ImGui::PushItemWidth((ImGui::CalcItemWidth() / 2 - ImGui::GetStyle().ItemInnerSpacing.x));

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// Setting X ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::Button("X", buttonSize);
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("first").c_str(), ImGuiDataType_Float, &v[0], v_speed, &v_min, &v_max, format, flags);
			bool first = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x + 1.f);

			// Setting Y ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				v[1] = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			//bool second = ImGui::DragScalar("##secondFloatId", ImGuiDataType_Float, &val.y, 1.f, &v_min, NULL, "%.3f");
			ImGui::DragScalar(std::string(ID).append("second").c_str(), ImGuiDataType_Float, &v[1], v_speed, &v_min, &v_max, format, flags);
			bool second = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x + 1.f);

			// Setting Z ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize))
				v[2] = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("third").c_str(), ImGuiDataType_Float, &v[2], v_speed, &v_min, &v_max, format, flags);
			bool third = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================

			ImGui::Dummy({ 0,5 });
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();

			if (first)
			{
				float ratio = v[0] / temp.x;
				v[1] *= ratio;
				v[2] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			else if (second)
			{
				float ratio = v[1] / temp.y;
				v[0] *= ratio;
				v[2] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			else if (third)
			{
				float ratio = v[2] / temp.z;
				v[0] *= ratio;
				v[1] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			return { valueChangeOnEdit, valueChangeOnRelease };
		}
		else
		{
			return DragFloat3ColorBox_NoText(ID, v, resetValue, v_speed, v_min, v_max, format, flags);
		}
	}

	void UniformScaleFloat2(const char* label, const char* ID, ImVec2 padding, bool* b, float v[2], float resetValue, float v_speed,
		float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Text(ICON_FA_LINK);
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Checkbox("##uniform_scale", b);
		ImGui::PopStyleVar();
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();

		if (*b)
		{
			//float min = 1.f;
			float ratio = v[0] / v[1];
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
			//ImGui::PushItemWidth((ImGui::CalcItemWidth() / 2 - ImGui::GetStyle().ItemInnerSpacing.x));

			ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// Setting X ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize))
				v[0] = 1.f / ratio;
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("first").c_str(), ImGuiDataType_Float, &v[0], v_speed, &v_min, &v_max, format, flags);
			bool first = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);

			// Setting Y ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				v[1] = 1.f / ratio;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			//bool second = ImGui::DragScalar("##secondFloatId", ImGuiDataType_Float, &val.y, 1.f, &v_min, NULL, "%.3f");
			ImGui::DragScalar(std::string(ID).append("second").c_str(), ImGuiDataType_Float, &v[1], v_speed, &v_min, &v_max, format, flags);
			bool second = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::Dummy({ 0,5 });
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();

			if (first)
			{
				v[1] = v[0] / ratio;
			}
			else if (second)
			{
				v[0] = v[1] * ratio;
			}

		}
		else
		{
			DragFloat2ColorBox_NoText(ID, v, resetValue, v_speed, v_min, v_max, format, flags);
		}
		ImGui::PopStyleVar();
	}

	std::pair<bool, bool> SliderInt(const char* label, const char* ID, ImVec2 padding, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderInt2(const char* label, const char* ID, ImVec2 padding, int v[2], int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt2(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderInt3(const char* label, const char* ID, ImVec2 padding, int v[3], int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt3(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat(const char* label, const char* ID, ImVec2 padding, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat2(const char* label, const char* ID, ImVec2 padding, float v[2], float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat2(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat3(const char* label, const char* ID, ImVec2 padding, float v[3], float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat3(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> InputFloat(const char* label, const char* ID, ImVec2 padding, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::InputFloat(ID, v, step, step_fast, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> ColorEdit4(const char* label, const char* ID, ImVec2 padding, float col[4], ImGuiColorEditFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::ColorEdit4(ID, col, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> ColorEdit3(const char* label, const char* ID, ImVec2 padding, float col[3], ImGuiColorEditFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::ColorEdit3(ID, col, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	bool Checkbox(const char* str, const char* ID, ImVec2 padding, bool* val)
	{
		bool pressed = false;
		ImGui::PushID(str);
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		pressed |= ImGui::Checkbox(ID, val);
		ImGui::PopID();
		return pressed;
	}

	bool InputText(const char* label, const char* ID, ImVec2 padding, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		bool pressed = false;
		ImGui::PushID(ID);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		pressed |= ImGui::InputText(ID, str, flags, callback, user_data);
		ImGui::PopID();
		return pressed;
	}
	void SubHeader(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/SubHeaderStyle.dds"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;


		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - textWidth - (curr_tex->GetWidth() * 2)) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SameLine();
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 1.f,1.f }, { 0.f,0.f });
		ImGui::EndGroup();
		ImGui::PopFont();


	}
	void Header_1(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/HeaderStyle1.dds"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;


		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(str);
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 0.f,1.f }, { 1.f, 0.f });
		ImGui::EndGroup();
		ImGui::PopFont();
	}

	void Header_2(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/HeaderStyle2.dds"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;
		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(str);
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 0.f,1.f }, { 1.f,0.f });
		ImGui::EndGroup();
		ImGui::PopFont();
	}

	bool UIButton_1(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
		const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };

		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(hover.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		ImGui::SetCursorPos(new_pos);
		bool hovered = ImGui::IsItemHovered();
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImGui::SetCursorPos(text_pos);
			/*ImGui::SetCursorPos(
				{
					pos.x + button_size.x / 2.f - text_size.x / 2.f,
					pos.y + 2.f
				});*/
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		return false;
	}

	//Default Button color
	bool UIButton_2(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
		const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		string str = "aaaaaaaaaaaaaaa000000";
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		return false;
	}

	bool UIButton_3(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.f, 0.f, 0.f, 1.f };
		//const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 hovered_border_col = { 0.f, 0.f, 0.f, 1.f };
		const ImVec4 button_bg_col = { 0.f, 0.f, 0.f, 0.f };
		string str = "aaaaaaaaaaaaaaa000000";
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.1f - test.x * 0.1f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f};

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::Text(hover.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::Text(unselected.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::Text(hover.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
				return true;
			}
		}
		return false;
	}

	//Blueish Button for Select Weapon Btn
	bool UIButton_Weapon(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, [[maybe_unused]] bool selected)
	{
		const float default_padding_x = 0.f;
		const float default_padding_y = 0.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.305f, 0.612f, 0.717f, 1.f };
		const ImVec4 hovered_border_col = { 0.443f, 0.690f, 0.775f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		auto tex = tex_map["Assets/WeaponBtn.dds"];
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		//padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		/*ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);*/

		ImVec2 text_size = { (float)tex->GetWidth()*0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 test = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 alt_text_size = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};
		ImVec2 ResetPos = ImGui::GetCursorPos();
		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

		
		

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		bool res = false;

		ImGui::SetCursorPos(new_pos);
		if (id != UNDEFINED)
		{	
			auto weapon = instance.ecs.get_if<tc::Weapon>(id);
			ImVec4 col = default_border_col;
			if (hovered)
				col = hovered_border_col;
			
			test = ImGui::CalcTextSize(weapon->name.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

			ImGui::SetCursorPos(new_pos);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, col);
			ImGui::PushStyleColor(ImGuiCol_Button, col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
			
			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);

			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.3f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1.f });
			ImGui::PushFont(font);
			if (weapon)
			{
				ImGui::Text(weapon->name.c_str());
			}
			else
			{
				ImGui::Text("NAN");
			}
				
			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::GetIO().MouseClicked[0])
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, default_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);


			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, hovered_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::GetIO().MouseClicked[0])
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		ImGui::SetCursorPos(ResetPos);
		return res;
	}

	//Greenish Button for Select Action Btn
	bool UIButton_Action(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, [[maybe_unused]] bool selected)
	{
		const float default_padding_x = 0.f;
		const float default_padding_y = 0.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.627f, 0.333f, 0.859f, 1.f };
		const ImVec4 hovered_border_col = { 0.701f, 0.466f, 0.886f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		auto tex = tex_map["Assets/ActionBtn.dds"];
		float rounding = 0.f;

		// button shit
		ImGui::PushFont(font);

		ImVec2 text_size = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 test = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 alt_text_size = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};
		ImVec2 ResetPos = ImGui::GetCursorPos();
		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
		ImVec2 imgMax = new_pos + text_size;
		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		bool res = false;
		
		ImGui::SetCursorPos(new_pos);
		if (id != UNDEFINED)
		{
			auto action = instance.ecs.get_if<tc::Graph>(id);
			ImVec4 col = default_border_col;
			if (hovered)
				col = hovered_border_col;

			if(action)
				test = ImGui::CalcTextSize(action->g.name.c_str(), nullptr, true);
			else
				test = ImGui::CalcTextSize("NAN", nullptr, true);
			
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

			ImGui::SetCursorPos(new_pos);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, col);
			ImGui::PushStyleColor(ImGuiCol_Button, col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);

			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), new_pos, imgMax);
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.3f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1.f });
			ImGui::PushFont(font);
			if (action)
			{
				ImGui::Text(action->g.name.c_str());
			}
			else
			{
				ImGui::Text("NAN");
			}

			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, default_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, hovered_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		ImGui::SetCursorPos(ResetPos);
		return res;
	}

	// Beige Button for Select Sequence Btn
	bool UIButton_Sequence(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, [[maybe_unused]] bool selected)
	{
		const float default_padding_x = 0.f;
		const float default_padding_y = 0.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.98f, 0.769f, 0.509f, 1.f };
		const ImVec4 hovered_border_col = { 0.984f, 0.816f, 0.608f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		auto tex = tex_map["Assets/SequenceBtn.dds"];
		float rounding = 0.f;
		// button shit
		ImGui::PushFont(font);


		ImVec2 text_size = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 test = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 alt_text_size = { (float)tex->GetWidth() * 0.7f, (float)tex->GetHeight() * 0.7f };
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
		ImVec2 imgMax = new_pos + text_size;
		ImVec2 ResetPos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		bool res = false;



		ImGui::SetCursorPos(new_pos);
		if (id != UNDEFINED)
		{
			auto action = instance.ecs.get_if<tc::Graph>(id);
			ImVec4 col = default_border_col;
			if (hovered)
				col = hovered_border_col;

			if (action)
				test = ImGui::CalcTextSize(action->g.name.c_str(), nullptr, true);
			else
				test = ImGui::CalcTextSize("NAN", nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

			ImGui::SetCursorPos(new_pos);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			
			ImGui::PushStyleColor(ImGuiCol_Border, col);
			ImGui::PushStyleColor(ImGuiCol_Button, col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), new_pos, imgMax);

			//Text
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.3f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1.f });
			ImGui::PushFont(font);
			if (action)
			{
				ImGui::Text(action->g.name.c_str());
			}
			else
			{
				ImGui::Text("NAN");
			}

			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, default_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
			text_pos = { new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };
			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, hovered_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				res = true;
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		ImGui::SetCursorPos(ResetPos);
		return res;
	}

	bool UIButton_Simulate(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.792f, 0.22f, 0.22f, 1.f };
		const ImVec4 hovered_border_col = { 0.835f, 0.376f, 0.376f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };

		string str = "aaaaaaaaaaaaaaa000000";
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		bool res = false;



		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			ImVec4 col = default_border_col;
			if (hovered)
				col = hovered_border_col;

			ImGui::SetCursorPos(new_pos);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);

			ImGui::PushStyleColor(ImGuiCol_Border, col);
			ImGui::PushStyleColor(ImGuiCol_Button, col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);

			//Text
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.3f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1.f });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				res = true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);


			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, default_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, hovered_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				res = true;
			}
		}
		if (selected)
		{
			auto SearchIcon = tex_map["Assets/Loading.dds"];
			ImVec2 iconMin = { pos.x,  pos.y };
			ImVec2 iconMax = { iconMin.x + SearchIcon->GetWidth(), iconMin.y + SearchIcon->GetHeight() };
			//ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(SearchIcon->GetID()), iconMin, iconMax);
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.5f, new_pos.y});
			ImGui::Image((void*)static_cast<size_t>(SearchIcon->GetID()), { (float)SearchIcon->GetWidth() * 0.75f, (float)SearchIcon->GetHeight() * 0.75f });
		}

		

		return res;
	}
	bool UIButton_CustomMap(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.217f, 0.545f, 0.784f, 1.f };
		const ImVec4 hovered_border_col = { 0.392f, 0.686f, 0.898f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };

		string str = "aaaaaaaaaaaaaaa000000";
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		bool res = false;



		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			ImVec4 col = default_border_col;
			if (hovered)
				col = hovered_border_col;
			auto SearchIcon = tex_map["Assets/CustomMapIcon.dds"];
			ImVec2 iconMin = { pos.x,  pos.y };
			ImVec2 iconMax = { iconMin.x + SearchIcon->GetWidth(), iconMin.y + SearchIcon->GetHeight() };

			ImGui::SetCursorPos(new_pos);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);

			ImGui::PushStyleColor(ImGuiCol_Border, col);
			ImGui::PushStyleColor(ImGuiCol_Button, col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
			
			//Drawing Image
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.47f, new_pos.y });
			ImGui::Image((void*)static_cast<size_t>(SearchIcon->GetID()), { (float)SearchIcon->GetWidth() * 0.75f, (float)SearchIcon->GetHeight() * 0.75f });

			//Text
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.35f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1.f });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				res = true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);


			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, default_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, hovered_border_col);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				res = true;
			}
		}
		//if (selected)
		//{
		//	auto SearchIcon = tex_map["Assets/CustomMapIcon.dds"];
		//	ImVec2 iconMin = { pos.x,  pos.y };
		//	ImVec2 iconMax = { iconMin.x + SearchIcon->GetWidth(), iconMin.y + SearchIcon->GetHeight() };
		//	//ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(SearchIcon->GetID()), iconMin, iconMax);
		//	ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.38f, new_pos.y - 1.f });
		//	ImGui::Image((void*)static_cast<size_t>(SearchIcon->GetID()), { (float)SearchIcon->GetWidth() * 0.75f, (float)SearchIcon->GetHeight() * 0.75f });
		//}



		return res;
	}
	bool UIButton_EndTurn(ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.98f, 0.769f, 0.51f, 1.f };
		const ImVec4 hovered_border_col = { 0.984f, 0.816f, 0.608f, 1.f };
		const ImVec4 button_bg_col = { 0.45f, 0.77f, 0.09f, 1.f };
		const ImVec4 button_active_col = { 0.53f, 0.89f, 0.11f, 1.f };

		string str = "aaaaaaaaaaaaaaa000000";
		string unselected = "End Turn";
		float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		//bool hovered = ImGui::IsItemHovered();
		bool res = false;



		ImGui::SetCursorPos(new_pos);
		if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);


			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.6f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDown(0))
			{
				ImGui::PushStyleColor(ImGuiCol_Border, button_active_col);
				ImGui::PushStyleColor(ImGuiCol_Button, button_active_col);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_active_col);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_col);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
				ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_col);		
			}
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.6f - test.x * 0.5f, text_pos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				res = true;

				AudioEngine ae;
				ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
			}
		}
		if (selected)
		{
			auto SearchIcon = tex_map["Assets/CheckMark.dds"];
			ImVec2 iconMin = { pos.x,  pos.y };
			ImVec2 iconMax = { iconMin.x + SearchIcon->GetWidth(), iconMin.y + SearchIcon->GetHeight() };
			//ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(SearchIcon->GetID()), iconMin, iconMax);
			ImGui::SetCursorPos({ new_pos.x + button_size.x * 0.05f, new_pos.y - 1.f });
			ImGui::Image((void*)static_cast<size_t>(SearchIcon->GetID()), { (float)SearchIcon->GetWidth() * 0.58f, (float)SearchIcon->GetHeight() * 0.58f });
		}



		return res;
	}

	std::pair<bool, bool> UIButtonWithDelete(string unselected, string id, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
		const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		string str = "aaaaaaaaaaaaaaa000000";
		static float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();




		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 10.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 10.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 10.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}

		return { false,false };
	}

	std::pair<bool, bool> UIWeapActionButtonWithDelete(string unselected, string id, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected, bool isWeapBtn, float size)
	{
		const float default_padding_x = 0.f;
		const float default_padding_y = 0.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.305f, 0.612f, 0.717f, 1.f };
		const ImVec4 hovered_border_col = { 0.443f, 0.690f, 0.775f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		auto tex = tex_map["Assets/WeaponBtn.dds"];
		if(isWeapBtn == false)
			tex = tex_map["Assets/ActionBtn.dds"];

		float rounding = 0.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = { (float)tex->GetWidth() * size, (float)tex->GetHeight() * size };
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = { (float)tex->GetWidth() * size, (float)tex->GetHeight() * size };
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.y * 0.2f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();

		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}

		return { false,false };
	}

	std::pair<bool, bool> UISeqButtonWithDelete(string unselected, string id, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected, float size)
	{
		const float default_padding_x = 0.f;
		const float default_padding_y = 0.f;
		const float border_size = 1.5f;

		const ImVec4 default_border_col = { 0.305f, 0.612f, 0.717f, 1.f };
		const ImVec4 hovered_border_col = { 0.443f, 0.690f, 0.775f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		auto tex = tex_map["Assets/SequenceBtn2.dds"];

		float rounding = 0.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = { (float)tex->GetWidth() * size, (float)tex->GetHeight() * size };
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = { (float)tex->GetWidth() * size, (float)tex->GetHeight() * size };
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.y * 0.2f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };


		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(7);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();

		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImVec2 imgMin = new_pos + ImGui::GetCurrentWindow()->Pos;
			ImVec2 imgMax = imgMin + text_size;
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), imgMin, imgMax);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
			ImGui::SetCursorPos({ new_pos.x + button_size.x - 15.f, new_pos.y - button_size.y * 0.5f + 15.f });
			if (ImGui::Button(string(ICON_FA_TRASH + id).c_str()))
			{
				AudioEngine ae;
				ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
				ImGui::PopStyleVar();
				return{ false, true };
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(7);
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos(text_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				return { true,false };
			}
		}

		return { false,false };
	}

	void AddUnderline(ImU32 col, ImVec2 min, ImVec2 max)
	{
		//ImVec2 min = ImGui::GetItemRectMin();
		//ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(
			min, max, col, 2.0f);
	}

	bool UISelectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 pos = window->DC.CursorPos;

		//Calculating the center position of the text
		float CreateItmCol = ImGui::GetContentRegionAvail().x * 0.5f;
		string CreateItmStr = label;
		float CreateItmCenter = CreateItmCol - label_size.x + (label_size.x * 0.5f);
		pos.x += CreateItmCenter;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ImGui::ItemSize(size, 0.0f);

		// Fill horizontal space
		// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
			size.x = ImMax(label_size.x, max_x - min_x);

		// Text stays at the submission position, but bounding box may be extended on both sides
		const ImVec2 text_min = pos;
		const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
			const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}
		//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		bool item_add;
		if (flags & ImGuiSelectableFlags_Disabled)
		{
			ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
			g.CurrentItemFlags |= ImGuiItemFlags_Disabled | ImGuiItemFlags_NoNavDefaultFocus;
			item_add = ImGui::ItemAdd(bb, id);
			g.CurrentItemFlags = backup_item_flags;
		}
		else
		{
			item_add = ImGui::ItemAdd(bb, id);
		}

		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add)
			return false;

		// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		// which would be advantageous since most selectable are not selected.
		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePushBackgroundChannel();

		// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_Disabled) { button_flags |= ImGuiButtonFlags_Disabled; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		if (flags & ImGuiSelectableFlags_Disabled)
			selected = false;

		const bool was_selected = selected;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);

		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect({ bb.Min.x - window->Pos.x, bb.Min.y - window->Pos.y }, { bb.Max.x - window->Pos.x, bb.Max.y - window->Pos.y }));
				g.NavDisableHighlight = true;
			}
		}
		if (pressed)
			ImGui::MarkItemEdited(id);

		if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			ImGui::SetItemAllowOverlap();

		// In this branch, Selectable() cannot toggle the selection so this will never trigger.
		if (selected != was_selected) //-V547
			window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render
		if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
			hovered = true;
		if (hovered || selected)
		{
			//Getting the texture and centering the position of the Image
			auto highlightImg = tex_map["Assets/MainMenuButton.dds"];
			const ImU32 col = ImGui::GetColorU32({ 0.980f, 0.768f, 0.509f, 1.f });
			float tex_width = highlightImg->GetWidth() * 0.5f;
			//float tex_height = highlightImg->GetHeight() * 0.5f;
			const ImVec2 hMin = { bb.Min.x - (tex_width * 0.5f) + (label_size.x * 0.5f), bb.Min.y };
			const ImVec2 hMax = { hMin.x + tex_width, bb.Max.y };

			//Adding of the Highlighted image when hover
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(highlightImg->GetID()), hMin, hMax);

			//Debugging Purposes
			//ImGui::RenderFrame(hMin, hMax, col, false, 0.0f);
		}

		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePopBackgroundChannel();

		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);

		ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();

		// Automatically close popups
		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.CurrentItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
			ImGui::CloseCurrentPopup();

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		return pressed;
	}
	bool UISelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (UISelectable(label, *p_selected, flags, size_arg))
		{
			*p_selected = !*p_selected;
			return true;
		}
		return false;
	}
	bool UICheckBox_1(const char* label, bool* v)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		auto curr_tex = tex_map["Assets/Unselected.dds"];
		const float square_sz = ImGui::GetFrameHeight();
		//const float square_sz = curr_tex->GetHeight();
		const ImVec2 pos = window->DC.CursorPos;
		ImVec2 sqSz = ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);
		const ImRect total_bb(pos, { pos.x + sqSz.x, pos.y + sqSz.y });
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
		{
			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
			return false;
		}

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
		{
			*v = !(*v);

			ImGui::MarkItemEdited(id);
		}

		const ImRect check_bb(pos, { pos.x + square_sz, pos.y + square_sz });
		ImGui::RenderNavHighlight(total_bb, id);

		if (!*v)
		{
			window->DrawList->AddImage((void*)static_cast<size_t>(curr_tex->GetID()), check_bb.Min, check_bb.Max);
			//ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(check_bb.GetWidth(), check_bb.GetWidth()));
		}

		ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);
		bool mixed_value = (g.CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0;
		if (mixed_value)
		{
			// Undocumented tristate/mixed/indeterminate checkbox (#2644)
			// This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
			ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
			window->DrawList->AddRectFilled({ check_bb.Min.x + pad.x, check_bb.Min.y + pad.y }, { check_bb.Max.x - pad.x, check_bb.Max.y - pad.y }, check_col, style.FrameRounding);
		}
		else if (*v)
		{
			const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
			curr_tex = tex_map["Assets/Selected.dds"];
			window->DrawList->AddImage((void*)static_cast<size_t>(curr_tex->GetID()), check_bb.Min, check_bb.Max);
			//ImGui::RenderCheckMark(window->DrawList, { check_bb.Min.x + pad, check_bb.Min.y + pad }, check_col, square_sz - pad * 2.0f);
		}

		ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
		if (g.LogEnabled)
			ImGui::LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
		if (label_size.x > 0.0f)
			ImGui::RenderText(label_pos, label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
		return pressed;
	}

	std::pair<bool, bool> UIConflictSelectable(const char* label, bool selected, int type, ImGuiSelectableFlags flags, [[maybe_unused]]const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return { false,false };

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		//Loading Of texture
		auto texIndex_Selection = tex_map["Assets/+_Unselected.dds"];
		auto texConflictBg = tex_map["Assets/ConflictUnSelected.dds"];
		switch (type)
		{
		case 1:
			texIndex_Selection = tex_map["Assets/01_Unselected.dds"];
			texConflictBg = tex_map["Assets/ConflictSelected.dds"];
		break;
		case 2:
			texIndex_Selection = tex_map["Assets/02_Unselected.dds"];
			texConflictBg = tex_map["Assets/ConflictSelected.dds"];
			break;
		case 3:
			texIndex_Selection = tex_map["Assets/03_Unselected.dds"];
			texConflictBg = tex_map["Assets/ConflictSelected.dds"];
			break;
		default:
			texIndex_Selection = tex_map["Assets/+_Unselected.dds"];
			texConflictBg = tex_map["Assets/ConflictUnSelected.dds"];
			break;
		}


		auto texConflictDel = tex_map["Assets/ConflictDelete.dds"];
		ImVec2 boxSize = { (float)texConflictBg->GetWidth()*0.7f, (float)texConflictBg->GetHeight() * 0.7f };


		// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		//ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 size(boxSize.x, boxSize.y);
		bool del = false;
		ImVec2 pos = window->DC.CursorPos;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ImGui::ItemSize(size, 0.0f);

		// Fill horizontal space
		// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		//if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
		//	size.x = ImMax(label_size.x, max_x - min_x);

		// Text stays at the submission position, but bounding box may be extended on both sides
		ImVec2 text_min = { pos.x + size.x * 0.2f, pos.y - size.y * 0.6f };
		ImVec2 text_max(min_x + size.x, pos.y + size.y);

		// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
			const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}
		//Drawing of Images to the selectable
		ImVec2 rectMin = { bb.Min.x + (size.x * 0.01f), bb.Min.y };
		ImVec2 rectMax = { bb.Min.x + (size.x * 0.17f), bb.Max.y };
		ImVec2 texMax = { rectMin.x + texIndex_Selection->GetWidth() * 0.7f ,rectMin.y + texIndex_Selection->GetHeight() * 0.7f };

		ImVec4 col = { 0.133f, 0.133f, 0.133f, 1.f };
		ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(texConflictBg->GetID()), bb.Min, bb.Max);


		//Creating Delete Button
		if (type != 0)
		{
			auto CurPos = ImGui::GetCursorPos();
			auto winPos = ImGui::GetWindowPos();
			ImGui::SetCursorPos({ bb.Min.x - winPos.x + boxSize.x * 0.85f ,bb.Min.y - winPos.y + boxSize.y * 0.6f });
			ImVec2 delSize = { (float)texConflictDel->GetWidth() * 0.7f,(float)texConflictDel->GetHeight() * 0.7f };
			//ImVec4 tinCol = { 0.7f, 0.7f, 0.7f,1 };
			ImVec4 tintHover = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 tintPressed = { 0.784f, 0.616f, 0.408f, 1.f };
			ImGui::PushID(label);
			if (UIImageButton((void*)static_cast<size_t>(texConflictDel->GetID()), delSize, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				del = true;
			}
			ImGui::PopID();
			ImGui::SetCursorPos(CurPos);
		}
		

		//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		bool item_add;
		if (flags & ImGuiSelectableFlags_Disabled)
		{
			ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
			g.CurrentItemFlags |= ImGuiItemFlags_Disabled | ImGuiItemFlags_NoNavDefaultFocus;
			item_add = ImGui::ItemAdd(bb, id);
			g.CurrentItemFlags = backup_item_flags;
		}
		else
		{
			item_add = ImGui::ItemAdd(bb, id);
		}

		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add)
			return { false,false };

		// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		// which would be advantageous since most selectable are not selected.
		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePushBackgroundChannel();

		// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_Disabled) { button_flags |= ImGuiButtonFlags_Disabled; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		if (flags & ImGuiSelectableFlags_Disabled)
			selected = false;

		const bool was_selected = selected;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);
		if (hovered || pressed || selected)
		{
			switch (type)
			{
			case 1:
				texIndex_Selection = tex_map["Assets/01_Selected.dds"];
				break;
			case 2:
				texIndex_Selection = tex_map["Assets/02_Selected.dds"];
				break;
			case 3:
				texIndex_Selection = tex_map["Assets/03_Selected.dds"];
				break;
			default:
				texIndex_Selection = tex_map["Assets/+_Selected.dds"];
				break;
			}
		}
		
		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect(bb.Min - window->Pos, bb.Max - window->Pos));
				g.NavDisableHighlight = true;
			}
		}
		if (pressed)
		{
			ImGui::MarkItemEdited(id);
			ImGui::SetMouseCursor(7);
		}
			

		if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			ImGui::SetItemAllowOverlap();

		// In this branch, Selectable() cannot toggle the selection so this will never trigger.
		if (selected != was_selected) //-V547
			window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render
		if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
			hovered = true;
		if (hovered || selected)
		{
			//[[maybe_unused]] const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
			//ImGui::RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
			//ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
			ImGui::SetMouseCursor(7);
		}

		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePopBackgroundChannel();

		ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, ImGui::GetColorU32(col));
		ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(texIndex_Selection->GetID()), rectMin, texMax);

		if (type != 0)
		{
			ImGui::PushFont(FONT_BTN);
			if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
			ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
			if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();
			ImGui::PopFont();
		}
		
		
		// Automatically close popups
		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.CurrentItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
			ImGui::CloseCurrentPopup();

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		return { pressed,del };
	}
	std::pair<bool, bool> UIConflictSelectable(const char* label, bool* p_selected, int type, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (UIConflictSelectable(label, *p_selected, type,flags, size_arg).first)
		{
			*p_selected = !*p_selected;
			return { true,false };
		}

		if (UIConflictSelectable(label, *p_selected, type, flags, size_arg).second)
		{
			return { false,true };
		}
		return { false,false };
	}

	std::pair<bool, bool> UIMapSelectable(const char* label, const char* date, bool selected,bool withDelete, [[maybe_unused]] int type, ImGuiSelectableFlags flags, [[maybe_unused]] const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return { false,false };

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		//Loading Of texture
		auto texMapBg = tex_map["Assets/MapSelectable.dds"];
		

		auto texConflictDel = tex_map["Assets/ConflictDelete.dds"];
		ImVec2 boxSize = { (float)texMapBg->GetWidth() * 0.83f, (float)texMapBg->GetHeight() * 0.83f };


		// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		//ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 size(boxSize.x, boxSize.y);
		bool del = false;
		ImVec2 pos = window->DC.CursorPos;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ImGui::ItemSize(size, 0.0f);

		// Fill horizontal space
		// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		//if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
		//	size.x = ImMax(label_size.x, max_x - min_x);

		// Text stays at the submission position, but bounding box may be extended on both sides
		ImVec2 text_min = { pos.x , pos.y - size.y * 0.9f };
		ImVec2 text_min2 = { pos.x , pos.y - size.y * 0.27f };
		ImVec2 text_max(min_x + size.x, pos.y + size.y);

		// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
			const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}
		//Drawing of Images to the selectable
		ImVec2 rectMin = { bb.Min.x + (size.x * 0.01f), bb.Min.y };
		ImVec2 rectMax = { bb.Min.x + (size.x * 0.17f), bb.Max.y };
		
		ImVec4 col = { 0.133f, 0.133f, 0.133f, 1.f };
		ImVec4 tintHover = { 0.980f, 0.768f, 0.509f, 1.f };
		ImVec4 tintPressed = { 0.784f, 0.616f, 0.408f, 1.f };
		ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(texMapBg->GetID()), bb.Min, bb.Max);

		if (withDelete)
		{
			//Creating Delete Button
			auto CurPos = ImGui::GetCursorPos();
			auto winPos = ImGui::GetWindowPos();
			//ImGui::SetCursorPos({ bb.Min.x - winPos.x + boxSize.x * 0.80f ,bb.Min.y - winPos.y + boxSize.y * 0.6f });


			ImGui::SetCursorPosX(bb.Min.x - winPos.x + boxSize.x * 0.80f);
			ImGui::SetCursorPosY(CurPos.y - 70.f);

			ImVec2 delSize = { (float)texConflictDel->GetWidth() * 0.7f,(float)texConflictDel->GetHeight() * 0.7f };
			//ImVec4 tinCol = { 0.7f, 0.7f, 0.7f,1 };

			ImGui::PushID(label);
			if (UIImageButton((void*)static_cast<size_t>(texConflictDel->GetID()), delSize, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, tintHover, tintPressed))
			{
				del = true;
			}
			ImGui::PopID();
			ImGui::SetCursorPos(CurPos);
		}
		
		


		//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		bool item_add;
		if (flags & ImGuiSelectableFlags_Disabled)
		{
			ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
			g.CurrentItemFlags |= ImGuiItemFlags_Disabled | ImGuiItemFlags_NoNavDefaultFocus;
			item_add = ImGui::ItemAdd(bb, id);
			g.CurrentItemFlags = backup_item_flags;
		}
		else
		{
			item_add = ImGui::ItemAdd(bb, id);
		}

		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add)
			return { false,false };

		// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		// which would be advantageous since most selectable are not selected.
		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePushBackgroundChannel();

		// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_Disabled) { button_flags |= ImGuiButtonFlags_Disabled; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		if (flags & ImGuiSelectableFlags_Disabled)
			selected = false;

		const bool was_selected = selected;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);
		

		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect(bb.Min - window->Pos, bb.Max - window->Pos));
				g.NavDisableHighlight = true;
			}
			ImGui::SetMouseCursor(7);
		}

		if (pressed)
			ImGui::MarkItemEdited(id);

		if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			ImGui::SetItemAllowOverlap();

		// In this branch, Selectable() cannot toggle the selection so this will never trigger.
		if (selected != was_selected) //-V547
			window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render
		if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
			hovered = true;
		if (hovered || selected)
		{
			//const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
			//ImGui::RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
			//ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
			ImGui::SetMouseCursor(7);
		}

		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePopBackgroundChannel();

		
		
		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
		ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
		ImGui::PushFont(FONT_TURN);
		ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
		ImGui::PopFont();
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
		ImGui::PushFont(FONT_PARA);
		ImGui::RenderTextClipped(text_min2, text_max, date, NULL, &label_size, style.SelectableTextAlign, &bb);
		ImGui::PopFont();
		ImGui::PopStyleColor();
		
		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();
		
		


		// Automatically close popups
		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.CurrentItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
			ImGui::CloseCurrentPopup();

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		return { pressed,del };
	}
	std::pair<bool, bool> UIMapSelectable(const char* label, const char* date, bool* p_selected, bool withDelete, int type, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (UIMapSelectable(label,date, *p_selected, withDelete, type, flags, size_arg).first)
		{
			*p_selected = !*p_selected;
			
			return { true,false };
		}

		if (UIMapSelectable(label,date, *p_selected, withDelete, type, flags, size_arg).second)
		{
			return { false,true };
		}
		return { false,false };
	}

	bool UIImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_hover, const ImVec4& tint_pressed, const ImVec4& tint_color)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID((void*)(intptr_t)user_texture_id);
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();

		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UIImageButtonEx(id, user_texture_id, size, uv0, uv1, padding, bg_col, tint_hover, tint_pressed, tint_color);
	}
	bool UIImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_hover, const ImVec4& tint_pressed, const ImVec4& tint_color)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(bg_col));

		if (pressed || held)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_pressed));
			ImGui::SetMouseCursor(7);
		}
		else if (hovered)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_hover));
			ImGui::SetMouseCursor(7);
		}
		else
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_color));



		return pressed;
	}

	bool UILoadProject(string str)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;
		auto tex = tex_map["Assets/RecentProjectBtn.dds"];
		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID((void*)(intptr_t)(void*)static_cast<size_t>(tex->GetID()));
		string idstr = "#Loadimage" + str;
		const ImGuiID id = window->GetID(idstr.c_str());
		ImGui::PopID();

		
		ImVec4 tintPressed = { 0.305f, 0.612f, 0.717f, 1.f };
		ImVec4 tintHover = { 0.443f, 0.690f, 0.775f, 1.f };
		ImVec2 size = { (float)tex->GetWidth(), (float)tex->GetHeight() };
		int frame_padding = 0;
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UILoadProjectEx(id, (void*)static_cast<size_t>(tex->GetID()), size ,str, { 0,0 }, { 1,1 }, padding, { 0,0,0,0 }, tintHover, tintPressed);
	}
	bool UILoadProjectEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, string str, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_hover, const ImVec4& tint_pressed)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(bg_col));

		if (pressed || held )
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_pressed));
			ImGui::SetMouseCursor(7);
		}
		else if (hovered)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_hover));
			ImGui::SetMouseCursor(7);
		}
		else
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32({ 1,1,1,1 }));
		
		ImGui::PushFont(FONT_BTN);

		string addstr = str;
		ImVec2 strPos = {bb.Min.x + size.x * 0.15f, bb.Min.y + size.y * 0.5f - ImGui::CalcTextSize(str.c_str()).y * 0.5f};
		window->DrawList->AddText(strPos, ImGui::GetColorU32({ 0,0,0,1 }), addstr.c_str());
		ImGui::PopFont();
		
		

		return pressed;
	}

	bool UIImgBtnWithText(ImTextureID user_texture_id, const ImVec2& size, string str)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;
		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(user_texture_id);
		string idstr = "#Loadimage" + str;
		const ImGuiID id = window->GetID(idstr.c_str());
		ImGui::PopID();


		ImVec4 tintPressed = { 0.305f, 0.612f, 0.717f, 1.f };
		ImVec4 tintHover = { 0.443f, 0.690f, 0.775f, 1.f };
		int frame_padding = 0;
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UIImgBtnWithTextEx(id, user_texture_id, size, str, { 0,0 }, { 1,1 }, padding, { 0,0,0,0 }, tintHover, tintPressed);
	}
	bool UIImgBtnWithTextEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, string str, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_hover, const ImVec4& tint_pressed)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(bg_col));

		if (pressed || held)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_pressed));
			ImGui::SetMouseCursor(7);
		}
		else if (hovered)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_hover));
			ImGui::SetMouseCursor(7);
		}
		else
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32({ 1,1,1,1 }));

		ImGui::PushFont(FONT_BTN);

		string addstr = str;
		 //strPos = { bb.Min.x + size.x * 0.15f, bb.Min.y + size.y * 0.5f - ImGui::CalcTextSize(str.c_str()).y * 0.5f };
		ImVec2 strPos = { bb.Min.x + size.x * 0.5f - ImGui::CalcTextSize(str.c_str()).x * 0.5f, bb.Max.y };
		ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
		window->DrawList->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), addstr.c_str());
		ImGui::PopFont();



		return pressed;
	}

	bool UIImgBtnWithText2(ImTextureID user_texture_id, const ImVec2& size, string str)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;
		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(user_texture_id);
		string idstr = "#Loadimage" + str;
		const ImGuiID id = window->GetID(idstr.c_str());
		ImGui::PopID();


		ImVec4 tintPressed = { 0.305f, 0.612f, 0.717f, 1.f };
		ImVec4 tintHover = { 0.443f, 0.690f, 0.775f, 1.f };
		int frame_padding = 0;
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UIImgBtnWithText2Ex(id, user_texture_id, size, str, { 0,0 }, { 1,1 }, padding, { 0,0,0,0 }, tintHover, tintPressed);
	}
	bool UIImgBtnWithText2Ex(ImGuiID id, ImTextureID texture_id, const ImVec2& size, string str, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_hover, const ImVec4& tint_pressed)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(bg_col));

		if (pressed || held)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_pressed));
			ImGui::SetMouseCursor(7);
		}
		else if (hovered)
		{
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint_hover));
			ImGui::SetMouseCursor(7);
		}
		else
			window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32({ 1,1,1,1 }));

		ImGui::PushFont(FONT_PARA);
		
		string addstr = str;
		//strPos = { bb.Min.x + size.x * 0.15f, bb.Min.y + size.y * 0.5f - ImGui::CalcTextSize(str.c_str()).y * 0.5f };
		ImVec2 strPos = { bb.Min.x + size.x * 0.3f - ImGui::CalcTextSize(str.c_str()).x * 0.5f,  bb.Min.y + size.y * 0.5f - ImGui::CalcTextSize(str.c_str()).y * 0.5f };
		//ImGui::GetWindowDrawList()->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
		window->DrawList->AddText(strPos, ImGui::GetColorU32({ 0,0,0,1 }), addstr.c_str());
		ImGui::PopFont();


		return pressed;
	}
	std::pair<bool, bool> UICharButton_WithDeleteEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float alpha = selected ? 1.f : 0;
		if (window->SkipItems)
			return { false,false };
		auto arrowImg = tex_map["Assets/SelectArrow.dds"];
		ImVec2 arrowSize = { (float)arrowImg->GetWidth(), (float)arrowImg->GetHeight() };
		ImVec2 newsize = { size.x, size.y + arrowSize.y };
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + newsize.x + padding.x * 2, window->DC.CursorPos.y + newsize.y + padding.y * 2 });
		
		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, alpha };
		ImVec4 arrowTint_col = { 1.f,1.f,1.f,alpha };

		ImVec2 frameMin = { bb.Min.x, bb.Min.y + arrowSize.y };
		
		ImGui::RenderFrame(frameMin, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		window->DrawList->AddImage((void*)static_cast<size_t>(arrowImg->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y - size.y }, uv0, uv1, ImGui::GetColorU32(arrowTint_col));
		window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y + arrowSize.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));
		
		//DELETE BTN
		auto pos = ImGui::GetCursorPos();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
		ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
		bool remove = false;
		ImGui::Dummy({ 0, newsize.y * 0.2f });
		ImGui::Dummy({ newsize.x * 0.75f,0 });
		ImGui::SameLine();
		if (ImGui::Button((string(ICON_FA_TRASH) + label).c_str()))
		{
			AudioEngine ae;
			ae.Play("Sounds2D/SFX_DeleteFile.wav", "SFX");
			//ImGui::PopStyleVar();
			remove = true;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(7);
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::SetCursorPos(pos);

		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return { false,false };

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		ImGui::RenderNavHighlight(bb, id);
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));

		return { pressed,remove };
	}
	std::pair<bool, bool> UICharButton_WithDelete(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return { false, false };

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(label.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;

		ImGui::BeginGroup();
		auto res = UICharButton_WithDeleteEx(id, user_texture_id, label, size, selected, uv0, uv1, padding, bg_col, tint_col);
		ImGui::PushFont(FONT_BODY);
		auto textWidth = ImGui::CalcTextSize(charName.c_str()).x;
		ImGui::Dummy({ 0, 5.f });
		ImGui::Dummy({ (size.x - textWidth) * 0.5f, 0.f });
		ImGui::SameLine();
		ImGui::Text(charName.c_str());
		ImGui::PopFont();
		ImGui::EndGroup();
		return res;
	}
	bool UICharButton_ArrowEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float alpha = selected ? 1.f : 0.f;
		if (window->SkipItems)
			return  false;
		auto arrowImg = tex_map["Assets/Arrow_glow.dds"];
		ImVec2 arrowSize = { (float)arrowImg->GetWidth(), (float)arrowImg->GetHeight() };
		ImVec2 newsize = { size.x, size.y + arrowSize.y };
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + newsize.x + padding.x * 2, window->DC.CursorPos.y + newsize.y + padding.y * 2 });

		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, alpha };
		ImVec4 arrowTint_col = { 1.f,1.f,1.f,alpha };

		ImVec2 frameMin = { bb.Min.x, bb.Min.y + arrowSize.y };

		ImGui::RenderFrame(frameMin, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
		window->DrawList->AddImage((void*)static_cast<size_t>(arrowImg->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y - size.y }, uv0, uv1, ImGui::GetColorU32(arrowTint_col));
		window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y + arrowSize.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));


		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		ImGui::RenderNavHighlight(bb, id);
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));

		return pressed;
	}

	bool UICharButton_Arrow(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(label.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;

		auto res = UICharButton_ArrowEx(id, user_texture_id, label, size, selected, uv0, uv1, padding, bg_col, tint_col);

		return res;
	}
	bool UICharButton_NoDeleteEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float alpha = selected ? 1.f : 0;
		if (window->SkipItems)
			return false;
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x + padding.x * 2, window->DC.CursorPos.y + size.y + padding.y * 2 });

		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, alpha };

		ImGui::RenderNavHighlight(bb, id);
		ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));

		window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y}, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));

		return pressed;
	}

	bool UICharButton_NoDelete(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(label.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;

		ImGui::BeginGroup();
		auto res = UICharButton_NoDeleteEx(id, user_texture_id, label, size, selected, uv0, uv1, padding, bg_col, tint_col);
		ImGui::PushFont(FONT_BODY);
		auto textWidth = ImGui::CalcTextSize(charName.c_str()).x;
		ImGui::Dummy({ 0, 5.f });
		ImGui::Dummy({ (size.x - textWidth) * 0.5f, 0.f });
		ImGui::SameLine();
		ImGui::Text(charName.c_str());
		ImGui::PopFont();
		ImGui::EndGroup();
		return res;
	}

	bool UICharButton_ToggleEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		//float alpha = selected ? 1 : 0;
		if (window->SkipItems)
			return false;
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x + padding.x * 2, window->DC.CursorPos.y + size.y + padding.y * 2 });
		


		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, 1 };
		ImVec4 emptyCol = { 0.1f, 0.1f, 0.1f, 1 };
		hovered ? emptyCol = { 0.2f, 0.2f, 0.2f, 1 } : emptyCol;

		//ImVec4 hoverCol = { 0.4f, 0.4f, 0.4f, 1 };

		ImGui::RenderNavHighlight(bb, id);
		ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		window->DrawList->AddRectFilled({ bb.Min.x + padding.x, bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(emptyCol));

		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));

		if(selected)
			window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));
		else
			window->DrawList->AddImage((void*)static_cast<size_t>(tex_map["Assets/EmptySimulateUnit.dds"]->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));
		if (pressed)
		{
			AudioEngine ae;
			ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
		}

		return pressed;
	}

	bool UICharButton_Toggle(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(label.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();
		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;

		ImGui::BeginGroup();
		auto res = UICharButton_ToggleEx(id, user_texture_id, label, size, selected, uv0, uv1, padding, bg_col, tint_col);
		ImGui::PushFont(FONT_BODY);
		auto textWidth = ImGui::CalcTextSize(charName.c_str()).x;
		ImGui::Dummy({ 0, 5.f });
		ImGui::Dummy({ (size.x - textWidth) * 0.5f, 0.f });
		ImGui::SameLine();
		ImGui::Text(charName.c_str());
		ImGui::PopFont();
		ImGui::EndGroup();
		return res;
	}

	bool UIActionButtonEx(ImGuiID id, string actionName, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		
		auto defaultImg = tex_map["Assets/SkillUnselected.dds"];
		auto selectedImg = tex_map["Assets/SkillSelected.dds"];
		ImVec2 size = { 0,0 };
		ImTextureID texture_id = 0;
		if (selected)
		{
			size = { (float)selectedImg->GetWidth(), (float)selectedImg->GetHeight() };
			texture_id = (void*)static_cast<size_t>(selectedImg->GetID());
		}
		else
		{
			size = { (float)defaultImg->GetWidth(), (float)defaultImg->GetHeight() };
			texture_id = (void*)static_cast<size_t>(defaultImg->GetID());
		}


		if (window->SkipItems)
			return false;
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x + padding.x * 2, window->DC.CursorPos.y + size.y + padding.y * 2 });

		

		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, 1 };
		ImVec4 emptyCol = { 0.1f, 0.1f, 0.1f, 1 };
		hovered ? emptyCol = { 0.2f, 0.2f, 0.2f, 1 } : emptyCol;

		//ImVec4 hoverCol = { 0.4f, 0.4f, 0.4f, 1 };

		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		//window->DrawList->AddRectFilled({ bb.Min.x + padding.x, bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(emptyCol));
		
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));
		if(hovered && !selected)
			window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32({ 0.980f, 0.768f, 0.509f, 1.f }));
		else
			window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));

		ImVec2 textPos = { bb.Min.x + size.x * 0.35f, bb.Min.y + size.y * 0.25f };
		ImGui::PushFont(FONT_TURN);
		window->DrawList->AddText(textPos, ImGui::GetColorU32({ 1,1,1,1 }), actionName.c_str());
		ImGui::PopFont();

		if (pressed)
		{
			AudioEngine ae;
			ae.Play("Sounds2D/Button_Click.wav", "SFX", 1.f);
		}

		return pressed;
	}

	

	bool UIActionButton(string actionName, string labelID, bool selected, const ImVec2& uv0, const ImVec2& uv1 , int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(labelID.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();

		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UIActionButtonEx(id, actionName, selected, uv0, uv1, padding, bg_col, tint_col);
	}

	bool UICharTurnButtonEx(ImGuiID id, ImTextureID texture_id, string characterName, const ImVec2& size, bool selected, bool isPlaced, bool isBig, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		//ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		
		
		if (window->SkipItems)
			return  false;
		auto arrowImg = tex_map["Assets/Arrow_glow.dds"];
		ImVec2 arrowSize = { (float)arrowImg->GetWidth(), (float)arrowImg->GetHeight() };
		ImVec2 newsize = { size.x, size.y + arrowSize.y };

		auto unselectedImg = tex_map["Assets/TurnUnselectedBtn.dds"];
		auto HoveredImg = tex_map["Assets/TurnHoverBtn.dds"];
		auto SelectedImg = tex_map["Assets/TurnSelectedBtn.dds"];
		auto PlacedImg = tex_map["Assets/TurnPlacedBtn.dds"];
		auto BGOverlay = tex_map["Assets/SelectedBGOverlay.dds"];
		auto newSize = size;
		auto font = FONT_TURN;
		ImTextureID selectedID = 0;
		ImVec2 selectedSize = { 0,0 };

		if (isBig)
			newSize = { size.x * 1.68f, size.y * 1.68f }; //Character Icon Size
		else
		{	//selectedSize = { selectedSize.x / 1.68f, selectedSize.y / 1.68f };
			font = FONT_OPEN;
			unselectedImg = tex_map["Assets/SmallTurnUnselectedBtn.dds"];
			HoveredImg = tex_map["Assets/SmallTurnHoverBtn.dds"];
		}
		selectedSize = { (float)HoveredImg->GetWidth(), (float)HoveredImg->GetHeight() };
		/*if (selected)
		{
			selectedSize = { (float)HoveredImg->GetWidth(), (float)HoveredImg->GetHeight() };
			selectedID = (void*)static_cast<size_t>(HoveredImg->GetID());
		}
		else
		{
			selectedSize = { (float)unselectedImg->GetWidth(), (float)unselectedImg->GetHeight() };
			selectedID = (void*)static_cast<size_t>(unselectedImg->GetID());
		}*/
		
			

		if (window->SkipItems)
			return false;

		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + selectedSize.x + padding.x * 2, window->DC.CursorPos.y + selectedSize.y + padding.y * 2 });
		ImVec2 CharacterImgMax = { bb.Min.x + newSize.x, bb.Min.y + newSize.y };


		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		if (hovered || pressed)
		{
			selectedID = (void*)static_cast<size_t>(HoveredImg->GetID());
		}
		else
		{
			selectedID = (void*)static_cast<size_t>(unselectedImg->GetID());
		}
		// Render
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImVec4 selectedCol = { 0.980f, 0.768f, 0.509f, 1 };
		ImVec4 emptyCol = { 0.1f, 0.1f, 0.1f, 1 };
		hovered ? emptyCol = { 0.2f, 0.2f, 0.2f, 1 } : emptyCol;

		//ImVec4 hoverCol = { 0.4f, 0.4f, 0.4f, 1 };

		ImGui::RenderNavHighlight(bb, id);
		//ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(selectedCol), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		//window->DrawList->AddRectFilled({ bb.Min.x + padding.x, bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(emptyCol));

		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled({ bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, ImGui::GetColorU32(bg_col));

		window->DrawList->AddImage(selectedID, { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(ImVec4{ 1,1,1,1 }));
		
		//Character Icon
		window->DrawList->AddImage(texture_id, { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { CharacterImgMax.x - padding.x, CharacterImgMax.y - padding.y }, uv0, uv1, ImGui::GetColorU32(tint_col));

		if (selected)
		{
			window->DrawList->AddImage((void*)static_cast<size_t>(BGOverlay->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(ImVec4{ 1,1,1,0.5f }));
			window->DrawList->AddImage((void*)static_cast<size_t>(SelectedImg->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(ImVec4{ 1,1,1,1 }));
		}
			

		if (isPlaced && !selected)
		{
			window->DrawList->AddImage((void*)static_cast<size_t>(BGOverlay->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(ImVec4{ 1,1,1,0.5f }));
			window->DrawList->AddImage((void*)static_cast<size_t>(PlacedImg->GetID()), { bb.Min.x + padding.x,  bb.Min.y + padding.y }, { bb.Max.x - padding.x, bb.Max.y - padding.y }, uv0, uv1, ImGui::GetColorU32(ImVec4{ 1,1,1,1 }));
		}
			

		ImVec2 textPos = { bb.Min.x + selectedSize.x * 0.62f, bb.Min.y + selectedSize.y * 0.2f };
		ImGui::PushFont(font);
		window->DrawList->AddText(textPos, ImGui::GetColorU32({ 0,0,0,1 }), characterName.c_str());
		ImGui::PopFont();
		return pressed;
	}



	bool UICharTurnButton(ImTextureID user_texture_id, const ImVec2& size, string actionName, string labelID, bool selected, bool isPlaced, bool isBig, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		if (window->SkipItems)
			return false;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		ImGui::PushID(labelID.c_str());
		const ImGuiID id = window->GetID("#image");
		ImGui::PopID();

		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;
		return UICharTurnButtonEx(id, user_texture_id, actionName, size, selected, isPlaced, isBig, uv0, uv1, padding, bg_col, tint_col);
	}

	
	void CharacterTurn(Instance& instance, Entity id, const ImVec2 pos, bool selected)
	{
		auto window = ImGui::GetWindowDrawList();
		auto windowPos = ImGui::GetCurrentWindow()->Pos;
		
		
		ImVec2 Min = { windowPos.x + pos.x, windowPos.y + pos.y };
		
		
		auto character = instance.ecs.get_if<tc::Character>(id);
		auto characterImg = tex_map["Assets/Unit_Black.dds"];

		if (selected)
		{
			auto selectedImg = tex_map["Assets/TurnSelected.dds"];
			ImVec2 selectedMax = { Min.x + selectedImg->GetWidth(), Min.y + selectedImg->GetHeight() };
			ImVec2 characterImgMin = { Min.x + selectedImg->GetWidth() * 0.1f, Min.y };
			ImVec2 characterImgMax = { characterImgMin.x + characterImg->GetWidth(), characterImgMin.y + characterImg->GetHeight() };
			window->AddImage((void*)static_cast<size_t>(selectedImg->GetID()), Min, selectedMax);
			window->AddImage((void*)static_cast<size_t>(characterImg->GetID()), characterImgMin, characterImgMax, ImVec2{ 0,0 }, ImVec2{ 1,1 }, ImGui::GetColorU32(ImVec4{ character->color.x,character->color.y, character->color.z, 1.0f }));
			ImVec2 TextStartPos = { Min.x + selectedImg->GetWidth() * 0.53f, Min.y + selectedImg->GetHeight() * 0.17f };
			ImGui::PushFont(FONT_OPEN);

			if (character)
				window->AddText(TextStartPos, ImGui::GetColorU32({ 0,0,0,1 }), character->name.c_str());
			else
				window->AddText(TextStartPos, ImGui::GetColorU32({ 0,0,0,1 }), "NAN");

			ImGui::PopFont();
		}
		else
		{
			auto unselectedImg = tex_map["Assets/TurnUnselected.dds"];
			ImVec2 unselectedMax = { Min.x + unselectedImg->GetWidth(), Min.y + unselectedImg->GetHeight() };
			ImVec2 characterImgMax = { Min.x + characterImg->GetWidth(), Min.y + characterImg->GetHeight() };
			window->AddImage((void*)static_cast<size_t>(unselectedImg->GetID()), Min, unselectedMax);
			window->AddImage((void*)static_cast<size_t>(characterImg->GetID()), Min, characterImgMax, ImVec2{ 0,0 }, ImVec2{ 1,1 }, ImGui::GetColorU32(ImVec4{ character->color.x,character->color.y, character->color.z, 1.0f }));
			ImVec2 TextStartPos = { Min.x + unselectedImg->GetWidth() * 0.45f, Min.y + unselectedImg->GetHeight() * 0.17f };
			ImGui::PushFont(FONT_OPEN);

			if (character)
				window->AddText(TextStartPos, ImGui::GetColorU32({ 0,0,0,1 }), character->name.c_str());
			else
				window->AddText(TextStartPos, ImGui::GetColorU32({ 0,0,0,1 }), "NAN");

			ImGui::PopFont();
		}
	}

	bool CharacterTurnData(Instance& instance, Entity id, const ImVec2 pos, bool isRightSide, bool isRoll)
	{
		auto window = ImGui::GetWindowDrawList();
		auto windowPos = ImGui::GetCurrentWindow()->Pos;
		auto character = instance.ecs.get_if<tc::Character>(id);

		auto selectedImg = tex_map["Assets/CharacterBackdrop.dds"];
		auto characterImg = tex_map["Assets/Placeholder_Character.dds"];
		bool ret = false;
		ImVec2 Min = { windowPos.x + pos.x, windowPos.y + pos.y };
		

		if (isRightSide)
		{
			selectedImg = tex_map["Assets/ActionBackdrop.dds"];
			Min = { Min.x - selectedImg->GetWidth(), Min.y };
		}
		ImVec2 charImgMax = { Min.x + characterImg->GetWidth(), Min.y + characterImg->GetHeight() };
		ImVec2 selectedMin = { Min.x, Min.y + characterImg->GetHeight() * 0.45f };
		ImVec2 selectedMax = { selectedMin.x + selectedImg->GetWidth(), selectedMin.y + selectedImg->GetHeight() };

		if (isRightSide)
		{
			ImVec2 charImgMin = { windowPos.x + pos.x - characterImg->GetWidth(), windowPos.y + pos.y };
			charImgMax = { charImgMin.x + characterImg->GetWidth(), charImgMin.y + characterImg->GetHeight() };
			//window->AddImage((void*)static_cast<size_t>(selectedImg->GetID()), selectedMin, selectedMax, { 1,0 }, { 0,1 });
			window->AddImage((void*)static_cast<size_t>(selectedImg->GetID()), selectedMin, selectedMax);
			window->AddImage((void*)static_cast<size_t>(characterImg->GetID()), charImgMin, charImgMax, { 1,0 }, { 0,1 }, ImGui::GetColorU32(ImVec4{ character->color.x, character->color.y,character->color.z, 1.0f }));
		}
		else
		{
			window->AddImage((void*)static_cast<size_t>(selectedImg->GetID()), selectedMin, selectedMax);
			window->AddImage((void*)static_cast<size_t>(characterImg->GetID()), Min, charImgMax, ImVec2{ 0,0 }, ImVec2{ 1,1 }, ImGui::GetColorU32(ImVec4{ character->color.x, character->color.y,character->color.z, 1.0f }));
		}
		
		ImVec2 namePos = { selectedMin.x + selectedImg->GetWidth() * 0.55f, selectedMin.y + selectedImg->GetHeight() * 0.09f };
		ImVec2 hpPos = { selectedMin.x + selectedImg->GetWidth() * 0.60f, selectedMin.y + selectedImg->GetHeight()   * 0.3f  };
		ImVec2 atkPos = { selectedMin.x + selectedImg->GetWidth() * 0.60f, selectedMin.y + selectedImg->GetHeight()  * 0.425f  };
		ImVec2 defPos = { selectedMin.x + selectedImg->GetWidth() * 0.60f, selectedMin.y + selectedImg->GetHeight()  * 0.55f  };
		ImVec2 rangePos = { selectedMin.x + selectedImg->GetWidth() * 0.60f, selectedMin.y + selectedImg->GetHeight()* 0.675f  };
		ImVec2 movePos = { selectedMin.x + selectedImg->GetWidth() * 0.60f, selectedMin.y + selectedImg->GetHeight() * 0.8f  };

		if (isRightSide)
		{
			namePos = { selectedMin.x + selectedImg->GetWidth() * 0.18f, selectedMin.y + selectedImg->GetHeight() * 0.08f };
			hpPos = { selectedMin.x + selectedImg->GetWidth() * 0.2f, selectedMin.y + selectedImg->GetHeight() * 0.3f };
			atkPos = { selectedMin.x + selectedImg->GetWidth() * 0.2f, selectedMin.y + selectedImg->GetHeight() * 0.4f };
			defPos = { selectedMin.x + selectedImg->GetWidth() * 0.2f, selectedMin.y + selectedImg->GetHeight() * 0.5f };
			rangePos = { selectedMin.x + selectedImg->GetWidth() * 0.2f, selectedMin.y + selectedImg->GetHeight() * 0.6f };
			movePos = { selectedMin.x + selectedImg->GetWidth() * 0.2f, selectedMin.y + selectedImg->GetHeight() * 0.7f };
		}
		
		if (!isRoll)
		{
			if (character)
			{
				ImGui::PushFont(FONT_TURN);
				window->AddText(namePos, ImGui::GetColorU32({ 0,0,0,1 }), character->name.c_str());
				ImGui::PopFont();

				ImGui::PushFont(FONT_OPEN_30);
				int curHp = character->get_stat(0) + character->get_statDelta(0);
				string str = "  HP   " + std::to_string(curHp) +"/" +std::to_string(character->get_stat(0));
				window->AddText(hpPos, ImGui::GetColorU32({ 1,0,0,1 }), str.c_str());
				str = "ATK   " + std::to_string(character->get_stat(1));
				window->AddText(atkPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "DEF   " + std::to_string(character->get_stat(2));
				window->AddText(defPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "RNG  " + std::to_string(character->get_stat(3));
				window->AddText(rangePos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "MOV  " + std::to_string(character->get_stat(4));
				window->AddText(movePos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				ImGui::PopFont();
			}
			else
			{
				ImGui::PushFont(FONT_TURN);
				window->AddText(namePos, ImGui::GetColorU32({ 0,0,0,1 }), "NAN");
				ImGui::PopFont();

				ImGui::PushFont(FONT_OPEN_30);
				string str = " HP   NAN";
				window->AddText(hpPos, ImGui::GetColorU32({ 1,0,0,1 }), str.c_str());
				str = "ATK   NAN";
				window->AddText(atkPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "DEF   NAN";
				window->AddText(defPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "RNG  NAN";
				window->AddText(rangePos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				str = "MOV  NAN";
				window->AddText(movePos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
				ImGui::PopFont();
			}
		}
		else
		{
			ImVec2 btnPos = { pos.x + selectedImg->GetWidth() * 0.67f, pos.y + characterImg->GetHeight() * 0.75f };
			if(isRightSide)
				btnPos = { pos.x - selectedImg->GetWidth() * 0.70f, pos.y + characterImg->GetHeight() * 0.75f };

			if (character)
			{
				ImGui::PushFont(FONT_TURN);
				window->AddText(namePos, ImGui::GetColorU32({ 0,0,0,1 }), character->name.c_str());
				ImGui::PopFont();
			}
			else
			{
				ImGui::PushFont(FONT_TURN);
				window->AddText(namePos, ImGui::GetColorU32({ 0,0,0,1 }), "NAN");
				ImGui::PopFont();
			}
			ret = UI::UIButton_2("Roll", "Roll", btnPos, { -50.f, 15.f }, FONT_BODY);
		}
		return ret;
	}

	void ActionUI(const ImVec2 pos, string title)
	{
		auto window = ImGui::GetWindowDrawList();
		auto windowPos = ImGui::GetCurrentWindow()->Pos;
		auto actionImg = tex_map["Assets/ActionBackdrop.dds"];

		ImVec2 topleft = { pos.x - actionImg->GetWidth(), pos.y };
		ImVec2 Min = { windowPos.x + topleft.x, windowPos.y + topleft.y };
		ImVec2 Max = { Min.x + actionImg->GetWidth(), Min.y + actionImg->GetHeight() };
		ImVec2 titlePos = { Min.x + actionImg->GetWidth() * 0.1f, Min.y + actionImg->GetHeight() * 0.09f };

		window->AddImage((void*)static_cast<size_t>(actionImg->GetID()), Min, Max);
		ImGui::PushFont(FONT_TURN);
		window->AddText(titlePos, ImGui::GetColorU32({ 0,0,0,1 }), title.c_str());
		ImGui::PopFont();
	}

	void AttackSuccessUI(string name, const ImVec2 pos, int successVal)
	{
		auto window = ImGui::GetWindowDrawList();
		auto windowPos = ImGui::GetCurrentWindow()->Pos;
		auto arrowImg = tex_map["Assets/SuccessArrow.dds"];
		auto gradientImg = tex_map["Assets/AttackGradient.dds"];

		//ImVec2 arrowMin = { windowPos.x + pos.x, windowPos.y + pos.y };
		ImVec2 arrowMin = { windowPos.x + pos.x - arrowImg->GetWidth() * 0.5f, windowPos.y + pos.y - arrowImg->GetHeight() * 0.5f };
		ImVec2 arrowMax = { arrowMin.x + arrowImg->GetWidth(), arrowMin.y + arrowImg->GetHeight() };
		

		ImGui::PushFont(FONT_HEAD);
		string atkStr = name.c_str();
		auto atkTextSize = ImGui::CalcTextSize(atkStr.c_str());
		ImVec2 atkTextPos = { arrowMin.x, arrowMin.y - atkTextSize.y *0.5f };

		window->AddImage((void*)static_cast<size_t>(gradientImg->GetID()), atkTextPos, { arrowMin.x + arrowImg->GetWidth(), arrowMin.y + arrowImg->GetHeight()*0.5f });
		window->AddImage((void*)static_cast<size_t>(arrowImg->GetID()), arrowMin, arrowMax);
		window->AddText(atkTextPos, ImGui::GetColorU32({ 1,1,1,1 }), atkStr.c_str());
		ImGui::PopFont();

		ImGui::PushFont(FONT_BODY);
		string succStr = "Success";
		auto succTextSize = ImGui::CalcTextSize(succStr.c_str());
		ImVec2 succTextPos = { arrowMin.x + arrowImg->GetWidth() * 0.55f, arrowMin.y  };
		window->AddText(succTextPos, ImGui::GetColorU32({ 1,1,1,1 }), succStr.c_str());
		ImGui::PopFont();

		ImGui::PushFont(FONT_SHEAD);
		string val = (std::to_string(successVal) + "%");
		auto valTextSize = ImGui::CalcTextSize(val.c_str());
		ImVec2 valTextPos = { arrowMin.x + arrowImg->GetWidth() * 0.7f, arrowMin.y - valTextSize.y * 0.2f };
		window->AddText(valTextPos, ImGui::GetColorU32({ 0.612f, 0.9f,0.271f,1.f }), val.c_str());
		ImGui::PopFont();
	}

	bool UISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return UISliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	bool UISliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = ImGui::CalcItemWidth();

		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
		const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

		const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemAddFlags_Focusable : 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
		//else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
		//	format = PatchFormatStringFloatToInt(format);
		
		// Tabbing or CTRL-clicking on Slider turns it into an input box
		const bool hovered = ImGui::ItemHoverable(frame_bb, id);
		bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
		if (!temp_input_is_active)
		{
			const bool focus_requested = temp_input_allowed && (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_Focused) != 0;
			const bool clicked = (hovered && g.IO.MouseClicked[0]);
			if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
			{
				ImGui::SetActiveID(id, window);
				ImGui::SetFocusID(id, window);
				ImGui::FocusWindow(window);
				g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
				if (temp_input_allowed && (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id))
					temp_input_is_active = true;
			}
		}

		if (temp_input_is_active)
		{
			// Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
			const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
			return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
		}

		// Draw frame
		const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		//ImGui::RenderNavHighlight(frame_bb, id);
		//ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);
		ImVec2 lineMin = { frame_bb.Min.x, (frame_bb.Min.y + frame_bb.Max.y) * 0.5f };
		ImVec2 lineMax = { frame_bb.Max.x, lineMin.y };
		window->DrawList->AddLine(lineMin, lineMax, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
		// Slider behavior
		ImRect grab_bb;
		const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
		if (value_changed)
			ImGui::MarkItemEdited(id);


		ImVec4 tintHover = { 0.980f, 0.768f, 0.509f, 1.f };
		ImVec4 tintPressed = { 0.784f, 0.616f, 0.408f, 1.f };
		// Render grab
		grab_bb.Min = { grab_bb.Min.x - 10.f, grab_bb.Min.y - 2.f };
		grab_bb.Max = { grab_bb.Max.x + 20.f, grab_bb.Max.y + 4.f };
		
		if (grab_bb.Max.x > grab_bb.Min.x)
			window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, ImGui::GetColorU32(g.ActiveId == id ? tintPressed : tintHover),0.f);

		// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
		char value_buf[64];
		const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
		if (g.LogEnabled)
			ImGui::LogSetNextTextDecoration("{", "}");
		ImGui::PushFont(FONT_BODY);
		ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,1 });
		ImGui::RenderTextClipped(grab_bb.Min, grab_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));
		ImGui::PopStyleColor();
		ImGui::PopFont();
		if (label_size.x > 0.0f)
			ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		return value_changed;
	}

	void TutArea(ImVec2 pos, ImVec2 size, bool border)
	{
		auto drawlist = ImGui::GetForegroundDrawList();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 min = pos;
		ImVec2 max = { min.x + size.x, min.y + size.y };
		auto tex = tex_map["Assets/MIBG.dds"];
		ImRect TopBox = { {0.f,0.f}, {viewport->Size.x , min.y} };
		ImRect RightBox = { {max.x ,min.y}, {viewport->Size.x , viewport->Size.y} };
		ImRect BtmBox = { {0.f , max.y}, {viewport->Size.x - RightBox.GetWidth() , viewport->Size.y} };
		ImRect LeftBox = { {0.f ,min.y}, {min.x , max.y} };
		ImVec4 col = { 0,0,0,0.7f };
		
		drawlist->AddRectFilled(TopBox.Min, TopBox.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(RightBox.Min, RightBox.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(BtmBox.Min, BtmBox.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(LeftBox.Min, LeftBox.Max, ImGui::GetColorU32(col));
		if(border)
			drawlist->AddRect(min, max, ImGui::GetColorU32({ 1,1,1,1.f }));

		/*block_input_if_mouse_not_in_bounds(pos, size);*/

	}

	void TutArea2(ImVec2 pos1, ImVec2 pos2, ImVec2 size1, ImVec2 size2, bool border)
	{
		auto drawlist = ImGui::GetForegroundDrawList();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		auto tex = tex_map["Assets/MIBG.dds"];
		ImVec4 col = { 0,0,0,0.7f };
		ImVec2 min1 = pos1;
		ImVec2 max1 = { min1.x + size1.x, min1.y + size1.y };
		ImVec2 min2 = pos2;
		ImVec2 max2 = { min2.x + size2.x, min2.y + size2.y };
		
		ImRect Box1 = { {0.f,0.f}, {viewport->Size.x , min2.y} };
		ImRect Box2 = { {0.f, Box1.Max.y}, {min2.x , min1.y} };
		ImRect Box3 = { {max2.x, min2.y}, {viewport->Size.x,viewport->Size.y} };
		ImRect Box4 = { {0.f, min1.y}, {min1.x,max1.y} };
		ImRect Box5 = { {max1.x, min1.y}, {min2.x, max1.y} };
		ImRect Box6 = { {0.f, max1.y}, {min2.x, viewport->Size.y} };
		ImRect Box7 = { {min2.x, max2.y}, {max2.x, viewport->Size.y}};

		drawlist->AddRectFilled(Box1.Min, Box1.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box2.Min, Box2.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box3.Min, Box3.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box4.Min, Box4.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box5.Min, Box5.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box6.Min, Box6.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box7.Min, Box7.Max, ImGui::GetColorU32(col));
		if (border)
		{
			drawlist->AddRect(min1, max1, ImGui::GetColorU32({ 1,1,1,1.f }));
			drawlist->AddRect(min2, max2, ImGui::GetColorU32({ 1,1,1,1.f }));
		}

		/*block_input_if_mouse_not_in_bounds(pos1, size1);
		block_input_if_mouse_not_in_bounds(pos2, size2);*/
			
	}
	void TutArea3(ImVec2 pos1, ImVec2 pos2, ImVec2 size1, ImVec2 size2, bool border)
	{
		auto drawlist = ImGui::GetForegroundDrawList();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		auto tex = tex_map["Assets/MIBG.dds"];
		ImVec4 col = { 0,0,0,0.7f };
		ImVec2 min1 = pos1;
		ImVec2 max1 = { min1.x + size1.x, min1.y + size1.y };
		ImVec2 min2 = pos2;
		ImVec2 max2 = { min2.x + size2.x, min2.y + size2.y };

		ImRect Box1 = { {0.f,0.f}, {viewport->Size.x , min1.y} };
		ImRect Box2 = { {0.f, min1.y}, {min1.x , max1.y} };
		ImRect Box3 = { {max1.x, min1.y}, {viewport->Size.x,max1.y} };
		ImRect Box4 = { {0.f, max1.y}, {viewport->Size.x, min2.y} };
		ImRect Box5 = { {0.f, min2.y}, {min2.x, max2.y} };
		ImRect Box6 = { {max2.x, min2.y}, {viewport->Size.x, viewport->Size.y} };
		ImRect Box7 = { {0.f, max2.y}, {max2.x, viewport->Size.y} };

		drawlist->AddRectFilled(Box1.Min, Box1.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box2.Min, Box2.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box3.Min, Box3.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box4.Min, Box4.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box5.Min, Box5.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box6.Min, Box6.Max, ImGui::GetColorU32(col));
		drawlist->AddRectFilled(Box7.Min, Box7.Max, ImGui::GetColorU32(col));
		if (border)
		{
			drawlist->AddRect(min1, max1, ImGui::GetColorU32({ 1,1,1,1.f }));
			drawlist->AddRect(min2, max2, ImGui::GetColorU32({ 1,1,1,1.f }));
		}

		/*block_input_if_mouse_not_in_bounds(pos1, size1);
		block_input_if_mouse_not_in_bounds(pos2, size2);*/

	}
	bool MouseIsWithin(const ImVec2 min, const ImVec2 max)
	{
		auto mousePos = ImGui::GetMousePos();
		if (mousePos.x > min.x && mousePos.x < max.x
			&& mousePos.y > min.y && mousePos.y < max.y)
			return true;


		return false;
	}

	void TutProgressBar(ImDrawList* drawlist, const ImVec2& viewport, int step)
	{
		const float diamondStep = viewport.x / 6.0f;

		// Tutorial progress line
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ viewport.x, viewport.y * 0.9f }, ImGui::GetColorU32({ 1,1,1,1 }), 4.0f);
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ 0 + diamondStep * step, viewport.y * 0.9f }, IM_COL32(250, 196, 130, 255), 4.0f);
		const string words[6] = { "Create a unit", "Create a weapon", "Create a action","Create a sequence","Simulate" };
		float currentDiamondPos = diamondStep;
		auto diamondImg = tex_map["Assets/TutorialDiamond.dds"];
		for (int i = 0; i < 5; ++i)
		{
			ImGui::PushFont(FONT_SHEAD);
			const string stepText = "Step " + std::to_string(i + 1);
			drawlist->AddText({ currentDiamondPos - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(stepText.c_str()).x * 0.5f, viewport.y * 0.82f }, i < step ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }), stepText.c_str());
			ImGui::PopFont();

			ImGui::PushFont(FONT_BODY);
			const string bottomText = words[i];
			drawlist->AddText({ currentDiamondPos - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(bottomText.c_str()).x * 0.5f, viewport.y * 0.95f }, ImGui::GetColorU32({ 1,1,1,1 }), bottomText.c_str());
			ImGui::PopFont();

			ImVec2 diamond_min = { currentDiamondPos - diamondImg->GetWidth() * 1.0f, viewport.y * 0.925f - diamondImg->GetHeight() * 1.0f };
			ImVec2 diamond_max = { diamond_min.x + diamondImg->GetWidth() * 1.0f, diamond_min.y + diamondImg->GetHeight() * 1.0f };
			drawlist->AddImage((void*)static_cast<size_t>(diamondImg->GetID()), diamond_min, diamond_max, ImVec2{ 0,0 }, ImVec2{ 1,1 }, i < step ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }));
			currentDiamondPos += diamondStep;
		}
	}

	void TutProgressBar2(ImDrawList* drawlist, const ImVec2& viewport, Instance& instance, bool isFullBar)
	{
		int step = 1;
		const float diamondStep = viewport.x / 5.0f;

		// funky stuff
		if (!isFullBar && instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()).size_hint() >= 2)
		{
			step = 2;

			if (instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>()).size_hint() >= 2)
			{
				step = 3;

				if (instance.ecs.view<Components::ActionGraph>(exclude_t<tc::Destroyed>()).size_hint() >= 2)
				{
					step = 4;
				}
			}
		}
		else if (isFullBar)
			step = 5;

		// Tutorial progress line
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ viewport.x, viewport.y * 0.9f }, ImGui::GetColorU32({ 1,1,1,1 }), 4.0f);
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ 0 + diamondStep * step, viewport.y * 0.9f }, IM_COL32(250, 196, 130, 255), 4.0f);
		const string words[5] = { "Create an enemy unit", "Create a weapon", "Create a defense action","Simulate" };
		float currentDiamondPos = diamondStep;
		auto diamondImg = tex_map["Assets/TutorialDiamond.dds"];
		for (int i = 0; i < 4; ++i)
		{
			ImGui::PushFont(FONT_SHEAD);
			const string stepText = "Step " + std::to_string(i + 1);
			drawlist->AddText({ currentDiamondPos - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(stepText.c_str()).x * 0.5f, viewport.y * 0.82f }, i < step ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }), stepText.c_str());
			ImGui::PopFont();

			ImGui::PushFont(FONT_BODY);
			const string bottomText = words[i];
			drawlist->AddText({ currentDiamondPos - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(bottomText.c_str()).x * 0.5f, viewport.y * 0.95f }, ImGui::GetColorU32({ 1,1,1,1 }), bottomText.c_str());
			ImGui::PopFont();

			ImVec2 diamond_min = { currentDiamondPos - diamondImg->GetWidth() * 1.0f, viewport.y * 0.925f - diamondImg->GetHeight() * 1.0f };
			ImVec2 diamond_max = { diamond_min.x + diamondImg->GetWidth() * 1.0f, diamond_min.y + diamondImg->GetHeight() * 1.0f };
			drawlist->AddImage((void*)static_cast<size_t>(diamondImg->GetID()), diamond_min, diamond_max, ImVec2{ 0,0 }, ImVec2{ 1,1 }, i < step ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }));
			currentDiamondPos += diamondStep;
		}
	}

	void TutProgressBar3(ImDrawList* drawlist, const ImVec2& viewport, int step)
	{
		const float diamondStep = viewport.x / 2.0f;

		// Tutorial progress line
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ viewport.x, viewport.y * 0.9f }, ImGui::GetColorU32({ 1,1,1,1 }), 4.0f);
		drawlist->AddLine(ImVec2{ 0, viewport.y * 0.9f }, ImVec2{ 0 + diamondStep * step, viewport.y * 0.9f }, IM_COL32(250, 196, 130, 255), 4.0f);
		const string bottomText = "Attain a simulation result of 50% - 60%";
		auto diamondImg = tex_map["Assets/TutorialDiamond.dds"];
		ImGui::PushFont(FONT_SHEAD);
		const string stepText = "Step 1";
		drawlist->AddText({ diamondStep - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(stepText.c_str()).x * 0.5f, viewport.y * 0.82f }, step >= 1 ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }), stepText.c_str());
		ImGui::PopFont();

		ImGui::PushFont(FONT_BODY);
		drawlist->AddText({ diamondStep - diamondImg->GetWidth() * 0.5f - ImGui::CalcTextSize(bottomText.c_str()).x * 0.5f, viewport.y * 0.95f }, ImGui::GetColorU32({ 1,1,1,1 }), bottomText.c_str());
		ImGui::PopFont();

		ImVec2 diamond_min = { diamondStep - diamondImg->GetWidth() * 1.0f, viewport.y * 0.925f - diamondImg->GetHeight() * 1.0f };
		ImVec2 diamond_max = { diamond_min.x + diamondImg->GetWidth() * 1.0f, diamond_min.y + diamondImg->GetHeight() * 1.0f };
		drawlist->AddImage((void*)static_cast<size_t>(diamondImg->GetID()), diamond_min, diamond_max, ImVec2{ 0,0 }, ImVec2{ 1,1 }, step >= 1 ? IM_COL32(250, 196, 130, 255) : ImGui::GetColorU32({ 1,1,1,1 }));
	}
}

