#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"

#include <windows.h>

namespace Stand
{
	class CommandStreamproof : public CommandToggle
	{
	public:
		explicit CommandStreamproof(CommandList* parent)
			: CommandToggle(parent, LOC("STRMPRF"), CMDNAMES("streamproof", "stream"), LOC("STRMPRF_H"))
		{
		}

		void onEnable(Click& click) final
		{
			if (Pointers.Hwnd && *Pointers.Hwnd)
				SetWindowDisplayAffinity(*Pointers.Hwnd, 0x00000011);
		}

		void onDisable(Click& click) final
		{
			if (Pointers.Hwnd && *Pointers.Hwnd)
				SetWindowDisplayAffinity(*Pointers.Hwnd, 0x00000000);
		}

		void applyDefaultState() final
		{
		}
	};
}
