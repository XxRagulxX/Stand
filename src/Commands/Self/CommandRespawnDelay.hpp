#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Network/GlobalPlayerBD.hpp"
#include "Scripting/Scripts.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRespawnDelay : public CommandToggle
	{
	public:
		explicit CommandRespawnDelay(CommandList* const parent)
			: CommandToggle(parent, LIT("Respawn Delay"), { CMDNAME("respawndelay") }, LIT("Skips the respawn wait timer after dying."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!Scripts::SafeToModifyFreemodeBroadcastGlobals())
				return;
			auto gpbd = GlobalPlayerBD::Get();
			if (!gpbd)
				return;
			auto& entry = gpbd->Entries[Stand::Self::GetPlayer().GetId()];
			if (entry.RespawnState != 0)
				entry.LastRespawnTime = 0;
		}

		~CommandRespawnDelay() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
