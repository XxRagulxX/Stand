#pragma once
#include "Commands/CommandLegacy.hpp"

#include <string>
#include <vector>

namespace Stand
{
	class CommandTextslider : public CommandLegacy
	{
	protected:
		virtual void OnChange() {};
		virtual void OnCall() override;
		virtual void SaveState(nlohmann::json& value) override;
		virtual void LoadState(nlohmann::json& value) override;

		std::vector<std::string> m_Options;
		int m_Index = 0;

	public:
		CommandTextslider(std::string name, std::string label, std::string description, std::vector<std::string> options, int def_index = 0);
		int GetIndex();
		void SetIndex(int index);
		void Cycle(int direction);
		const std::string& GetCurrentOption() const;
		const std::vector<std::string>& GetOptions() const;
	};
}
