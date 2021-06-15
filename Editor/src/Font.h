#pragma once
#include "Fonts/IconsFontaudio.h"
#include "Fonts/IconsFontAwesome5.h"
#include "Fonts/IconsForkAwesome.h"
#include "Fonts/IconsKenney.h"
#include "imgui/imgui.h"
#include "Core.h"

//static const char* default_font = "FiraMono-Regular.ttf";
//static const char* bold_font = "FiraMono-Bold.ttf";

// make sure these files are in Resource/Fonts/
static const char* default_font = "FiraMono-Regular.ttf";
static const char* bold_font = "FiraMono-Bold.ttf";


static ImFont*			def;
static ImFont*			bold;
static ImFont*			fk;
static ImFont*			fad;
static ImFont*			ki;

#define FONT_DEFAULT	def
#define FONT_BOLD		bold
#define FONT_FK			fk
#define FONT_FAD		fad
#define FONT_KI			ki

namespace Tempest
{
	void init_font()
	{
		auto io = ImGui::GetIO();
		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		config.PixelSnapH = true;

		tpath def_f = tpath("Fonts") / default_font;
		tpath bold_f = tpath("Fonts") / bold_font;
		def_f.replace_extension(".ttf");
		bold_f.replace_extension(".ttf");


		static const ImWchar fa_range[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), 15.0f); // change this to change default font
		io.Fonts->AddFontFromFileTTF("Fonts/fa-regular-400.ttf", 13.0f, &config, fa_range);
		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", 13.0f, &config, fa_range);
		sprintf_s<40>(config.Name, "Default");
		io.Fonts->AddFontDefault(&config);

		io.Fonts->AddFontFromFileTTF(bold_f.string().c_str(), 15.0f);

		static const ImWchar fk_range[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), 15.0f);
		io.Fonts->AddFontFromFileTTF("Fonts/forkawesome-webfont.ttf", 13.0f, &config, fk_range);
		io.Fonts->AddFontDefault(&config);

		static const ImWchar fad_range[] = { ICON_MIN_FAD, ICON_MAX_FAD, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), 15.0f);
		io.Fonts->AddFontFromFileTTF("Fonts/fontaudio.ttf", 13.0f, &config, fad_range);
		io.Fonts->AddFontDefault(&config);

		static const ImWchar ki_range[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
		io.Fonts->AddFontFromFileTTF(def_f.string().c_str(), 15.0f);
		io.Fonts->AddFontFromFileTTF("Fonts/kenney-icon-font.ttf", 13.0f, &config, ki_range);
		io.Fonts->AddFontDefault(&config);

		def = io.Fonts->Fonts[0];
		bold = io.Fonts->Fonts[1];
		fk = io.Fonts->Fonts[2];
		fad = io.Fonts->Fonts[3];
		ki = io.Fonts->Fonts[4];
	}
}