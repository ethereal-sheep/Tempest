/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "UIElements.h"


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

	bool ConfirmDeletePopup(const char* str)
	{
		bool ret = false;

		if (ImGui::Button(str))
		{
			ImGui::OpenPopup(str);
		}

		if (ImGui::BeginPopupModal(str, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Confirm Deletion?\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0.f)))
			{
				ret = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120.f, 0.f)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return ret;
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

	void UniformScaleFloat2(const char* label, const char* ID, ImVec2 padding, bool* b, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
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

}

