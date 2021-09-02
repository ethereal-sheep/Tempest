#pragma once
#include "TMath.h"
#include "Font.h"
#include <Tempest/src/Core.h>

namespace Tempest::UI
{
	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	inline std::string CreateLabelWithId(std::string label, T a)
	{
		return label + "##" + std::to_string(a);
	}

	template <>
	inline std::string CreateLabelWithId<std::string, void>(std::string label, std::string a)
	{
		return label + "##" + a;
	}

	void PaddedSeparator(float padding = 0.f);
	void PaddedSeparator(float up, float down);

	void DrawLine();

	void HelpMarker(const char* str);

	void Tooltip(const char* label, const char* str, bool enabled = true);

	bool ConfirmDeletePopup(const char* str);

	void ShowLabel(const char* label, ImColor color);

	bool Selectable(const char* label, const char* ID, bool selected = false, ImGuiSelectableFlags flags = ImGuiSelectableFlags_None, ImVec2 size = ImVec2{ 0, 0 });

	void RenderText(const std::string& label, float padding);

	// Draw DragFloat3 with Text Display on the left
	// Return <True the moment value is change, True only when after edit>
	std::pair<bool, bool> DragFloat3(const char* label, const char* ID, ImVec2 padding, float v[3], float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	// Draw DragFloat2 with Text Display on the left
	// Return <True the moment value is change, True only when after edit>
	std::pair<bool, bool> DragFloat2(const char* label, const char* ID, ImVec2 padding, float v[2], float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	// Draw DragFloat with Text Display on the left
	// Return <True the moment value is change, True only when after edit>
	std::pair<bool, bool> DragFloat(const char* label, const char* ID, ImVec2 padding, float* val, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	// Draw DragInt with Text Display on the left
	// Return <True the moment value is change, True only when after edit>
	std::pair<bool, bool> DragInt(const char* label, const char* ID, ImVec2 padding,
		int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> DragInt2(const char* label, const char* ID, ImVec2 padding,
		int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> DragInt3(const char* label, const char* ID, ImVec2 padding,
		int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

	// Draw DragFloat3 with X Y Z Colored button
	// Text Display will be on the Left
	// Pressing the button will reset the value
	std::pair<bool, bool> DragFloat3ColorBox(const char* label, const char* ID, ImVec2 padding, float v[3], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	
	// Draw DragFloat3 with X Y Z Colored button
	// Text Display will be on the Left
	// Pressing the button will reset the value
	std::pair<bool, bool> DragFloat2ColorBox(const char* label, const char* ID, ImVec2 padding, float v[2], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	// Draw DragFloat3 with X Y Z Colored button
	// No Text Display
	// Pressing the button will reset the value
	std::pair<bool, bool> DragFloat3ColorBox_NoText(const char* ID, float v[3], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	// Draw DragFloat2 with X YColored button
	// No Text Display
	// Pressing the button will reset the value to 0
	std::pair<bool, bool> DragFloat2ColorBox_NoText(const char* ID, float v[2], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> UniformScaleFloat3(const char* label, const char* ID, ImVec2 padding, bool* b, float v[3], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	void UniformScaleFloat2(const char* label, const char* ID, ImVec2 padding, bool* b, float v[2], float resetValue = 0, float v_speed = 1.0f,
		float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderInt(const char* label, const char* ID, ImVec2 padding, int* v,
		int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderInt2(const char* label, const char* ID, ImVec2 padding, int v[2],
		int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderInt3(const char* label, const char* ID, ImVec2 padding, int v[3],
		int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderFloat(const char* label, const char* ID, ImVec2 padding, float* v,
		float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderFloat2(const char* label, const char* ID, ImVec2 padding, float v[2],
		float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> SliderFloat3(const char* label, const char* ID, ImVec2 padding, float v[3],
		float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	std::pair<bool, bool> InputFloat(const char* label, const char* ID, ImVec2 padding, float* v,
		float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);

	std::pair<bool, bool> ColorEdit4(const char* label, const char* ID, ImVec2 padding, float col[4], ImGuiColorEditFlags flags = 0);
	
	std::pair<bool, bool> ColorEdit3(const char* label, const char* ID, ImVec2 padding, float col[3], ImGuiColorEditFlags flags = 0);

	// Draw Checkbox
	// Text Display will be on the Left
	bool Checkbox(const char* label, const char* ID, ImVec2 padding, bool* val);

	// Draw InputText
	// Text Display will be on the Left
	bool InputText(const char* label, const char* ID, ImVec2 padding, std::string* str, ImGuiInputTextFlags flags = 0,
			ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

	void SubHeader(ImVec2 padding, const char* str);
	void Header(ImVec2 padding, const char* str);
	bool UIButton_1(string unselected, string hover,ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);
	void AddUnderline(ImU32 col, ImVec2 min = { 0, 0 }, ImVec2 max = { 0, 0 });
	bool UISelectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool UISelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

}

