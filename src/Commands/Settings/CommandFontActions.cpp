#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"

#include <commdlg.h>
#include <windows.h>

namespace Stand::Features
{
	static StandWidgets::CommandLambdaAction _SetFont{"setfont",
	    "Set Font",
	    "Opens a file picker to load a custom .spritefont file.",
	    [] {
		    OPENFILENAMEA ofn{};
		    char filePath[MAX_PATH]{};
		    ofn.lStructSize = sizeof(ofn);
		    ofn.lpstrFilter = "SpriteFont Files\0*.spritefont\0All Files\0*.*\0\0";
		    ofn.lpstrFile = filePath;
		    ofn.nMaxFile = MAX_PATH;
		    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		    if (GetOpenFileNameA(&ofn))
		    {
			    Rendering::Theme::kFontPath = filePath;
			    Rendering::Theme::kFontReloadPending = true;
		    }
	    }};

	static StandWidgets::CommandLambdaAction _PresetFontYahei{"presetfontyahei",
	    "Microsoft YaHei",
	    "Stand's default font.",
	    [] {
		    Notifications::Show("StandEnhanced", "Microsoft YaHei font is not bundled in this build.");
	    }};

	static StandWidgets::CommandLambdaAction _PresetFontNanumGothic{"presetfontnanumgothic",
	    "Nanum Gothic",
	    "Stand's auxiliary font for Korean.",
	    [] {
		    Notifications::Show("StandEnhanced", "Nanum Gothic font is not bundled in this build.");
	    }};

	static StandWidgets::CommandLambdaAction _PresetFontBeVietnamProLight{"presetfontbevietnamprolight",
	    "Be Vietnam Pro Light",
	    "Stand's auxiliary font for Vietnamese.",
	    [] {
		    Rendering::Theme::kFontPath = "";
		    Rendering::Theme::kFontReloadPending = true;
	    }};

	static StandWidgets::CommandLambdaAction _ReloadFont{"reloadfont",
	    "Reload Font",
	    "Reloads the current font.",
	    [] {
		    Rendering::Theme::kFontReloadPending = true;
	    }};
}
