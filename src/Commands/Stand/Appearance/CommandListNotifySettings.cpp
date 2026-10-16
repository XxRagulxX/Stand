#include "Commands/Stand/Appearance/CommandListNotifySettings.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

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
		Notifications::Show("StandEnhanced", "Here's some text to feast your eyes upon.", NotificationType::Info);
	}
}

namespace Stand::Features
{
	Stand::CommandListNotifySettings& GetCommandListNotifySettings()
	{
		return *GetCommandTabStand().notifications;
	}
}
