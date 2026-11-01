#include "Commands/Widgets/CommandSchemeKey.hpp"

#include "Menu/ClickType.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	void CommandSchemeKey::onClick(Click& click)
	{
		if (click.type == CLICK_HOTKEY)
		{
			click.setNoResponse();
			return;
		}
		openHotkeysList(click.thread_context);
		click.setNoResponse();
	}

	const std::vector<Hotkey>& CommandSchemeKey::getDefaultHotkeys() const noexcept
	{
		return default_hotkeys;
	}

	void CommandSchemeKey::onHotkeysChanged(ClickType type) const
	{
		*scheme_key = hotkeys;
	}

	void CommandSchemeKey::setHotkeysFromScheme()
	{
		hotkeys = *scheme_key;
		g_gui.hotkeys.data[getPathConfig()] = hotkeys;
	}
}
