#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/vector.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Particles.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <string_view>

namespace Stand
{
	class CommandImpactParticles : public CommandToggle
	{
		static int GetParticleIndex()
		{
			static const int idx = []() {
				for (int i = 0; i < Particles::count; ++i)
				{
					if (std::string_view(Particles::particles[i].rawName) == "exp_grd_plane")
						return i;
				}
				return 0;
			}();
			return idx;
		}

	public:
		explicit CommandImpactParticles(CommandList* const parent)
			: CommandToggle(parent, LIT("Impact Particles"), CMDNAMES("impactparticles"), LIT("Plays a particle effect at the point where your bullets hit."))
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
			if (auto ped = Stand::Self::GetPed())
			{
				Vector3 coords{};
				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped.GetHandle(), &coords))
					Particles::tryPlay(GetParticleIndex(), {coords.x, coords.y, coords.z});
			}
		}

		~CommandImpactParticles() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
