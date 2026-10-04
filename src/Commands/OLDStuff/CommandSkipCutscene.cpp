#include "Commands/CommandLegacy.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class SkipCutscene : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		}
	};

	static SkipCutscene _SkipCutscene{"skipcutscene", "Skip Cutscene", "Skips any currently playing cutscene"};
}
