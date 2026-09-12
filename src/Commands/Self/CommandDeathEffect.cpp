#include "Commands/Self/CommandDeathEffect.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Rendering/Particles.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	CommandDeathEffect::CommandDeathEffect(CommandList* parent)
		: CommandSlider(parent, LIT("Death Effect"), CMDNAMES("deathparticle"), NOLABEL, 0, Particles::count, 0, 1, 0)
	{
		CommandTickDispatch::AddCommand(this);
	}

	CommandDeathEffect::~CommandDeathEffect()
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	std::string CommandDeathEffect::getValueText() const
	{
		if (value == 0)
			return "None";
		return Particles::particles[value - 1].rawName;
	}

	void CommandDeathEffect::onChange(Click& click, int prev_value)
	{
	}

	void CommandDeathEffect::onTick()
	{
		if (value == 0)
		{
			m_sent_fx = false;
			return;
		}
		if (ENTITY::IS_ENTITY_DEAD(Self::GetPed().GetHandle(), false))
		{
			if (!m_sent_fx)
			{
				Particles::tryPlay(value - 1, Self::GetPed().GetPosition());
				m_sent_fx = true;
			}
		}
		else
		{
			m_sent_fx = false;
		}
	}

	void CommandDeathEffect::applyDefaultState()
	{
		m_sent_fx = false;
		value = 0;
	}
}
