#pragma once
#include "Commands/CommandToggleLegacy.hpp"

#include <functional>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandLambdaToggle (Commands/Widgets/
	// CommandLambdaToggle.hpp, verified against origin/stand-reference) -
	// a toggle whose behaviour is supplied inline as callbacks rather
	// than needing its own subclass file the way every existing
	// CommandToggle-derived feature in this codebase does (see Godmode.cpp/
	// CommandNoRagdoll.cpp) - the single biggest thing slowing down
	// porting Stand's own commands one at a time, since Stand's own
	// source leans on this exact pattern constantly (e.g.
	// CommandTabSelf.cpp's own standonvehicles-style one-off toggles).
	//
	// Split into separate onEnable/onDisable callbacks rather than
	// Stand's single onChange(bool, Click&) - matches this project's own
	// CommandToggle::OnEnable()/OnDisable() split (every existing feature
	// file already follows this shape) and there's no Click& to thread
	// through anyway (this project has no Click system - see this
	// folder's own sibling files for the same note).
	class CommandLambdaToggle : public CommandToggleLegacy
	{
	public:
		CommandLambdaToggle(std::string name,
		    std::string label,
		    std::string description,
		    std::function<void()> onEnable,
		    std::function<void()> onDisable = nullptr,
		    bool def_value = false) :
		    CommandToggleLegacy(std::move(name), std::move(label), std::move(description), def_value),
		    m_OnEnable(std::move(onEnable)),
		    m_OnDisable(std::move(onDisable))
		{
		}

	protected:
		void OnEnable() override
		{
			if (m_OnEnable)
				m_OnEnable();
		}

		void OnDisable() override
		{
			if (m_OnDisable)
				m_OnDisable();
		}

	private:
		std::function<void()> m_OnEnable;
		std::function<void()> m_OnDisable;
	};
}
