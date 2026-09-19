#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandFriction : public CommandToggle
	{
	public:
		explicit CommandFriction(CommandList* const parent)
			: CommandToggle(parent, LIT("Increased Friction"), CMDNAMES("friction"), LIT("Reduces the slipperiness when turning with a high walk/run speed."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_MIN_MOVE_BLEND_RATIO(ped.GetHandle(), 0.0f);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_MIN_MOVE_BLEND_RATIO(ped.GetHandle(), 1.0f);
		}

		~CommandFriction() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
