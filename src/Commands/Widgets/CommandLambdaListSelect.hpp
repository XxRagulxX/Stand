#pragma once
#include "Commands/CommandListSelect.hpp"

#include <functional>
#include <utility>
#include <vector>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandLambdaListSelect - see
	// CommandLambdaToggle.hpp's own doc comment for why this whole file exists.
	class CommandLambdaListSelect : public CommandListSelect
	{
	public:
		CommandLambdaListSelect(std::string name,
		    std::string label,
		    std::string description,
		    std::vector<std::pair<int, const char*>> list,
		    int def_val,
		    std::function<void(int)> onChange) :
		    CommandListSelect(std::move(name), std::move(label), std::move(description), std::move(list), def_val),
		    m_OnChange(std::move(onChange))
		{
		}

	protected:
		void OnChange() override
		{
			if (m_OnChange)
				m_OnChange(GetState());
		}

	private:
		std::function<void(int)> m_OnChange;
	};
}
