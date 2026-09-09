#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/vector.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRespawnRecall : public CommandToggle
	{
		rage::fvector3 m_DeathPos{};
		bool m_HasDeathPos = false;

	public:
		explicit CommandRespawnRecall(CommandList* const parent)
			: CommandToggle(parent, LIT("Respawn At Place Of Death"), { CMDNAME("respawnrecall") }, LIT("Returns you to where you died after respawning."))
		{
		}

		void onEnable(Click& click) override
		{
			m_HasDeathPos = false;
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			m_HasDeathPos = false;
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			if (!m_HasDeathPos)
			{
				if (ped.IsDead())
				{
					m_DeathPos = ped.GetPosition();
					m_HasDeathPos = true;
				}
				return;
			}

			if (!ped.IsDead())
			{
				ped.SetPosition(m_DeathPos);
				m_HasDeathPos = false;
			}
		}

		~CommandRespawnRecall() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
