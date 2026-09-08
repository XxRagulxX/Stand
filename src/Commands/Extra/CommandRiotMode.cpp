#include "Commands/LoopedCommand.hpp"

#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class PedRiotMode : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			MISC::SET_RIOT_MODE_ENABLED(TRUE);
		}

		virtual void OnDisable() override
		{
			MISC::SET_RIOT_MODE_ENABLED(FALSE);
		}
	};

	static PedRiotMode _PedRiotMode{"PedRiotMode", "Riot Mode", "All PEDs attack each other."};

}