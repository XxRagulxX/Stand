#pragma once
#include "Commands/CommandLegacy.hpp"
#include "Game/Player.hpp"

namespace Stand
{
	class PlayerCommand;
	class PlayerAllCommand : public CommandLegacy
	{
		PlayerCommand* m_PlayerCommand;

	public:
		PlayerAllCommand(std::string name, std::string label, std::string description, int num_args, PlayerCommand* parent);
		virtual void OnCall() override;
	};

	class PlayerCommand : public CommandLegacy
	{
		virtual void OnCall() override;
		std::unique_ptr<PlayerAllCommand> m_AllCommand;

	protected:
		virtual void OnCall(Player) = 0;
		virtual void OnCall(const std::vector<Player>&);

	public:
		PlayerCommand(std::string name, std::string label, std::string description, int num_args = 0, bool all_version = true);
		void Call(Player target);
		void Call(const std::vector<Player>& targets);
	};
}