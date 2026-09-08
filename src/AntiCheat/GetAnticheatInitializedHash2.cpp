#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Pointers.hpp"
#include "AntiCheat/CAnticheatContext.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	std::uint32_t Anticheat::GetAnticheatInitializedHash2(void* ac_var, std::uint32_t seed)
	{
		auto orig = (*Pointers.AnticheatContext) ? (*Pointers.AnticheatContext)->m_BattlEyeEnabled : false;
		if (*Pointers.AnticheatContext)
			(*Pointers.AnticheatContext)->m_BattlEyeEnabled = true; // integ checks will boot us out if we set this outside this function
		auto ret = Hooking::Get<Anticheat::GetAnticheatInitializedHash2>()->Original<decltype(&Anticheat::GetAnticheatInitializedHash2)>()(ac_var, seed);
		if (*Pointers.AnticheatContext)
			(*Pointers.AnticheatContext)->m_BattlEyeEnabled = orig;
		return ret;
	}
}