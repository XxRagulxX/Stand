#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

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

		void onChangeToggleScriptTickEventHandler(Click& click, std::function<bool()>&& handler);
	};
}
