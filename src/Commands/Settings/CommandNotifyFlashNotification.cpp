#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/Notifications.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own "Flash Notification" (Commands/Online/
	// CommandListNotifySettings.cpp on origin/stand-reference) - real
	// Stand's own version calls GridToaster::flashPersistentToast(),
	// re-flashing the SAME persistent preview toast this settings page
	// already keeps alive (Notifications::SetPreviewActive(), driven by
	// GridRenderer.cpp while this page is open) rather than creating a
	// second, separate notification the way this button first shipped -
	// Notifications::FlashPreview() is this project's own equivalent,
	// and is itself a no-op while the preview isn't active, matching
	// real Stand's own guard on this button ("if (g_toaster ==
	// &g_grid_toaster)").
	static StandWidgets::CommandLambdaAction _NotifyFlashNotification{"notifyflashnow",
	    "Flash Notification",
	    "Re-flashes the preview notification above, demonstrating the Flash Colour/Border Colour transition.",
	    [] {
		    Notifications::FlashPreview();
	    }};
}
