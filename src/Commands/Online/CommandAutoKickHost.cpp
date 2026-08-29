#include "Commands/Online/CommandAutoKickHost.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandPlayer.hpp"
#include "Commands/Online/CommandSessionHopper.hpp"
#include "Network/evtTransitionFinishedEvent.hpp"
#include "Core/FiberPool.hpp"
#include "Scripting/Script.hpp"

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
