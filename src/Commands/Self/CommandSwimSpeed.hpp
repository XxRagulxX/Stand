#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandSwimSpeed : public CommandSliderFloat
	{
	public:
		explicit CommandSwimSpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Swim Speed"), CMDNAMES("swimspeed"), NOLABEL, -1000000, 1000000, 100)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (auto player = Stand::Self::GetPlayer())
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player.GetId(), getFloatValue());
		}

		~CommandSwimSpeed() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
