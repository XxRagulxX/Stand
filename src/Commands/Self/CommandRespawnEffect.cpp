#include "Commands/Self/CommandRespawnEffect.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/Particles.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	CommandRespawnEffect::CommandRespawnEffect(CommandList* parent)
		: CommandSlider(parent, LIT("Respawn Effect"), CMDNAMES("respawnparticle"), NOLABEL, 0, Particles::count, 0, 1, 0)
	{
		CommandTickDispatch::AddCommand(this);
	}

	CommandRespawnEffect::~CommandRespawnEffect()
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	std::string CommandRespawnEffect::getValueText() const
	{
		if (value == 0)
			return "None";
		return Particles::particles[value - 1].rawName;
	}

	void CommandRespawnEffect::onChange(Click& click, int prev_value)
	{
	}

	void CommandRespawnEffect::onTick()
	{
		if (value == 0)
		{
			m_dead_last_tick = false;
			return;
		}
		if (ENTITY::IS_ENTITY_DEAD(Self::GetPed().GetHandle(), false))
		{
			m_dead_last_tick = true;
		}
		else if (m_dead_last_tick)
		{
			const int idx = value - 1;
			const rage::fvector3 pos = Self::GetPed().GetPosition();
			FiberPool::queueJob([idx, pos]
			{
				Particles::play(idx, pos);
			});
			m_dead_last_tick = false;
		}
	}

	void CommandRespawnEffect::applyDefaultState()
	{
		m_dead_last_tick = false;
		value = 0;
	}
}
