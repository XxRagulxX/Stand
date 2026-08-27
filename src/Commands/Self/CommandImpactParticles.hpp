#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractEntity.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Rendering/Particles.hpp"

namespace Stand
{
	class CommandImpactParticles : public CommandToggle
	{
	public:
		inline static CommandListSelect* particle = nullptr;

		explicit CommandImpactParticles(CommandList* const parent)
			: CommandToggle(parent, LOC("IMPPART"), { CMDNAME("impactparticles") }, NOLABEL, false, CMDFLAGS_TOGGLE | CMDFLAG_TUTORIAL_CTX_POINT)
		{
		}

		void onEnable(Click& click) final
		{
			registerScriptTickEventHandler([this]
			{
				Vector3 coords;

				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(g_player_ped, &coords))
				{
					Particles::tryPlay(this->particle->value, std::move(coords));
				}

				return this->m_on;
			});
		}
	};
}