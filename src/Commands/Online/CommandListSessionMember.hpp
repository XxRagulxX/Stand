#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "Network/rlGamerInfo.hpp"
#include "Network/ScAccount.hpp"

namespace Stand
{
	class CommandListSessionMember : public CommandList
	{
	public:
		rage::rlGamerInfo gamerInfo;
		ScAccount* account;
		time_t last_accounted_for;
		bool did_scaccount_lookup = false;

		explicit CommandListSessionMember(CommandList* const _parent, rage::rlGamerInfo&& _gamerInfo);

		[[nodiscard]] const CommandListSession* getSession() const;
		[[nodiscard]] const rage::rlSessionInfo* getSessionInfoIfAvailable() const;

		void onTickInGameViewport() final;
		void onTickInWebViewport() final;
	};
}
