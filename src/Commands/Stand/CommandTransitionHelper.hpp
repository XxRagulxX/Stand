#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/TransitionHelper.hpp"

namespace Stand
{
	class CommandTransitionHelper : public CommandToggle
	{
	public:
		explicit CommandTransitionHelper(CommandList* const parent)
			: CommandToggle(parent, LOC("TRNSHLP"), CMDNAMES("transitionhelper"), LOC("TRNSHLP_H"), true)
		{
		}

		void onChange(Click& click) final
		{
			TransitionHelper::fix_stucks = m_on;
		}
	};
}
