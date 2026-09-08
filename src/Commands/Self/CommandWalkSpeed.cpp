#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's own Walk Speed slider is -1,000,000 to 1,000,000
	// (step 10, /100 for a run-speed multiplier) - scaled down here to
	// 0-1000 (0x-10x, default 100 = 1.0x/normal), a far more practical
	// range than Stand's own literal one and consistent with every
	// other slider in this project - a deliberate improvement, not a
	// fidelity gap.
	//
	// Real Stand applies this by writing into its own
	// g_gui.user_defined_run_speed, read continuously by a central
	// movement system this project doesn't have. Reuses
	// PED::SET_PED_MOVE_RATE_OVERRIDE instead - the same native this
	// project's own Super Run (CommandSuperRun.cpp) already uses, and
	// confirmed there (not guessed) to need continuous per-tick
	// reapplication or the game's own default speed wins back. Real
	// Stand's own slider has no separate enable toggle either - it's
	// just always active - so this comes with its own hidden, always-on
	// LoopedCommand (WalkSpeedApplier below) rather than a plain
	// CommandSlider: m_State is set directly to true in its constructor
	// (bypassing SetState()'s own deferred FiberPool job, unsafe to run
	// during static initialization) and ShouldSaveState() returns false,
	// so it never shows up as a real toggle anywhere and never gets
	// written to the saved config - Commands::EnableBoolCommandsImpl()
	// (run once, safely, after static init finishes) is what actually
	// starts it ticking.
	//
	// Competes with Super Run if both are active (whichever's own OnTick
	// runs later in a frame wins) - the same kind of interaction real
	// Stand's own separate Walk Speed/Super Run features would have.
	static CommandSliderLegacy _WalkSpeed{"walkspeed", "Walk Speed", "Run speed multiplier (100 = normal)", 0, 1000, 100};

	class WalkSpeedApplier : public LoopedCommand
	{
	public:
		WalkSpeedApplier(std::string name, std::string label, std::string description) :
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
			if (auto ped = Self::GetPed())
				PED::SET_PED_MOVE_RATE_OVERRIDE(ped.GetHandle(), static_cast<float>(_WalkSpeed.GetState()) / 100.0f);
		}
	};

	static WalkSpeedApplier _WalkSpeedApplier{"walkspeedapplier", "Walk Speed Applier", "Internal - always on, applies Walk Speed every tick"};
}
