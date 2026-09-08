#include "Commands/Widgets/Position2dCommands.hpp"
#include "Rendering/NotifySettings.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNotifyPosition (Commands/Online/
	// CommandListNotifySettings.cpp on origin/stand-reference) - only
	// takes effect while NotifySettings::kType == StandCustomPosition
	// (see GetAnchor() in Notifications.cpp). Reuses the same generic
	// CommandPositionAxis/CommandPosition2dMouse widgets Settings >
	// Appearance > Position already uses for the menu's own position
	// (src/Commands/Widgets/Position2dCommands.hpp) rather than
	// duplicating CommandMenuPosition.cpp's own hand-written X/Y/
	// MouseMove trio a second time.
	static StandWidgets::CommandPositionAxis _NotifyPositionX{"notifyx",
	    "X",
	    "The custom position's horizontal coordinate.",
	    &Rendering::NotifySettings::kCustomPositionX,
	    Rendering::NotifySettings::kCustomPositionX};

	static StandWidgets::CommandPositionAxis _NotifyPositionY{"notifyy",
	    "Y",
	    "The custom position's vertical coordinate.",
	    &Rendering::NotifySettings::kCustomPositionY,
	    Rendering::NotifySettings::kCustomPositionY};

	static StandWidgets::CommandPosition2dMouse _NotifyMouseMove{"notifymousemove",
	    "Move With Mouse",
	    "While on, moving the mouse drags the custom position with it - turn this back off to drop it in place.",
	    &_NotifyPositionX,
	    &_NotifyPositionY};
}
