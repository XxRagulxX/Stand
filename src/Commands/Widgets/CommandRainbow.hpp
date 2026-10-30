#pragma once

#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/StandPort/CommandColourCustom.hpp"
#include "Game/nullsub.hpp"
#include "Util/get_current_time_millis.hpp"

namespace Stand
{
	class CommandRainbow;

	using hue_change_handler_t = void(*)(CommandRainbow*);

	struct RainbowData
	{
		time_t ms_wait;
		time_t last_frame;
		time_t ms_passed = 0ll;
	};

	class CommandRainbow : public CommandSlider
	{
	protected:
		CommandColourCustom* const colour;
	private:
		hue_change_handler_t hue_change_handler;
		RainbowData data{};

	public:
		explicit CommandRainbow(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, CommandColourCustom* colour, hue_change_handler_t hue_change_handler = &nullsub<>)
			: CommandSlider(parent, std::move(menu_name), std::move(command_names), LOC("LGBT_H"), 0, 1000, 0, 1)
			, colour(colour)
			, hue_change_handler(hue_change_handler)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandRainbow()
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onChange(Click& click, int prev_value) final
		{
			if (value == 0)
			{
				colour->rainbow_active = false;
				data = {};
			}
			else
			{
				colour->rainbow_active = true;
				data.ms_wait = (long long)value;
			}
		}

		void onTick() override
		{
			if (value <= 0 || !colour->h)
				return;

			const time_t now = get_current_time_millis();
			if (data.last_frame == 0)
			{
				data.last_frame = now;
				return;
			}

			data.ms_passed += now - data.last_frame;
			data.last_frame = now;

			while (data.ms_passed >= data.ms_wait)
			{
				data.ms_passed -= data.ms_wait;
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				if (colour->h->value >= colour->h->max_value)
					colour->h->setValue(click, 0);
				else
					colour->h->setValue(click, colour->h->value + 1);
				hue_change_handler(this);
			}
		}
	};
}
