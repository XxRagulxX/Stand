#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
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
	class CommandImpactParticleSelect : public CommandSlider
	{
		static int DefaultIndex()
		{
			for (int i = 0; i < Particles::count; ++i)
			{
				if (std::string_view(Particles::particles[i].rawName) == "exp_grd_plane")
					return i;
			}
			return 0;
		}

	public:
		explicit CommandImpactParticleSelect(CommandList* parent)
			: CommandSlider(parent, LIT("Particle"), CMDNAMES("impactparticle"),
				  LIT("Lets you select the particle to use for Impact Particles."),
				  0, Particles::count - 1, DefaultIndex(), 1, 0)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			return Particles::particles[value].rawName;
		}
	};

	class CommandImpactParticles : public CommandToggle
	{
	public:
		CommandImpactParticleSelect* m_particle = nullptr;

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
				const int idx = m_particle ? m_particle->value : 0;
				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped.GetHandle(), &coords))
					Particles::tryPlay(idx, {coords.x, coords.y, coords.z});
			}
		}

		~CommandImpactParticles() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
