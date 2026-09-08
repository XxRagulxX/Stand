#include "Commands/PlayerCommand.hpp"
#include "Scripting/Scripts.hpp"
#include "Network/PlayerData.hpp"
#include "Network/GSBD.hpp"

namespace Stand::Features
{
	class FreemodeStateKick : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		void CorruptHighBits(void* script_var)
		{
			*(reinterpret_cast<std::int32_t*>(script_var) + 1) = rand();
		}

		virtual void OnCall(Player player) override
		{
			if (auto freemode = Scripts::FindScriptThread("freemode"_J); freemode && Scripts::SafeToModifyFreemodeBroadcastGlobals())
			{
				Scripts::ForceScriptHost(freemode);
				player.GetData().m_RunFreemodeStateKick = true;
				CorruptHighBits(&GSBD::Get()->FreemodeState);
			}
		}
	};

	static FreemodeStateKick _FreemodeStateKick{"endkick", "Freemode State Kick", "Kicks the player by forcing them to end the freemode session"};
}