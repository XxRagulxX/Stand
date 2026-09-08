#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/LoopedCommand.hpp"

#include <string>
#include <utility>
#include <vector>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandToggleAll (Commands/Widgets/
	// CommandToggleAll.hpp on origin/stand-reference) - a master toggle
	// for a list of sibling CommandToggles: shows "on" only while every
	// sibling is on, refreshed continuously, and turning it on/off sets
	// every sibling to match.
	//
	// Real Stand's own version recomputes its on-indication by directly
	// checking every sibling's own state each time it draws its row -
	// this project's CommandToggle::GetState() isn't virtual (nothing
	// else needs it to be), so there's no equivalent seam to hook a
	// live-recompute into without changing CommandToggle.hpp itself for
	// every other toggle in the codebase. Solved the same way
	// CommandSliderRainbow.hpp/CommandTogglePointerDuplex.hpp already
	// do: a hidden always-on LoopedCommand ticker keeps this toggle's own
	// m_State synced to "are all siblings on" every tick (via a guarded
	// SetState() call, same "only call it when the value actually
	// changed" guard CommandMenuMouseMove's own Tick() uses, so this
	// doesn't spam FiberPool::queueJob every tick while stable), so
	// GetState() reads correctly through the ordinary, unmodified
	// CommandToggle base.
	class CommandToggleAll : public CommandToggleLegacy
	{
	public:
		CommandToggleAll(std::string name, std::string label, std::string description, std::vector<CommandToggleLegacy*> siblings) :
		    CommandToggleLegacy(name, label, description, false),
		    m_Siblings(std::move(siblings)),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, keeps this reflecting whether every sibling toggle is on", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		void Tick()
		{
			bool allOn = !m_Siblings.empty();
			for (auto* sibling : m_Siblings)
			{
				if (!sibling || !sibling->GetState())
				{
					allOn = false;
					break;
				}
			}

			if (allOn != GetState())
				SetState(allOn);
		}

	protected:
		// Fired both by a real user click AND by Tick()'s own SetState()
		// call above whenever the collective state crosses the all-on/
		// all-off boundary from siblings changing individually - re-
		// applying the same state to every sibling in that second case is
		// a harmless, guarded no-op (they already match), not a loop:
		// SetState() below only ever runs when GetState() actually
		// changes value.
		void OnEnable() override
		{
			for (auto* sibling : m_Siblings)
				if (sibling && !sibling->GetState())
					sibling->SetState(true);
		}

		void OnDisable() override
		{
			for (auto* sibling : m_Siblings)
				if (sibling && sibling->GetState())
					sibling->SetState(false);
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandToggleAll* owner) :
			    LoopedCommand(std::move(name), std::move(label), std::move(description)),
			    m_Owner(owner)
			{
				m_State = true;
			}

			bool ShouldSaveState() const override
			{
				return false;
			}

		protected:
			void OnTick() override
			{
				m_Owner->Tick();
			}

		private:
			CommandToggleAll* m_Owner;
		};

		std::vector<CommandToggleLegacy*> m_Siblings;
		Ticker m_Ticker;
	};
}
