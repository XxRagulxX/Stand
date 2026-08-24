#pragma once

#include <cstddef>
#include <type_traits>
#include <windows.h>

#include "gta_fwddecl.hpp"

// am_launcher

#define LOCAL_LNCH_PLAYERSTATE 268

// fm_content_business_battles

#define LOCAL_BB2_SHDATA			5462
#define LOCAL_BB2_STATE				LOCAL_BB2_SHDATA + 1152

// casinoroulette

#define LOCAL_CR_BASE				153 // NETWORK_REGISTER_HOST_BROADCAST_VARIABLES
#define LOCAL_CR_SPINRESULT			1357 + 153 // if (Var0.f_4 == 31)

// am_mp_drone
#define LOCAL_DRONE_DATA 231
#define LOCAL_DRONE_SHOCK_COOLDOWN LOCAL_DRONE_DATA + 253 // iVar1 = 3000;
#define LOCAL_DRONE_BOOST_RECHARGE_COOLDOWN LOCAL_DRONE_DATA + 257
#define LOCAL_DRONE_DETONATE_COOLDOWN LOCAL_DRONE_DATA + 356 // HUD_Detonate_Charge

#pragma region ScrUpdate
// maintransition
#define LOCAL_MT_LFF 181
#define LOCAL_MT_LFF_CHECKIDX 352
// am_pi_menu
#define LOCAL_PI_STATE 1786
#define LOCAL_PI_BITSET 1952
// am_penned_in
#define LOCAL_PNDIN_SHDATA 127
// am_cp_collection
#define LOCAL_CPCOL_SHDATA 849
// am_hunt_the_beast
#define LOCAL_HTB_SHDATA 633
#define LOCAL_HTB_BEAST LOCAL_HTB_SHDATA + 1 + 6
// am_challenges
#define LOCAL_CHLNG_SHDATA 671
#define LOCAL_SHDATA_WARMUP 43
#define LOCAL_CHLNG_PLYDATA 2638
#define LOCAL_CHLNG_PLYDATA_SIZE 12
// am_criminal_damage
#define LOCAL_CRIMDMG_SCORE 139
#define LOCAL_CRIMDMG_SHDATA 144
#define LOCAL_CRIMDMG_PLYDATA 2076
#define LOCAL_CRIMDMG_PLYDATA_SIZE 7
// business_battles
#define LOCAL_BB_SHDATA 2472
// shop_controller
#define LOCAL_SHOPCTRL_CHEAT 378
#define LOCAL_SHOPCTRL_CHEAT_STATE LOCAL_SHOPCTRL_CHEAT + 3
// blackjack
#define LOCAL_BJ_DEALERCARDS_1 145
#define LOCAL_BJ_DEALERCARDS_2 846
#define LOCAL_BJ_DEALERCARDS LOCAL_BJ_DEALERCARDS_1 + LOCAL_BJ_DEALERCARDS_2
#define LOCAL_BJ_DEALER_SIZE 13
// am_gang_call
#define LOCAL_GANGCALL_SHDATA 81
// am_mp_property_int
#define LOCAL_INT_SHDATA 10638
#define LOCAL_INT_SHDATA_KICK 690
#pragma endregion ScrUpdate

#define LOCAL_GANGCALL_STATE		0
#define LOCAL_GANGCALL_BITSET		1
#define LOCAL_GANGCALL_GANGMEMBER	10
#define LOCAL_GANGMEMBER_SIZE		7

namespace Stand
{
	class ScriptLocal
	{
	public:
		explicit ScriptLocal(rage::scrThread* thread, std::size_t index);
		explicit ScriptLocal(PVOID stack, std::size_t index);

		ScriptLocal at(std::ptrdiff_t index);
		ScriptLocal at(std::ptrdiff_t index, std::size_t size);

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as()
		{
			return static_cast<T>(get());
		}

		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
		{
			return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(get());
		}
	private:
		void* get();
		std::size_t m_index;
		PVOID m_stack;
	};
}
