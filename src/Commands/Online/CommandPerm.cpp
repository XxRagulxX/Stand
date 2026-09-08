#include "Commands/Online/CommandPerm.hpp"

namespace Stand
{
	Label getCommandPermLabel(CommandPerm perm)
	{
		switch (perm)
		{
		case COMMANDPERM_FRIENDLY:
			return LIT("Friendly");

		case COMMANDPERM_NEUTRAL:
			return LIT("Neutral");

		case COMMANDPERM_SPAWN:
			return LIT("Spawn");

		case COMMANDPERM_RUDE:
			return LIT("Rude");

		case COMMANDPERM_AGGRESSIVE:
			return LIT("Aggressive");

		case COMMANDPERM_TOXIC:
			return LIT("Toxic");
		}
		return NOLABEL;
	}
}
