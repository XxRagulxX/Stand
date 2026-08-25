#include "Commands/Stand/CommandListQrcode.hpp"

#include "Menu/MenuGrid.hpp"

namespace Stand
{
	void CommandListQrcode::onActiveListUpdate()
	{
		if (isCurrentUiList())
		{
			g_menu_grid.qrcode = getText();
		}
		else
		{
			g_menu_grid.qrcode.clear();
		}
	}
}
