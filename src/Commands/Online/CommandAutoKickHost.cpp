#include "CommandAutoKickHost.hpp"

#include "AbstractPlayer.hpp"
#include "CommandPlayer.hpp"
#include "CommandSessionHopper.hpp"
#include "evtTransitionFinishedEvent.hpp"
#include "FiberPool.hpp"
#include "Script.hpp"

namespace Stand
{
	CommandAutoKickHost::CommandAutoKickHost(CommandList* parent)
		: CommandToggleUltimate(parent, LOC("AKCKHST"))
	{
		instance = this;
	}

	CommandAutoKickHost::~CommandAutoKickHost()
	{
		if (instance == this)
		{
			instance = nullptr;
		}
	}

	static void onAutoKickHostTransitionFinished(evtTransitionFinishedEvent&)
	{
		if (!CommandSessionHopper::instance->m_on || !CommandSessionHopper::instance->shouldHop())
		{
			auto next_host = AbstractPlayer::getNextHost();
			if (next_host == g_player)
			{
				FiberPool::queueJob([]
				{
					AbstractPlayer p;
					while (p = AbstractPlayer::getHost(), p != g_player)
					{
						if (auto cmd = p.getCommand())
						{
							if (cmd->hasStandUserHandshakeFinished())
							{
								if (CommandAutoKickHost::instance->canRemove(p))
								{
									p.kick(false);
								}
								break;
							}
						}
						Script::current()->yield();
					}
				});
			}
		}
	}

	void CommandAutoKickHost::onEnableInner(Click& click)
	{
		evtTransitionFinishedEvent::registerHandler(&onAutoKickHostTransitionFinished);
	}

	void CommandAutoKickHost::onDisableInner(Click& click)
	{
		evtTransitionFinishedEvent::unregisterHandler(&onAutoKickHostTransitionFinished);
	}

	[[nodiscard]] bool CommandAutoKickHost::canRemove(AbstractPlayer p) const
	{
		return !excludes.isExcluded(p);
	}
}
