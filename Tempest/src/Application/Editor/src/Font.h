#pragma once
#include "Fonts/IconsFontaudio.h"
#include "Fonts/IconsFontAwesome5.h"
#include "Fonts/IconsForkAwesome.h"
#include "Fonts/IconsKenney.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/ImGuiFileBrowser.h"

//static const char* default_font = "FiraMono-Regular.ttf";
//static const char* bold_font = "FiraMono-Bold.ttf";

// make sure these files are in Resource/Fonts/
static const char* default_font = "FiraMono-Regular.ttf";
static const char* bold_font = "FiraMono-Bold.ttf";

static const float font_text_size = 15.f;
static const float font_icon_size = 13.f;

inline auto def() { return ImGui::GetIO().Fonts->Fonts[0]; }
inline auto bold() { return ImGui::GetIO().Fonts->Fonts[1]; }
inline auto fk() { return ImGui::GetIO().Fonts->Fonts[2]; }
inline auto fad() { return ImGui::GetIO().Fonts->Fonts[3]; }
inline auto ki() { return ImGui::GetIO().Fonts->Fonts[4]; }

#define FONT_DEFAULT	def()
#define FONT_BOLD		bold()
#define FONT_FK			fk()
#define FONT_FAD		fad()
#define FONT_KI			ki()

namespace Tempest
{
	
}