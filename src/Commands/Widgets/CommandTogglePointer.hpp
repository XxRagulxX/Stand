#pragma once
#include "Commands/CommandToggleLegacy.hpp"

#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandTogglePointer - a toggle backed
	// by an existing bool* (typically a static field on some manager
	// class, e.g. real Stand's own AllEntitiesEveryTick::aimbot_los_check)
	// rather than owning its own storage. Real Stand's own version reads
	// *ptr once at construction (as the initial state) and writes back to
	// it on every change - a one-way sync onto an already-virtual
	// OnEnable()/OnDisable() hook, not something needing GetState()/
	// SetState() themselves to be virtual, so this needed no changes to
	// CommandToggle.hpp at all.
	//
	// Why this matters for porting: a lot of Stand's own remaining
	// categories (Weapons > Aimbot, for one) are built entirely out of
	// CommandTogglePointer/CommandSliderPointer (see CommandSliderPointer.hpp)
	// rows bound directly to a shared settings struct's own fields,
	// rather than each row owning independent state - this is the one
	// widget missing to port those directly instead of redesigning them
	// around independent BoolCommands.
	class CommandTogglePointer : public CommandToggleLegacy
	{
	public:
		CommandTogglePointer(std::string name, std::string label, std::string description, bool* ptr) :
		    CommandToggleLegacy(std::move(name), std::move(label), std::move(description), ptr && *ptr),
		    m_Ptr(ptr)
		{
		}

	protected:
		void OnEnable() override
		{
			if (m_Ptr)
				*m_Ptr = true;
		}

		void OnDisable() override
		{
			if (m_Ptr)
				*m_Ptr = false;
		}

	private:
		bool* m_Ptr;
	};
}
