#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	static StandWidgets::CommandLambdaAction _NotifyCopyPrimaryColour{"notifycopyprimary",
	    "Copy Primary Colour",
	    "Copies the current Primary Colour to the Border Colour.",
	    [] {
		    Rendering::NotifySettings::kBorderColour = Rendering::Theme::kAccent;
	    }};

	static StandWidgets::CommandLambdaAction _NotifyCopyBgColour{"notifycopybg",
	    "Copy Background Colour",
	    "Copies the current Background Colour to the notification's Background Colour.",
	    [] {
		    Rendering::NotifySettings::kBackgroundColour = Rendering::Theme::kPanelBackground;
	    }};

	static StandWidgets::CommandLambdaAction _NotifySampleSmall{"notifysamplesmall",
	    "Small",
	    "Shows a small sample notification.",
	    [] {
		    Notifications::Show("StandEnhanced", "Hi");
	    }};

	static StandWidgets::CommandLambdaAction _NotifySampleMedium{"notifysamplemedium",
	    "Medium",
	    "Shows a medium sample notification.",
	    [] {
		    Notifications::Show("StandEnhanced", "This is a medium sample notification.");
	    }};

	static StandWidgets::CommandLambdaAction _NotifySampleLarge{"notifysamplelarge",
	    "Large",
	    "Shows a large sample notification.",
	    [] {
		    Notifications::Show("StandEnhanced", "This is a large sample notification to help preview how longer notification messages look with your current width, padding, timing, and colour settings.");
	    }};
}
