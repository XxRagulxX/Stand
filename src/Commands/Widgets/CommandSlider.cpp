#include "Commands/Widgets/CommandSlider.hpp"

#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Menu/Click.hpp"

#include <stdexcept>

namespace Stand
{
	bool CommandSlider::onLeft(Click& click, bool holding)
	{
		bool ret;
		int value = this->value;
		const bool was_above_default_value = holding && value > default_value;
		value -= static_cast<int>(step_size);
		if (was_above_default_value && value <= default_value)
		{
			value = default_value;
			ret = false;
		}
		else if (value < min_value)
		{
			if (holding || value + static_cast<int>(step_size) != min_value)
			{
				value = min_value;
				ret = false;
			}
			else
			{
				value = max_value;
				ret = true;
			}
		}
		else
		{
			ret = true;
		}
		if (this->value != value)
			updateValue(click, value);
		return ret;
	}

	bool CommandSlider::onRight(Click& click, bool holding)
	{
		bool ret;
		int value = this->value;
		const bool was_below_default_value = holding && value < default_value;
		value += static_cast<int>(step_size);
		if (was_below_default_value && value >= default_value)
		{
			value = default_value;
			ret = false;
		}
		else if (value > max_value || value < this->value)
		{
			if (holding || value - static_cast<int>(step_size) != max_value)
			{
				value = max_value;
				ret = false;
			}
			else
			{
				value = min_value;
				ret = true;
			}
		}
		else
		{
			ret = true;
		}
		if (this->value != value)
			updateValue(click, value);
		return ret;
	}

	void CommandSlider::updateValue(Click& click, int value)
	{
		ensureScriptThread(click, [this, value](Click& click) mutable {
			const int prev_value = this->value;
			this->value = value;
			updateState(click);
			onChange(click, prev_value);
		});
	}

	void CommandSlider::setValue(Click& click, int value)
	{
		if (value < min_value)
			value = min_value;
		else if (value > max_value)
			value = max_value;
		if (this->value != value)
		{
			const int prev_value = this->value;
			this->value = value;
			updateState(click);
			onChange(click, prev_value);
		}
	}

	std::string CommandSlider::getState() const
	{
		return std::to_string(value);
	}

	std::string CommandSlider::getDefaultState() const
	{
		return std::to_string(default_value);
	}

	void CommandSlider::setState(Click& click, const std::string& state)
	{
		int updated_value = default_value;
		if (!state.empty())
		{
			for (const auto& [intVal, strName] : getNamedValues())
			{
				if (state == strName)
				{
					setValue(click, intVal);
					return;
				}
			}
			try
			{
				updated_value = std::stoi(state);
			}
			catch (const std::exception&)
			{
			}
		}
		setValue(click, updated_value);
	}

	void CommandSlider::applyDefaultState()
	{
		if (value != default_value)
		{
			const int prev_value = value;
			value = default_value;
			Click click(CLICK_BULK, TC_APPLYDEFAULTSTATE);
			updateState(click);
			onChange(click, prev_value);
		}
	}

	void CommandSlider::updateState(const Click& click)
	{
		// Every path that actually changes value (updateValue() from a
		// real onLeft()/onRight() click, setValue() from setState()/
		// applyDefaultState()) funnels through here - see
		// CommandStateSerializer.hpp's own class comment.
		CommandStateSerializer::MarkDirty();
	}
}
