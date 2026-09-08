#pragma once
#include "Commands/CommandLegacy.hpp"

namespace Stand
{
	class CommandInput : public CommandLegacy
	{
	protected:
		std::string m_StringValue;

		virtual void OnChange()
		{
		}

		virtual void OnCall() override;
		virtual void SaveState(nlohmann::json& value) override;
		virtual void LoadState(nlohmann::json& value) override;

	public:
		CommandInput(std::string name, std::string label, std::string description, std::string defaultValue = "");

		std::string GetString() const;
		void SetStringValue(const std::string& value);
	};
}
