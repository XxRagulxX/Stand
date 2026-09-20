#include "Commands/Settings/Appearance/CommandTabNotifications.hpp"

#include "Rendering/NotifySettings.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	void CommandNotifyCopyPrimary::onClick(Click& click)
	{
		const auto& src = Rendering::Theme::kAccent;
		Rendering::NotifySettings::kBorderColour = {src.x, src.y, src.z, src.w};
	}

	void CommandNotifyCopyBg::onClick(Click& click)
	{
		const auto& src = Rendering::Theme::kPanelBackground;
		Rendering::NotifySettings::kBackgroundColour = {src.x, src.y, src.z, src.w};
	}

	void CommandNotifyFlashNow::onClick(Click& click)
	{
		Notifications::FlashPreview();
	}
}

namespace Stand::Features
{
	Stand::CommandTabNotifications& GetCommandTabNotifications()
	{
		static Stand::CommandTabNotifications instance{};
		return instance;
	}
}
