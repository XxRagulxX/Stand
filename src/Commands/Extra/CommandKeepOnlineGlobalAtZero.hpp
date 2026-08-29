#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/pointers.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	template <int... globals>
	class CommandKeepOnlineGlobalAtZero : public CommandToggle
	{
	public:
		using CommandToggle::CommandToggle;

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				if (*pointers::is_session_started)
				{
					constexpr int globals_arr[] = { globals... };
					for (const int global : globals_arr)
					{
						*ScriptGlobal(global).as<int*>() = 0;
					}
				}
			});
		}
	};
}
