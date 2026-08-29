#include "Commands/Online/CommandRidTools.hpp"

#include "Commands/Widgets/CommandDivider.hpp"

#include "Commands/Online/CommandNameJoin.hpp"
#include "Commands/Online/CommandNameSpectate.hpp"
#include "Commands/Online/CommandNameInvite.hpp"
#include "Commands/Stand/CommandNameProfile.hpp"
//#include "CommandNameKick.hpp"
#include "Commands/Online/CommandRidJoin.hpp"
#include "Commands/Online/CommandRidSpectate.hpp"
#include "Commands/Online/CommandRidInvite.hpp"
#include "Commands/Online/CommandRidProfile.hpp"
//#include "CommandRidKick.hpp"
#ifdef STAND_DEBUG
#include "Commands/Online/CommandGsinfoJoin.hpp"
#endif

namespace Stand
{
	CommandRidTools::CommandRidTools(CommandList* const parent)
		: CommandList(parent, LOC("RIDTOOLS"))
	{
		this->createChild<CommandDivider>(LOC("NME"));
		this->createChild<CommandNameJoin>();
#if CAN_SPECTATE
		this->createChild<CommandNameSpectate>();
#endif
		this->createChild<CommandNameInvite>();
		this->createChild<CommandNameProfile>();
		//this->createChild<CommandNameKick>();
		this->createChild<CommandDivider>(LIT("Rockstar ID"));
		this->createChild<CommandRidJoin>();
#if CAN_SPECTATE
		this->createChild<CommandRidSpectate>();
#endif
		this->createChild<CommandRidInvite>();
		this->createChild<CommandRidProfile>();
		//this->createChild<CommandRidKick>();
#ifdef STAND_DEBUG
		this->createChild<CommandDivider>(LIT("gsinfo"));
		this->createChild<CommandGsinfoJoin>();
#endif
	}
}
