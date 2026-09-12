#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWalkSpeed : public CommandSliderFloat
	{
	public:
		explicit CommandWalkSpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Walk And Run Speed"), CMDNAMES("walkspeed"), NOLABEL, -1000000, 1000000, 100)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_MOVE_RATE_OVERRIDE(ped.GetHandle(), getFloatValue());
		}

		~CommandWalkSpeed() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
