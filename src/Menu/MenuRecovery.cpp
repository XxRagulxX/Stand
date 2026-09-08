#include "Menu/MenuRecovery.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in RecoveryGrid/
	// RecoveryBusinessesGrid/RecoveryHeistsGrid/DailyActivitiesGrid/
	// StatEditorGrid/TransactionsGrid/RecoveryCasinoGrid/
	// RecoveryUnlocksGrid instead; this stays a real Submenu only so a
	// Lua script can still attach into "Recovery" by name).
	Recovery::Recovery() :
#define ICON_FA_SACK_DOLLAR "\xef\x93\x80"
	    Submenu::Submenu("Recovery", ICON_FA_SACK_DOLLAR)
	{
	}
}
