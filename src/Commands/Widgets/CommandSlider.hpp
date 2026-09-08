#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

// Real Stand's CommandSlider also carries a command-box prefill/parse
// path (openCommandBoxWithPrefill/onCommand), chat-syntax help, and a
// web-relay state sync (updateState's g_relay.sendLine calls). None of
// that exists here. onLeft()/onRight()'s actual step math - the
// default-value snap and min/max wraparound-while-holding behaviour -
// is ported verbatim, since that's real, load-bearing logic, not display
// plumbing.
namespace Stand
{
	class CommandSlider : public CommandPhysical
	{
	public:
		int min_value;
		int max_value;
		int value;
		int default_value;
		unsigned int step_size;

		explicit CommandSlider(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, int min_value, int max_value, int default_value, unsigned int step_size = 1, commandflags_t flags = CMDFLAGS_SLIDER, CommandPerm perm = COMMANDPERM_USERONLY, const std::vector<Hotkey>& default_hotkeys = {}, CommandType type = COMMAND_SLIDER) :
		    CommandPhysical(type, parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, perm, default_hotkeys),
		    min_value(min_value),
		    max_value(max_value),
		    value(default_value),
		    default_value(default_value),
		    step_size(step_size)
		{
		}

		[[nodiscard]] bool isNotApplicable() const noexcept
		{
			return min_value == max_value;
		}

		[[nodiscard]] virtual float getFloatValue() const
		{
			return static_cast<float>(value);
		}

		bool onLeft(Click& click, bool holding) override;
		bool onRight(Click& click, bool holding) override;

		void setValue(Click& click, int value);

		std::string getState() const override;
		std::string getDefaultState() const override;
		void setState(Click& click, const std::string& state) override;
		void applyDefaultState() override;

		virtual void onChange(Click& click, int prev_value)
		{
		}

		void setMinValue(int min_value)
		{
			this->min_value = min_value;
		}

		void setMaxValue(int max_value)
		{
			this->max_value = max_value;
		}

		void setStepSize(unsigned int step_size)
		{
			this->step_size = step_size;
		}

	private:
		void updateValue(Click& click, int value);
		void updateState(const Click& click);
	};
}
