#include "CommandTextsliderStateful.hpp"

#include "Exceptional.hpp"
#include "Gui.hpp"
#include "RelayCon.hpp"

namespace Stand
{
	CommandTextsliderStateful::CommandTextsliderStateful(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, std::vector<CommandTextsliderData>&& options, const commandflags_t flags, const CommandPerm perms)
		: CommandTextslider(parent, std::move(menu_name), std::move(command_names), std::move(help_text), std::move(options), flags, perms, COMMAND_TEXTSLIDER_STATEFUL)
	{
		if (hasOptions())
		{
			default_value = this->options.at(0).value;
		}
	}

	void CommandTextsliderStateful::onFocus(ThreadContext thread_context, Direction momentum)
	{
	}

	std::string CommandTextsliderStateful::getState() const
	{
		if (value == default_value)
		{
			return {};
		}
		return getCurrentValueLabel().getEnglishUtf8();
	}

	std::string CommandTextsliderStateful::getWebState() const
	{
		return getCurrentValueLabel().getWebString();
	}

	void CommandTextsliderStateful::setState(Click& click, const std::string& state)
	{
		if (state.empty())
		{
			applyDefaultState();
			return;
		}

		for (const auto& option : options)
		{
			if (state == option.label.getEnglishUtf8())
			{
				this->value = option.value;
				processValueChange(click);
				break;
			}
		}
	}

	void CommandTextsliderStateful::applyDefaultState()
	{
		this->value = default_value;
	}

	// Kept out of the lambda below, which holds EXCEPTIONAL_LOCK's __try: the
	// e_line/s_line std::string objects can't be built or destroyed in a frame
	// that also has a __try, so this only takes references to already-built ones.
	static void sendTextsliderStatefulLines(std::string& e_line, std::string& s_line)
	{
		EXCEPTIONAL_LOCK(g_relay.send_mtx)
		g_relay.sendLine(std::move(e_line));
		g_relay.sendLine(std::move(s_line));
		EXCEPTIONAL_UNLOCK(g_relay.send_mtx)
	}

	void CommandTextsliderStateful::processValueChange(const Click& click) const
	{
		CommandTextslider::processValueChange(click);

		if (click.type != CLICK_BULK
			&& supportsStateOperations()
			)
		{
			g_gui.on_command_state_change();
		}

		if (!click.isWeb()
			&& isActiveOnWeb()
			)
		{
			Exceptional::createManagedExceptionalThread(__FUNCTION__, [this]
			{
				auto e_line = std::string("e ").append(menu_name.getWebString());
				auto s_line = std::string("s ").append(getWebState());
				sendTextsliderStatefulLines(e_line, s_line);
			});
		}
	}
}
