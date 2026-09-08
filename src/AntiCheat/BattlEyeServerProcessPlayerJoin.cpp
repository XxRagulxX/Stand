#include "Core/DetourHook.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Core/Hooks.hpp"
#include "Scripting/Natives.hpp"
#include "AntiCheat/CBattlEyePlayerModifyContext.hpp"
#include "AntiCheat/AnticheatBypass.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Anticheat::BattlEyeServerProcessPlayerJoin(CBattlEyePlayerModifyInterface* server_iface, CBattlEyePlayerModifyContext* context)
	{
		if (!AnticheatBypass::IsBattlEyeRunning())
		{
			if (context->m_IsLocal)
			{
				if (NETWORK::NETWORK_SESSION_IS_CLOSED_FRIENDS() || NETWORK::NETWORK_SESSION_IS_CLOSED_CREW() || NETWORK::NETWORK_SESSION_IS_SOLO() || NETWORK::NETWORK_SESSION_IS_PRIVATE())
				{
					return true;
				}
			}

			static auto cheater_pool = Commands::GetCommand<CommandToggleLegacy>("cheaterpool"_J);
			if (cheater_pool->GetState())
			{
				return true;
			}
		}

		return Hooking::Get<Anticheat::BattlEyeServerProcessPlayerJoin>()->Original<decltype(&Anticheat::BattlEyeServerProcessPlayerJoin)>()(server_iface, context);
	}
}