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

static const float font_text_size = 15.f;
static const float font_icon_size = 13.f;


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
	
}