#include "Commands/CommandSliderLegacy.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	// Real Stand's CommandFakeWanted (0-6) applies the value immediately
	// and, only while it's above 0, keeps re-asserting it every tick via
	// a script-tick handler registered from its own onChange (real wanted
	// stars can still overwrite the fake ones on their own). This project
	// has no equivalent "keep re-asserting a captured value from inside a
	// slider's own onChange" primitive - see CommandMaxHealth.cpp's
	// identical note - so this applies it once, immediately, on change
	// instead; a real, disclosed gap from Stand's own continuous
	// enforcement.
	class FakeWanted : public CommandSliderLegacy
	{
		using CommandSliderLegacy::CommandSliderLegacy;

		virtual void OnChange() override
		{
			MISC::SET_FAKE_WANTED_LEVEL(GetState());
		}
	};

	static FakeWanted _FakeWanted{"fakewanted", "Fake Wanted", "Shows wanted stars on the HUD without actually being wanted", 0, 6, 0};
}
