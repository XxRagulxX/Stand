#pragma once

#include "Commands/Widgets/CommandListWithOnTickAsActiveList.hpp"
#include "Commands/Stand/CommandListConcealer.hpp"

namespace Stand
{
	class CommandListSessionScripts : public CommandListWithOnTickAsActiveList<CommandListConcealer>
	{
	private:
		CommandDivider* running_scripts_divider;
		size_t running_scripts_offset;

	public:
		explicit CommandListSessionScripts(CommandList* const parent);

		void onTickAsActiveList() final;
	};
}
