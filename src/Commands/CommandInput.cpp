#include "Commands/CommandInput.hpp"

namespace Stand
{
	void CommandInput::OnCall()
	{
	}

	void CommandInput::SaveState(nlohmann::json& value)
	{
		value = m_StringValue;
	}

	void CommandInput::LoadState(nlohmann::json& value)
	{
		m_StringValue = value.get<std::string>();
	}

	CommandInput::CommandInput(std::string name, std::string label, std::string description, std::string defaultValue) :
	    CommandLegacy(name, label, description, 0),
	    m_StringValue(defaultValue)
	{
	}

	std::string CommandInput::GetString() const
	{
		return m_StringValue;
	}

	void CommandInput::SetStringValue(const std::string& value)
	{
		m_StringValue = value;
		OnChange();
		MarkDirty();
	}
}
