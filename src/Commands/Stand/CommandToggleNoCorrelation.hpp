#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

// Real Stand's CommandToggleNoCorrelation also carries chat-command
// syntax help, a web-command sync (updateWebState, sent over its own
// RelayCon bridge), and localized on/off response sentences built from
// its own Lang database. None of that exists here. The actual toggle
// logic - onClick() flips m_on then dispatches onEnable()/onDisable(),
// exactly the same OnEnable()/OnDisable() split this project's own
// CommandToggle already uses - is ported faithfully, since that's the
// part that matters.
namespace Stand
{
	class CommandToggleNoCorrelation : public CommandPhysical
	{
	public:
		const bool default_on;
		bool m_on;

		explicit CommandToggleNoCorrelation(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, bool default_on = false, commandflags_t flags = CMDFLAGS_TOGGLE, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}, CommandType type = COMMAND_TOGGLE) :
		    CommandPhysical(type, parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, perm, default_hotkeys),
		    default_on(default_on),
		    m_on(default_on)
		{
		}

		void onClick(Click& click) override;

		// "Command: <name> [on/off]" - real Stand's own
		// CommandToggleNoCorrelation::getCommandSyntax() override
		// (confirmed against origin/stand-reference), appending " [on/off]"
		// onto CommandPhysical's own base "Command: <name>".
		[[nodiscard]] std::string getCommandSyntax() const override;

		virtual void onChange(Click& click);
		virtual void onEnable(Click& click);
		virtual void onDisable(Click& click);

		std::string getState() const override;
		std::string getDefaultState() const override;
		void setState(Click& click, const std::string& state) override;
		void applyDefaultState() override;

		void setStateBool(Click& click, bool toggle);
		void toggleState(Click& click);
		void updateState(Click& click);
	};
}
