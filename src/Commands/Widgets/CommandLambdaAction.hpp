#pragma once
#include "Commands/CommandLegacy.hpp"

#include <functional>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandLambdaAction - a one-shot
	// action (button) whose behaviour is supplied inline, the Command
	// equivalent of CommandLambdaToggle.hpp's own doc comment (see there for
	// why this whole file exists). Every existing plain-Command feature
	// in this codebase (CommandHeal.cpp, CommandSuicide.cpp, ...) still
	// needs its own subclass file for a single OnCall() override - this
	// lets a one-off action be dropped in with no subclass at all.
	class CommandLambdaAction : public CommandLegacy
	{
	public:
		CommandLambdaAction(std::string name, std::string label, std::string description, std::function<void()> onCall) :
		    CommandLegacy(std::move(name), std::move(label), std::move(description), 0),
		    m_OnCall(std::move(onCall))
		{
		}

	protected:
		void OnCall() override
		{
			if (m_OnCall)
				m_OnCall();
		}

	private:
		std::function<void()> m_OnCall;
	};
}
