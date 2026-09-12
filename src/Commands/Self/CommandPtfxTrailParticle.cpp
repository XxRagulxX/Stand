#include "Commands/Self/CommandPtfxTrailParticle.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Particles.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "World/Self.hpp"

namespace Stand
{
	CommandPtfxTrailParticle::CommandPtfxTrailParticle(CommandList* parent)
		: CommandSlider(parent, LIT("Particle"), CMDNAMES("trailparticle"),
			NOLABEL, 0, Particles::count, 0, 1, 0)
	{
	}

	std::string CommandPtfxTrailParticle::getValueText() const
	{
		if (value == 0)
			return "None";
		return Particles::particles[value - 1].rawName;
	}

	void CommandPtfxTrailParticle::onChange(Click& click, int prev_value)
	{
		if (value > 0 && prev_value == 0)
			CommandTickDispatch::AddCommand(this);
		else if (value == 0 && prev_value > 0)
			CommandTickDispatch::RemoveCommand(this);
	}

	void CommandPtfxTrailParticle::onTick()
	{
		if (value == 0 || !m_interval || !m_scale || !m_offset)
			return;

		const auto& p = Particles::particles[value - 1];
		STREAMING::REQUEST_NAMED_PTFX_ASSET(p.assetCategory);
		if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(p.assetCategory))
			return;

		const auto  now  = get_current_time_millis();
		const auto  ped  = Self::GetPed().GetHandle();
		const float sc   = m_scale->getFloatValue();
		const float off  = m_offset->getFloatValue();
		const int   iv   = m_interval->value;

		bool used = false;
		for (auto* bone : m_bones)
		{
			if (!bone->m_on)
				continue;
			if (iv > 0 && GET_MILLIS_SINCE(bone->last_applied) < (time_t)iv)
				continue;

			if (!used)
			{
				GRAPHICS::USE_PARTICLE_FX_ASSET(p.assetCategory);
				used = true;
			}
			GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(
				p.rawName, ped,
				0.0f, 0.0f, off,
				0.0f, 180.0f, 0.0f,
				bone->bone,
				sc,
				false, false, false);
			bone->last_applied = now;
		}

		if (used)
			STREAMING::REMOVE_NAMED_PTFX_ASSET(p.assetCategory);
	}

	void CommandPtfxTrailParticle::applyDefaultState()
	{
		if (value > 0)
			CommandTickDispatch::RemoveCommand(this);
		value = 0;
		for (auto* bone : m_bones)
			bone->last_applied = 0;
	}
}
