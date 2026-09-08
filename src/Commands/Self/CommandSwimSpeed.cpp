#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's own Swim Speed slider, scaled to a practical range the
	// same way Walk Speed is (see CommandWalkSpeed.cpp's own comment for
	// the full reasoning, including why this needs its own hidden,
	// always-on LoopedCommand rather than a plain CommandSlider).
	// PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER is the same native this
	// project's own Super Run already uses for its own swim multiplier.
	static CommandSliderLegacy _SwimSpeed{"swimspeed", "Swim Speed", "Swim speed multiplier (100 = normal)", 0, 1000, 100};

	class SwimSpeedApplier : public LoopedCommand
	{
	public:
		SwimSpeedApplier(std::string name, std::string label, std::string description) :
		    LoopedCommand(std::move(name), std::move(label), std::move(description))
		{
			m_State = true;
		}

		virtual bool ShouldSaveState() const override
		{
			return false;
		}

	protected:
		virtual void OnTick() override
		{
			if (auto player = Self::GetPlayer())
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player.GetId(), static_cast<float>(_SwimSpeed.GetState()) / 100.0f);
		}
	};

	static SwimSpeedApplier _SwimSpeedApplier{"swimspeedapplier", "Swim Speed Applier", "Internal - always on, applies Swim Speed every tick"};
}
