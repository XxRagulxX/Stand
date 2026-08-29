#include "Commands/Player/CommandHistoricPlayerStats.hpp"

#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Player/CommandHistoricPlayer.hpp"
#include "Commands/Widgets/CommandReadonlyValueCopy.hpp"
#include "Core/Exceptional.hpp"
#include "Rendering/Gui.hpp"
#include "Network/RemoteStats.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	CommandHistoricPlayerStats::CommandHistoricPlayerStats(CommandList* const parent)
		: CommandListWithOnTickAsActiveList(parent, LOC("STS"), {}, NOLABEL, CMDFLAGS_LIST | CMDFLAG_TEMPORARY)
	{
		createChild<CommandDivider>(LOC("GENWAIT"));
	}

	void CommandHistoricPlayerStats::onClick(Click& click)
	{
		if (!click.isRegularEdition())
		{
			return click.setResponse(LOC("CMDNEDR"));
		}
		s = RemoteStats::get(parent->as<CommandHistoricPlayer>()->player->rid);
		CommandListWithOnTickAsActiveList::onClick(click);
	}

	// LOC() constructs a temporary Label; keep it out of the __try frame.
	static void addWaitingDivider(CommandHistoricPlayerStats* self)
	{
		self->createChild<CommandDivider>(LOC("GENWAIT"));
	}

	void CommandHistoricPlayerStats::onActiveListUpdate()
	{
		CommandListWithOnTickAsActiveList::onActiveListUpdate();

		if (!isCurrentMenuListInTab()
			&& !isCurrentWebList()
			&& s
			&& RemoteStats::tryFree(s)
			)
		{
			resetChildren();
			EXCEPTIONAL_LOCK_WRITE(g_gui.root_mtx)
			addWaitingDivider(this);
			EXCEPTIONAL_UNLOCK_WRITE(g_gui.root_mtx)
		}
	}

	// LOC()/fmt::to_wstring()/etc. construct temporaries (Label, std::wstring); keep them out of the __try frame.
	static void addPlayerStatsChildren(CommandHistoricPlayerStats* self, RemoteStats* s)
	{
		if (s->hasData())
		{
			self->createChild<CommandReadonlyValueCopy>(LOC("MNY_B"), StringUtils::toStringWithThousandsSeparatorW(s->getBankBalance()));
			self->createChild<CommandReadonlyValueCopy>(LOC("KLLS"), fmt::to_wstring(s->getKills()));
			self->createChild<CommandReadonlyValueCopy>(LOC("DTHS"), fmt::to_wstring(s->getDeaths()));
			self->createChild<CommandReadonlyValueCopy>(LOC("KD"), fmt::to_wstring(s->getKillDeathRatio()));
			if (s->getMp0IsActive())
			{
				self->createChild<CommandDivider>(s->getLastChar() == 0 ? LOC("CHAR1_ACT") : LOC("CHAR1"));
				self->createChild<CommandReadonlyValueCopy>(LOC("RNK"), fmt::to_wstring(s->getMp0Rank()));
				self->createChild<CommandReadonlyValueCopy>(LOC("MNY_W"), StringUtils::toStringWithThousandsSeparatorW(s->getMp0WalletBalance()));
			}
			if (s->getMp1IsActive())
			{
				self->createChild<CommandDivider>(s->getLastChar() == 1 ? LOC("CHAR2_ACT") : LOC("CHAR2"));
				self->createChild<CommandReadonlyValueCopy>(LOC("RNK"), fmt::to_wstring(s->getMp1Rank()));
				self->createChild<CommandReadonlyValueCopy>(LOC("MNY_W"), StringUtils::toStringWithThousandsSeparatorW(s->getMp1WalletBalance()));
			}
		}
		else
		{
			// A player who has never played before, e.g. test123
			self->createChild<CommandDivider>(LOC("NDATA"));
		}
	}

	void CommandHistoricPlayerStats::onTickAsActiveList()
	{
		if (children.size() == 1
			&& s
			&& !s->isPending()
			)
		{
			resetChildren();
			EXCEPTIONAL_LOCK_WRITE(g_gui.root_mtx)
			addPlayerStatsChildren(this, s);
			EXCEPTIONAL_UNLOCK_WRITE(g_gui.root_mtx)
			processChildrenUpdate();
		}
	}
}
