#include "Commands/Commands.hpp"
#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/ScriptMgr.hpp"

namespace Stand
{
	Commands::Commands() :
	    IStateSerializer("commands")
	{
	}

	void Commands::RunScript()
	{
		Commands::EnableBoolCommands();
		while (g_Running)
		{
			Commands::RunLoopedCommands();
			Script::current()->yield();
		}
	}

	void Commands::AddCommandImpl(CommandLegacy* command)
	{
		m_Commands.insert({command->GetHash(), command});
	}

	void Commands::AddBoolCommandImpl(CommandToggleLegacy* command)
	{
		m_BoolCommands.push_back(command);
	}

	void Commands::AddLoopedCommandImpl(LoopedCommand* command)
	{
		m_LoopedCommands.push_back(command);
	}

	void Commands::RemoveCommandImpl(CommandLegacy* command)
	{
		if (!command)
			return;

		m_Commands.erase(command->GetHash());
		
		std::erase(m_BoolCommands, dynamic_cast<CommandToggleLegacy*>(command));
		std::erase(m_LoopedCommands, dynamic_cast<LoopedCommand*>(command));
	}

	void Commands::EnableBoolCommandsImpl()
	{
		for (auto& command : m_BoolCommands)
			if (command->GetState())
				command->Initialize();
	}

	void Commands::RunLoopedCommandsImpl()
	{
		for (auto& command : m_LoopedCommands)
			if (command->GetState() && command->IsReady())
				command->Tick();
	}

	CommandLegacy* Commands::GetCommandImpl(joaat_t hash)
	{
		if (auto it = m_Commands.find(hash); it != m_Commands.end())
			return it->second;
		return nullptr;
	}

	void Commands::SaveStateImpl(nlohmann::json& state)
	{
		for (auto& command : m_Commands)
		{
			if (!command.second->ShouldSaveState())
				continue;

			if (!state.contains(command.second->GetName()))
				state[command.second->GetName()] = nlohmann::json::object();

			command.second->SaveState(state[command.second->GetName()]);
		}
	}

	void Commands::LoadStateImpl(nlohmann::json& state)
	{
		for (auto& command : m_Commands)
		{
			if (!command.second->ShouldSaveState())
				continue;

			if (state.contains(command.second->GetName()))
				command.second->LoadState(state[command.second->GetName()]);
		}
	}

	void Commands::ShutdownImpl()
	{
		for (auto& command : m_LoopedCommands)
			if (command->GetState())
				command->Shutdown();
	}
}