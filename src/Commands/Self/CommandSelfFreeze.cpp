#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandSelfFreeze.
	class SelfFreeze : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			ENTITY::FREEZE_ENTITY_POSITION(Self::GetPed().GetHandle(), true);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			ENTITY::FREEZE_ENTITY_POSITION(Self::GetPed().GetHandle(), false);
		}
	};

	static SelfFreeze _SelfFreeze{"selffreeze", "Self Freeze", "Freezes you in place"};
}
