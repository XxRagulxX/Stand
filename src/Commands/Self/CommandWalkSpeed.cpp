#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	static CommandSliderFloatLegacy _WalkSpeed{"walkspeed", "Walk And Run Speed",
	    "Run speed multiplier (1.0 = normal)", -10000.0f, 10000.0f, 1.0f};

	class WalkSpeedApplier : public LoopedCommand
	{
	public:
		WalkSpeedApplier(std::string name, std::string label, std::string description) :
		    LoopedCommand(std::move(name), std::move(label), std::move(description))
		{
			m_State = true;
		}

		bool ShouldSaveState() const override
		{
			return false;
		}

	protected:
		void OnTick() override
		{
			if (auto ped = Self::GetPed())
				PED::SET_PED_MOVE_RATE_OVERRIDE(ped.GetHandle(), _WalkSpeed.GetState());
		}
	};

	static WalkSpeedApplier _WalkSpeedApplier{"walkspeedapplier", "Walk Speed Applier",
	    "Internal - always on, applies Walk Speed every tick"};
}
