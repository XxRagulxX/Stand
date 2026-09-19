#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRangeMultiplier : public CommandSliderFloat
	{
	public:
		explicit CommandRangeMultiplier(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Range Multiplier"), CMDNAMES("rangemultiplier"),
			                     NOLABEL,
			                     0, 150, 100, 5)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (value == 100)
				return;

			const auto player = Stand::Self::GetPlayer().GetId();
			PLAYER::SET_PLAYER_LOCKON_RANGE_OVERRIDE(player, static_cast<float>(value));
		}

		~CommandRangeMultiplier() override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (value != 100)
				PLAYER::SET_PLAYER_LOCKON_RANGE_OVERRIDE(Stand::Self::GetPlayer().GetId(), -1.0f);
		}
	};
}
