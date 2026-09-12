#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandSuperRun : public CommandSliderFloat
	{
	public:
		explicit CommandSuperRun(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Super Run"), CMDNAMES("superrun"), NOLABEL, -1000000, 1000000, 100)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_MOVE_RATE_OVERRIDE(ped.GetHandle(), getFloatValue());
		}

		~CommandSuperRun() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
