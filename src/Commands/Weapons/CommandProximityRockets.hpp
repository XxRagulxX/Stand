#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
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
	class CommandProximityRocketsToggle : public CommandToggle
	{
	public:
		CommandSlider* range{};

	private:
		static constexpr Hash kProjectileWeapons[] = {
			"WEAPON_RPG"_J,
			"WEAPON_HOMINGLAUNCHER"_J,
			"WEAPON_FIREWORK"_J,
			"WEAPON_GRENADELAUNCHER"_J,
			"WEAPON_GRENADELAUNCHER_SMOKE"_J,
		};

	public:
		explicit CommandProximityRocketsToggle(CommandList* parent)
			: CommandToggle(parent, LIT("Proximity Rockets"), CMDNAMES("proxyrockets"),
			                LIT("Proximity rockets only work on other players."))
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
			const float r = range ? static_cast<float>(range->value) / 100.0f : 3.0f;

			for (auto& [id, plyr] : Players::GetPlayers())
			{
				if (plyr == Stand::Self::GetPlayer())
					continue;

				auto ped = plyr.GetPed();
				if (!ped.IsValid())
					continue;

				const Vector3 pos = ped.GetPosition();

				for (Hash wh : kProjectileWeapons)
				{
					if (MISC::IS_PROJECTILE_TYPE_WITHIN_DISTANCE(pos.x, pos.y, pos.z, wh, r, TRUE))
					{
						FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 10.0f, true, false, 1.0f, false);
						break;
					}
				}
			}
		}

		~CommandProximityRocketsToggle() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};

	class CommandProximityRockets : public CommandList
	{
	public:
		CommandProximityRocketsToggle* const toggle;
		CommandSlider*                 const range;

		explicit CommandProximityRockets(CommandList* parent)
			: CommandList(parent, LIT("Proximity Rockets"), CMDNAMES("proxyrocketslist")),
			  toggle(createChild<CommandProximityRocketsToggle>()),
			  range(createChild<CommandSlider>(
			      LIT("Range"), CMDNAMES("proxyrange"),
			      LIT("Detonation radius in metres (0.50 to 5.00)."),
			      50, 500, 300, 10))
		{
			toggle->range = range;
		}
	};
}
