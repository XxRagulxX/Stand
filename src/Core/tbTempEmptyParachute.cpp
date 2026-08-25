#include "Core/tbTempEmptyParachute.hpp"

#include "Core/AbstractEntity.hpp"
#include "Core/AbstractPlayer.hpp"
#include "Ped/ePedConfigFlags.hpp"
#include "Game/gta_ped.hpp"
#include "Game/atStringHash.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	void tbTempEmptyParachute::onEnable()
	{
		had_parachute = WEAPON::HAS_PED_GOT_WEAPON(g_player_ped, ATSTRINGHASH("gadget_parachute"), FALSE);
		if (had_parachute)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(g_player_ped, ATSTRINGHASH("gadget_parachute"));
		}

		had_reserve_parachute = PLAYER::GET_PLAYER_HAS_RESERVE_PARACHUTE(g_player);
		if (had_reserve_parachute)
		{
			PED::SET_PED_CONFIG_FLAG(g_player_ped, CPED_CONFIG_FLAG_HasReserveParachute, FALSE);
		}
	}

	void tbTempEmptyParachute::onDisable()
	{
		if (had_parachute)
		{
			g_player_ped.giveParachute();
		}

		if (had_reserve_parachute)
		{
			PLAYER::SET_PLAYER_HAS_RESERVE_PARACHUTE(g_player);
		}
	}
}
