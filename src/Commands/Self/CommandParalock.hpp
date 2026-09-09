#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Ped/PedConfigFlag.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandParalock : public CommandSlider
	{
	public:
		explicit CommandParalock(CommandList* const parent)
			: CommandSlider(parent, LIT("Lock Parachutes"), { CMDNAME("paralock") }, LIT("Force your parachute availability on or off."), 0, 2, 0)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			if (value == 1) return "Full";
			if (value == 2) return "Empty";
			return "Off";
		}

		[[nodiscard]] std::vector<std::pair<int, std::string>> getNamedValues() const override
		{
			return {{0, "off"}, {1, "full"}, {2, "empty"}};
		}

		void onChange(Click& click, int prev_value) final
		{
			if (value == 1)
			{
				CommandTickDispatch::AddCommand(this);
			}
			else
			{
				CommandTickDispatch::RemoveCommand(this);
				if (value == 2)
				{
					auto ped = Stand::Self::GetPed();
					if (ped)
					{
						ped.RemoveWeapon("GADGET_PARACHUTE"_J);
						ped.SetConfigFlag(PedConfigFlag::HasReserveParachute, false);
					}
				}
			}
		}

		void onTick() override
		{
			PLAYER::SET_PLAYER_HAS_RESERVE_PARACHUTE(Stand::Self::GetPlayer().GetId());
			auto ped = Stand::Self::GetPed();
			if (ped && !ped.HasWeapon("GADGET_PARACHUTE"_J))
				ped.GiveWeapon("GADGET_PARACHUTE"_J);
		}

		~CommandParalock() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
