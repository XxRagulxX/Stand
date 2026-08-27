#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Commands/Player/CommandPlayer.hpp"
#include "Core/FiberPool.hpp"
#include "Network/is_session.hpp"
#include "Network/JoinUtil.hpp"
#include "Network/RemoteGamer.hpp"
#include "Scripting/Script.hpp"
#include "Core/tbLoadingSessionDetails.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandJoinDesynced : public CommandToggle
	{
	private:
		uint8_t player_count_last_tick = 0;

	public:
		ToastConfig toast_config{ true };
		inline static bool amnesty = false;

		explicit CommandJoinDesynced(CommandList* const parent)
			: CommandToggle(parent, LOC("AUTOREJOIN"), CMDNAMES_OBF("autorejoindesynced"), LOC("AUTOREJOIN_H"))
		{
		}

		void onEnable(Click& click) final
		{
			registerScriptTickEventHandler([this]
			{
				HANDLER_CHECK(this->m_on);
				if (!amnesty)
				{
					if (wasDesynced())
					{
						Util::toast(LANG_GET("AUTOREJOIN_MSG"), toast_config.getToastFlags());
						FiberPool::queueJob([this]
						{
							if (!attemptJoin())
							{
								Util::toast(LANG_GET("AUTOREJOIN_FAIL"), toast_config.getToastFlags());
							}
						});
					}
				}
				else
				{
					amnesty = false;
				}
				player_count_last_tick = (uint8_t)getPlayersNotTimedOut().size();
				HANDLER_END;
			});
		}

		[[nodiscard]] bool wasDesynced() const
		{
			return is_session_freeroam() // Post-mission completion screen may send you to a solo job lobby.
				&& player_count_last_tick > 5 // 4 players can leave at once due to CEO missions & stuff.
				&& !is_session_transition_active(true)
				&& AbstractPlayer::listAll().size() == 1
				;
		}

		[[nodiscard]] bool attemptJoin() const
		{
			if (auto* g = RemoteGamer::get(CommandPlayer::trampoline_rid, true, RGU_BULK))
			{
				// As non-host, we want to use nuts to avoid "You're already a member of this GTA Online session."
				// Also has the side-effect of not giving the user the prompt. If that's good or bad depends on the user I guess.
				return g->useAsTrampoline(true);
			}
			return false;
		}

		[[nodiscard]] std::vector<AbstractPlayer> getPlayersNotTimedOut() const
		{
			auto players = AbstractPlayer::listAll();
			std::erase_if(players, [](AbstractPlayer& p)
			{
				return p.isInTimeout();
			});
			return players;
		}
	};
}