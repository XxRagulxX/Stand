#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandSeatGlue : public CommandToggle
	{
	public:
		explicit CommandSeatGlue(CommandList* const parent)
			: CommandToggle(parent, LIT("Glued To Seats"), { CMDNAME("seatbelt"), CMDNAME("seatglue") }, LIT("Makes your character unable to fall off bikes."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;
			PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), true);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped.GetHandle(), 0);
			Stand::Self::GetPlayer().SetFallDistanceOverride(0.0f);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped || !Stand::Self::GetVehicle())
				return;
			PED::SET_PED_CAN_RAGDOLL(ped.GetHandle(), false);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped.GetHandle(), 1);
			Stand::Self::GetPlayer().SetFallDistanceOverride(1000.0f);
		}

		~CommandSeatGlue() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
