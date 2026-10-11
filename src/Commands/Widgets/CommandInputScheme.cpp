#include "Commands/Widgets/CommandInputScheme.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandInputScheme::CommandInputScheme(CommandList* const parent)
		: CommandList(parent, LOC("KSCHM"), { CMDNAME("inputscheme"), CMDNAME("keyboardinputscheme") })
	{
	}

	void CommandInputScheme::setHotkeysFromScheme()
	{
		if (key_menu_open_close) key_menu_open_close->setHotkeysFromScheme();
		if (key_menu_root_up) key_menu_root_up->setHotkeysFromScheme();
		if (key_menu_root_down) key_menu_root_down->setHotkeysFromScheme();
		if (key_menu_up) key_menu_up->setHotkeysFromScheme();
		if (key_menu_down) key_menu_down->setHotkeysFromScheme();
		if (key_menu_left) key_menu_left->setHotkeysFromScheme();
		if (key_menu_right) key_menu_right->setHotkeysFromScheme();
		if (key_menu_click) key_menu_click->setHotkeysFromScheme();
		if (key_menu_back) key_menu_back->setHotkeysFromScheme();
		if (key_menu_context) key_menu_context->setHotkeysFromScheme();
		if (key_command_box) key_command_box->setHotkeysFromScheme();
	}

	void CommandInputScheme::onSchemeUpdate()
	{
		setHotkeysFromScheme();
		g_gui.hotkeys.save();
	}
}
