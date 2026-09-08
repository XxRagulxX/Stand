#include "Commands/PlayerCommand.hpp"

#include "Network/Players.hpp"
#include "World/Self.hpp"

namespace Stand
{
	PlayerAllCommand::PlayerAllCommand(std::string name, std::string label, std::string description, int num_args, PlayerCommand* parent) :
	    CommandLegacy(name + "all", label, description, num_args),
	    m_PlayerCommand(parent)
	{
	}

	void PlayerAllCommand::OnCall()
	{
		std::vector<Player> validPlayers;
		validPlayers.reserve(Players::GetPlayers().size() - 1);
		for (auto& p : Players::GetPlayers())
		{
			if (p.second.IsValid() && (!Self::GetPlayer().IsValid() || p.second.GetId() != Self::GetPlayer().GetId()))
				validPlayers.push_back(p.second);
		}
		m_PlayerCommand->Call(validPlayers);
	}

	void PlayerCommand::OnCall(const std::vector<Player>& players)
	{
		for (auto& p : players)
			Call(p);
	}

	PlayerCommand::PlayerCommand(std::string name, std::string label, std::string description, int num_args, bool all_version) :
	    CommandLegacy::CommandLegacy(name, label, description, num_args + 1)
	{
		if (all_version)
			m_AllCommand = std::make_unique<PlayerAllCommand>(name, label, description, num_args, this);
	}

	void PlayerCommand::OnCall()
	{
		LOG(WARNING) << GetName() << " requires a player argument";
	}

	void PlayerCommand::Call(Player target)
	{
		OnCall(target);
	}

	void PlayerCommand::Call(const std::vector<Player>& targets)
	{
		OnCall(targets);
	}
}