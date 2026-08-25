#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "lib/soup/Bitset.hpp"

#include "Network/punishments.hpp"

namespace Stand
{
	class CommandPunishment : public CommandToggle
	{
	private:
		soup::Bitset<punishment_t>* const bitset;
		const punishment_t mask;

	public:
		explicit CommandPunishment(CommandList* const parent, soup::Bitset<punishment_t>* const bitset, const Punishment& p, std::vector<CommandName>&& command_names = {})
			: CommandToggle(parent, Label(p.name), std::move(command_names), Label(p.help_text)), bitset(bitset), mask(p.mask)
		{
		}

		Label getActivationName() const final
		{
			return getActivationNameImplCombineWithParent();
		}

		void onChange(Click& click) final
		{
			bitset->setMask(mask, m_on);
		}
	};
}
