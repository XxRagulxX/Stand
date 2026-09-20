#pragma once
#include <cstddef>
#include <type_traits>
#include "Scripting/scrThread.hpp"
#include "Scripting/Scripts.hpp"

#define LOCAL_LNCH_PLAYERSTATE 270

#define LOCAL_BB2_SHDATA 5464
#define LOCAL_BB2_STATE LOCAL_BB2_SHDATA + 1152

#define LOCAL_CR_BASE 155
#define LOCAL_CR_SPINRESULT 1357 + 155

#define LOCAL_DRONE_DATA 233
#define LOCAL_DRONE_SHOCK_COOLDOWN LOCAL_DRONE_DATA + 253
#define LOCAL_DRONE_BOOST_RECHARGE_COOLDOWN LOCAL_DRONE_DATA + 257
#define LOCAL_DRONE_DETONATE_COOLDOWN LOCAL_DRONE_DATA + 356

#pragma region ScrUpdate
#define LOCAL_MT_LFF 210
#define LOCAL_MT_LFF_CHECKIDX 352
#define LOCAL_PI_STATE 1786
#define LOCAL_PI_BITSET 1952
#define LOCAL_PNDIN_SHDATA 129
#define LOCAL_CPCOL_SHDATA 851
#define LOCAL_HTB_SHDATA 635
#define LOCAL_HTB_BEAST LOCAL_HTB_SHDATA + 1 + 6
#define LOCAL_CHLNG_SHDATA 673
#define LOCAL_SHDATA_WARMUP 43
#define LOCAL_CHLNG_PLYDATA 2638
#define LOCAL_CHLNG_PLYDATA_SIZE 12
#define LOCAL_CRIMDMG_SCORE 139
#define LOCAL_CRIMDMG_SHDATA 146
#define LOCAL_CRIMDMG_PLYDATA 2076
#define LOCAL_CRIMDMG_PLYDATA_SIZE 7
#define LOCAL_BB_SHDATA 2474
#define LOCAL_SHOPCTRL_CHEAT 378
#define LOCAL_SHOPCTRL_CHEAT_STATE LOCAL_SHOPCTRL_CHEAT + 3
#define LOCAL_BJ_DEALERCARDS_1 145
#define LOCAL_BJ_DEALERCARDS_2 846
#define LOCAL_BJ_DEALERCARDS LOCAL_BJ_DEALERCARDS_1 + LOCAL_BJ_DEALERCARDS_2
#define LOCAL_BJ_DEALER_SIZE 13
#define LOCAL_GANGCALL_SHDATA 81
#define LOCAL_INT_SHDATA 10638
#define LOCAL_INT_SHDATA_KICK 690
#pragma endregion ScrUpdate

#define LOCAL_GANGCALL_STATE 0
#define LOCAL_GANGCALL_BITSET 1
#define LOCAL_GANGCALL_GANGMEMBER 10
#define LOCAL_GANGMEMBER_SIZE 7

namespace Stand
{
	class ScriptLocal
	{
	public:
		explicit ScriptLocal(rage::scrThread* thread, std::size_t index);
		explicit ScriptLocal(void* stack, std::size_t index);

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
		void* m_stack;
	};
}