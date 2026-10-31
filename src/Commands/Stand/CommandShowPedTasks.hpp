#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/AbstractEntity.hpp"

namespace Stand
{
	class CommandShowPedTasks : public CommandToggle
	{
	public:
		explicit CommandShowPedTasks(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Ped Tasks"), { CMDNAME("showpedtasks") })
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, [this]() -> bool
			{
				if (!m_on)
				{
					return false;
				}
				std::vector<AbstractEntity> vec;
				AbstractEntity::getAllPeds(vec);
				for (AbstractEntity& ent : vec)
				{
					(void)ent;
				}
				return true;
			});
		}
	};
}
