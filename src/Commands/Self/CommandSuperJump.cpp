#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class SuperJump : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(Self::GetPlayer().GetId());
		}
	};

	static SuperJump _SuperJump{"superjump", "Super Jump", "Jump higher than normal"};
}