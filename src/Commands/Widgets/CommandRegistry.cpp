#include "Commands/Widgets/CommandRegistry.hpp"

#include "Commands/Widgets/CommandIssuable.hpp"

namespace Stand
{
	void CommandRegistry::Add(CommandIssuable* command)
	{
		GetInstance().AddImpl(command);
	}

	void CommandRegistry::Remove(CommandIssuable* command)
	{
		GetInstance().RemoveImpl(command);
	}

	void CommandRegistry::AddImpl(CommandIssuable* command)
	{
		if (!command)
			return;

		for (const auto& name : command->command_names)
			m_Commands.insert({Stand::Joaat(name), command});
	}

	void CommandRegistry::RemoveImpl(CommandIssuable* command)
	{
		if (!command)
			return;

		for (const auto& name : command->command_names)
			m_Commands.erase(Stand::Joaat(name));
	}

	CommandIssuable* CommandRegistry::GetCommandImpl(Stand::joaat_t hash)
	{
		if (auto it = m_Commands.find(hash); it != m_Commands.end())
			return it->second;
		return nullptr;
	}
}
