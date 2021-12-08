/**********************************************************************************
* \author		HuangXurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "TMath.h"
#include "Font.h"
#include <Tempest/src/Core.h>
#include <Tempest/src/ECS/Entity.h>
#include "Instance/Instance.h"

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

	void PaddedSeparator(float padding = 1.f);
	void PaddedSeparator(float up, float down);

	void DrawLine();

	void HelpMarker(const char* str);

	void Tooltip(const char* label, const char* str, bool enabled = true);

	bool ConfirmDeletePopup(const char* popupName, string str);
	bool ConfirmDeletePopup_DefineStat(const char* popupName);

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

	void SubHeader(const char* str);
	void Header_1(const char* str);
	void Header_2(const char* str);
	// Button size will Scale with Text
	bool UIButton_1(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	// Button size will not scale with Text
	// Default Btn Color
	bool UIButton_2(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	// Button size will not scale with Text
	// Blueish Button for Select Weapon Btn
	bool UIButton_Weapon(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	// Button size will not scale with Text
	// Greenish Button for Select Action Btn
	bool UIButton_Action(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	// Button size will not scale with Text
	// Beige Button for Select Sequence Btn
	bool UIButton_Sequence(Instance& instance, Entity id, string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	// Button size will not scale with Text
	// Beige Button for Select Sequence Btn
	bool UIButton_Simulate(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	bool UIButton_EndTurn(ImVec2 pos, ImVec2 padding, ImFont* font, bool selected = false);

	//return (IsButtonClick, IsDeleteClicked)
	std::pair<bool, bool> UIButtonWithDelete(string unselected, string id, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected);

	void AddUnderline(ImU32 col, ImVec2 min = { 0, 0 }, ImVec2 max = { 0, 0 });
	bool UISelectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool UISelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	std::pair<bool, bool> UIConflictSelectable(const char* label, bool selected = false, int type = 0,ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	std::pair<bool,bool> UIConflictSelectable(const char* label, bool* p_selected, int type = 0, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	std::pair<bool, bool> UIMapSelectable(const char* label, const char* date, bool selected = false, int type = 0, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	std::pair<bool, bool> UIMapSelectable(const char* label, const char* date, bool* p_selected, int type = 0, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool UIImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_hover = ImVec4(1, 1, 1, 1), const ImVec4& tint_pressed = ImVec4(1,1,1,1));
	bool UIImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col, const ImVec4& tint_pressed);
	bool UICheckBox_1(const char* label, bool* v);

	std::pair<bool, bool> UICharButton_WithDeleteEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	bool UICharButton_ArrowEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	bool UICharButton_NoDeleteEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	bool UICharButton_ToggleEx(ImGuiID id, ImTextureID texture_id, string label, const ImVec2& size, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	bool UIActionButtonEx (ImGuiID id, string actionName, bool selected, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	bool UICharTurnButtonEx (ImGuiID id, ImTextureID texture_id, string characterName, const ImVec2& size, bool selected, bool isBig, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
	/*
	user_texture_id - Id of texture
	size - Size of Img
	charName - Display Name for the Character
	label - ID of the delete Btn (MUST BE UNIQUE)
	*/
	std::pair<bool, bool> UICharButton_WithDelete(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = 2, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool UICharButton_Arrow(ImTextureID user_texture_id, const ImVec2& size, string charName, string label, bool selected = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = 2, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool UICharButton_NoDelete(ImTextureID user_texture_id, const ImVec2& size, string charName, string labelID, bool selected = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = 2, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool UICharButton_Toggle(ImTextureID user_texture_id, const ImVec2& size, string charName, string labelID, bool selected = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = 2, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool UIActionButton(string actionName, string labelID, bool selected = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	bool UICharTurnButton(ImTextureID user_texture_id, const ImVec2& size, string characterName, string labelID, bool selected = false, bool isBig = false, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	// Pos is from top left 
	void CharacterTurn(Instance& instance, Entity id, const ImVec2 pos, bool selected = false);
	// Pos is from top left 
	bool CharacterTurnData(Instance& instance, Entity id, const ImVec2 pos, bool isRightSide = false, bool isRoll = false);

	// Pos is from top right
	void ActionUI(const ImVec2 pos, string title);

	// Pos is from Middle of arrow 
	void AttackSuccessUI(string name, const ImVec2 pos, int successVal);

}

namespace Tempest
{
	static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
	static inline ImVec2 operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
	static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
	static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
	static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
	static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
	static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
	static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
	static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
	static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
	static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
	static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
	static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
	static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
	static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
}

