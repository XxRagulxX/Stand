#pragma once

#include "Commands/Widgets/CommandListWithOnTickAsActiveList.hpp"
#include "Commands/Stand/CommandListConcealer.hpp"

namespace Stand
{
	class CommandListHandling : public CommandListWithOnTickAsActiveList<CommandListConcealer>
	{
	private:
		CommandToggle* show_na;

	public:
		explicit CommandListHandling(CommandList* const parent);

		void onTickAsActiveList() final;
	};

	class CommandListSubhandling;
}
