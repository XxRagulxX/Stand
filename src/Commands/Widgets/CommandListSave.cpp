#include "Commands/Widgets/CommandListSave.hpp"

#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Widgets/CommandListSaveAction.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	CommandListSave::CommandListSave(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& action_command_names)
		: CommandList(parent, std::move(menu_name)), action_command_names(action_command_names)
	{
	}

	void CommandListSave::populate()
	{
		this->createChild<CommandListSaveAction>(Label(menu_name), std::vector<CommandName>(action_command_names));
		this->createChild<CommandDivider>(LOC("SVESEL"));
		auto data_order = getDataOrderRaw();
		for (const auto& key : data_order)
		{
			if (canEntryBeOmitted(key))
			{
				this->createChild<CommandToggle>(LOC_RT(key), {}, NOLABEL, true, CMDFLAGS_TOGGLE | CMDFLAG_FEATURELIST_FINISHLIST);
			}
		}
	}

	bool CommandListSave::canEntryBeOmitted(hash_t key) const
	{
		return true;
	}

	bool CommandListSave::shouldOmitEntry(hash_t hash) const
	{
		auto* toggle = resolveChildByMenuNameHash(hash);
		return toggle != nullptr && !((CommandToggleNoCorrelation*)toggle)->m_on;
	}

	void CommandListSave::onSaved() const
	{
	}
}
