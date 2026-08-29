#include "Network/PlayerProvider.hpp"

#include <unordered_set>

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandPlayer.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Core/Exceptional.hpp"
#include "Core/FiberPool.hpp"
#include "Rendering/Gui.hpp"
#include "Network/PlayerProviderSingle.hpp"
#include "Scripting/Script.hpp"
#include "Core/regular_event.hpp"

namespace Stand
{
	std::vector<AbstractPlayer> PlayerProvider::getPlayers() const
	{
		return getPlayers(single);
	}

	AbstractPlayer PlayerProvider::getPlayer() const
	{
		if (single)
		{
			return ((const PlayerProviderSingle*)this)->getPlayer();
		}
		return MAX_PLAYERS;
	}

	bool PlayerProvider::isUser() const
	{
		return single && ((const PlayerProviderSingle*)this)->p == g_player;
	}

	void PlayerProvider::toggleFiberBatch(CommandPhysical* const cmd, std::function<void(const std::vector<AbstractPlayer>&)>&& callback) const
	{
		toggleFiberBatch(cmd->getPathEnFromParent(COMMAND_LIST_PLAYER), std::move(callback));
	}

	static std::unordered_set<std::string> active_toggle_batches;

	// No __try here, so it's safe to construct the per-command AbstractPlayer/vector temporaries in this frame.
	static void processPlayerCommandForToggleBatch(CommandPlayer* const command, std::unordered_set<Player>& included_players, std::vector<AbstractPlayer>& players, const std::string& english_command_path_from_player_command)
	{
		AbstractPlayer ap(command->pp->getPlayers(true).at(0));
		if (included_players.find(ap) == included_players.end())
		{
			auto* const toggle = ((const CommandToggleNoCorrelation*)command->resolveCommandEnglish(english_command_path_from_player_command));
			if (toggle != nullptr && toggle->m_on)
			{
				players.emplace_back(ap);
			}
		}
	}

	// No destructible locals here (only references/pointers), so it's safe to __try in this function.
	static void scanPlayerCommandsForToggleBatch(std::unordered_set<Player>& included_players, std::vector<AbstractPlayer>& players, const std::string& english_command_path_from_player_command)
	{
		g_gui.root_mtx.lockRead();
		__try
		{
			// Note: No reference here because we could hit a race condition where command is set to nullptr after we checked that it's not a nullptr!
			for (const auto command : g_gui.player_commands)
			{
				if (command == nullptr)
				{
					continue;
				}
				processPlayerCommandForToggleBatch(command, included_players, players, english_command_path_from_player_command);
			}
		}
		__EXCEPTIONAL()
		{
		}
		g_gui.root_mtx.unlockRead();
	}

	// No __try here, so it's safe to own the included_players/players containers in this frame.
	static std::vector<AbstractPlayer> collectToggleBatchPlayers(const CommandToggleNoCorrelation* const all_players_toggle, const std::string& english_command_path_from_player_command)
	{
		std::unordered_set<Player> included_players;
		std::vector<AbstractPlayer> players;
		if (all_players_toggle->m_on)
		{
			players = PlayerProvider::excludes.getNonExcludedPlayers(true);
			for (const AbstractPlayer p : players)
			{
				included_players.emplace(p.p);
			}
		}
		scanPlayerCommandsForToggleBatch(included_players, players, english_command_path_from_player_command);
		return players;
	}

	// No destructible locals here (only references), so it's safe to __try in this function.
	static void invokeToggleFiberBatchCallback(const std::function<void(const std::vector<AbstractPlayer>&)>& callback, const std::vector<AbstractPlayer>& players)
	{
		__try
		{
			callback(players);
		}
		__EXCEPTIONAL()
		{
		}
	}

	// No __try here, so it's safe to own the players vector across loop iterations in this frame.
	static void runToggleFiberBatchLoop(const CommandToggleNoCorrelation* const all_players_toggle, const std::string& english_command_path_from_player_command, const std::function<void(const std::vector<AbstractPlayer>&)>& callback)
	{
		do
		{
			std::vector<AbstractPlayer> players = collectToggleBatchPlayers(all_players_toggle, english_command_path_from_player_command);
			if (!all_players_toggle->m_on && players.empty())
			{
				break;
			}
			invokeToggleFiberBatchCallback(callback, players);
			Script::current()->yield();
		} while (true);
	}

	void PlayerProvider::toggleFiberBatch(const std::string& english_command_path_from_player_command, std::function<void(const std::vector<AbstractPlayer>&)>&& callback) const
	{
		if (active_toggle_batches.find(english_command_path_from_player_command) != active_toggle_batches.end())
		{
			return;
		}
		active_toggle_batches.emplace(english_command_path_from_player_command);
		auto* const all_players_toggle = ((const CommandToggleNoCorrelation*)((const CommandList*)((const CommandList*)g_gui.root_list->resolveChildByMenuName(LOC("PLYLST")))->resolveChildByMenuName(LOC("ALLPLY")))->resolveCommandEnglish(english_command_path_from_player_command));
		runToggleFiberBatchLoop(all_players_toggle, english_command_path_from_player_command, callback);
		active_toggle_batches.erase(english_command_path_from_player_command);
	}

	void PlayerProvider::toggleBatch(const std::string& english_command_path_from_player_command, std::function<void(const std::vector<AbstractPlayer>&)>&& callback) const
	{
		if (active_toggle_batches.find(english_command_path_from_player_command) == active_toggle_batches.end())
		{
			FiberPool::queueJob([english_command_path_from_player_command, callback{std::move(callback)}]
			{
				active_toggle_batches.emplace(english_command_path_from_player_command);
				auto* const all_players_toggle = ((const CommandToggleNoCorrelation*)((const CommandList*)((const CommandList*)g_gui.root_list->resolveChildByMenuName(LOC("PLYLST")))->resolveChildByMenuName(LOC("ALLPLY")))->resolveCommandEnglish(english_command_path_from_player_command));
				reScriptTickEvent::registerHandlerInThreadContext([=]()
				{
					std::vector<AbstractPlayer> players = collectToggleBatchPlayers(all_players_toggle, english_command_path_from_player_command);
					if (!all_players_toggle->m_on && players.empty())
					{
						active_toggle_batches.erase(english_command_path_from_player_command);
						return false;
					}
					callback(players);
					return true;
				}, std::move(std::string(english_command_path_from_player_command)));
			});
		}
	}
}
