#pragma once

// am_mp_rc_vehicle

#define FUNC_RC_ISRCPV			426

// appinternet

#define FUNC_AI_CANPRCHS		8705 // 2 functions below: (joaat("terbyte"), -1))

// carmod_shop

#define FUNC_CM_CANENTER		2962
#define FUNC_CM_CANFULLYMOD		2565

// freemode

#define FUNC_FM_ISRCVEH			2128
#define FUNC_FM_DAILY_EXPENSES	12911 // function above: iVar1 = 2880000;
#define FUNC_FM_BIGMAP			15466 // mpply_toggle_off_big_radar
#define FUNC_FM_SHOULDHIDEBLIP  17054 // if (iVar0 != iVar1 && !
#define FUNC_FM_SHOULDSHOWBLIP  17808 // bParam0] = NETWORK::GET_TIME_OFFSET(NETWORK::GET_NETWORK_TIME(), -35000);
#define FUNC_FM_AMTT_NSUIT		20927 // "AMTT_NSUIT" followed by `return 0;`
#define FUNC_FM_LDB_ISLAND_CHK  23604 // {DPAD_PASSED_ISLAND_CHECK} if ((bVar0 && !bVar1) || (!bVar0 && bVar1))
#define FUNC_FM_SET_PLY_CNT		23775 // {SET_TOTAL_PLAYER_COUNT_DATA_POST} ((*uParam0 - uParam0->f_3) - iVar0)

#include <cstdint>

namespace Stand
{
	class scrFunc
	{
	public:
		uint32_t logical_start = 0; // including Enter instruction
		uint32_t start = 0; // first instruction after Enter
		uint32_t end = 0; // instruction after final Return

		[[nodiscard]] bool isValid() const noexcept
		{
			return end != 0;
		}

		[[nodiscard]] uint32_t size() const noexcept
		{
			return end - start;
		}

		[[nodiscard]] uint32_t getLogicalSize() const noexcept
		{
			return end - logical_start;
		}
	};
}
