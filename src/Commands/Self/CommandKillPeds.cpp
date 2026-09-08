#include "Commands/CommandLegacy.hpp"
#include "Game/Pools.hpp"

namespace Stand::Features
{
	class KillAll : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto ped : Pools::GetPeds())
			{
				if (!ped.IsPlayer())
					ped.Kill();
			}
		}
	};

	class KillAllEnemies : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto ped : Pools::GetPeds())
			{
				if (!ped.IsPlayer() && ped.IsEnemy())
					ped.Kill();
			}
		}
	};

	static KillAll _KillAll{"killallpeds", "Kill All Peds", "Kills all peds in the game world"};
	static KillAllEnemies _KillAllEnemies{"killallenemies", "Kill All Enemies", "Kills all enemies in the game world"};
}