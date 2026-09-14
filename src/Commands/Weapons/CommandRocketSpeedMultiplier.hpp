#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <unordered_set>

namespace Stand
{
	class CommandRocketSpeedMultiplier : public CommandSliderFloat
	{
		std::unordered_set<int> m_handled;

		static constexpr Hash kRocketHashes[] = {
			"WEAPON_RPG"_J,
			"WEAPON_HOMINGLAUNCHER"_J,
			"WEAPON_FIREWORK"_J,
			"WEAPON_GRENADELAUNCHER"_J,
			"WEAPON_GRENADELAUNCHER_SMOKE"_J,
		};

	public:
		explicit CommandRocketSpeedMultiplier(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Rocket Speed Multiplier"), CMDNAMES("rocketspeedmultiplier"),
			                     NOLABEL,
			                     0, 3000, 100, 5)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (value == 100)
			{
				m_handled.clear();
				return;
			}

			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const int   pedHandle = ped.GetHandle();
			const float mult      = static_cast<float>(value) / 100.0f;

			for (Hash wh : kRocketHashes)
			{
				Vector3 outCoords{};
				int     proj = 0;
				if (MISC::GET_PROJECTILE_OF_PROJECTILE_TYPE_WITHIN_DISTANCE(
				        pedHandle, wh, 10000.0f, &outCoords, &proj, TRUE)
				    && proj != 0
				    && !m_handled.contains(proj))
				{
					m_handled.insert(proj);
					const Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(proj);
					ENTITY::SET_ENTITY_VELOCITY(proj,
					    vel.x * mult, vel.y * mult, vel.z * mult);
				}
			}
		}

		~CommandRocketSpeedMultiplier() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
