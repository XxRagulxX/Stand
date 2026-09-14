#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Network/Players.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandInstantProxyDetonate : public CommandToggle
	{
	public:
		explicit CommandInstantProxyDetonate(CommandList* const parent)
			: CommandToggle(parent, LIT("Instant Proximity Mine Detonation"),
			                CMDNAMES("instantproxy"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto selfPed = Stand::Self::GetPed();
			if (!selfPed) return;
			const int selfPedHandle = selfPed.GetHandle();

			for (auto& [id, plyr] : Players::GetPlayers())
			{
				if (plyr == Stand::Self::GetPlayer()) continue;
				auto ped = plyr.GetPed();
				if (!ped.IsValid()) continue;

				const Vector3 pos = ped.GetPosition();
				if (MISC::IS_PROJECTILE_TYPE_WITHIN_DISTANCE(pos.x, pos.y, pos.z,
				        "WEAPON_PROXMINE"_J, 3.0f, TRUE))
				{
					WEAPON::EXPLODE_PROJECTILES(selfPedHandle, "WEAPON_PROXMINE"_J, TRUE);
					return;
				}
			}
		}
	};
}
