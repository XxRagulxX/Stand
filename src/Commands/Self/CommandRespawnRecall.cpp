#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandRespawnRecall: remembers where
	// you died, then - once you're alive again - warps you back there
	// instead of leaving you at whatever spawn point the game picked.
	class RespawnRecall : public LoopedCommand
	{
		rage::fvector3 m_DeathPos{};
		bool m_HasDeathPos = false;

		using LoopedCommand::LoopedCommand;

		virtual void OnEnable() override
		{
			m_HasDeathPos = false;
		}

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			if (!m_HasDeathPos)
			{
				if (Self::GetPed().IsDead())
				{
					m_DeathPos = Self::GetPed().GetPosition();
					m_HasDeathPos = true;
				}
				return;
			}

			if (!Self::GetPed().IsDead())
			{
				Self::GetPed().SetPosition(m_DeathPos);
				m_HasDeathPos = false;
			}
		}
	};

	static RespawnRecall _RespawnRecall{"respawnrecall", "Respawn Recall", "Returns you to where you died after respawning"};
}
