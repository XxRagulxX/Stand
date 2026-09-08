#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class Suicide : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Self::GetPed().SetInvincible(false);
			Self::GetPed().SetHealth(0);
		}
	};

	static Suicide _Suicide{"suicide", "Suicide", "Kills you"};
}