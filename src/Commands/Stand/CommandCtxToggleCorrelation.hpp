#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandCtxToggleCorrelation : public CommandListSelect
	{
	public:
		CommandToggleNoCorrelation* invert = nullptr;

		explicit CommandCtxToggleCorrelation(CommandList* const parent, CommandToggle* const target);

		void updateHelpTexts(const CommandToggle* const toggle);

		void onChange(Click& click, long long prev_value) final;
	};
}
