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
	class CommandReducedCollision : public CommandToggle
	{
	public:
		explicit CommandReducedCollision(CommandList* const parent)
			: CommandToggle(parent, LIT("Reduced Collision"), CMDNAMES("reducedcollision"), LIT("Enables walking through walls and vehicles."))
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
				PED::SET_PED_CAPSULE(ped.GetHandle(), 0.25f);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_CAPSULE(ped.GetHandle(), 1.175494e-38f);
		}

		~CommandReducedCollision() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
