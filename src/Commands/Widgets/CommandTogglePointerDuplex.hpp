#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/LoopedCommand.hpp"

#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandTogglePointerDuplex (Commands/
	// Widgets/CommandTogglePointerDuplex.hpp on origin/stand-reference) -
	// CommandTogglePointer.hpp's own one-way sync (writes *ptr on user
	// change only), plus the other direction: polls *ptr every tick and
	// updates this toggle's own displayed state to match, for a pointer
	// that something ELSE (a hotkey, a script, another feature) can also
	// flip out from under the menu. Same hidden always-on LoopedCommand
	// ticker shape as CommandSliderRainbow.hpp/CommandToggleAll.hpp, and
	// the same guarded-SetState() pattern (only call it when *ptr's value
	// actually differs from GetState(), so a stable pointer doesn't spam
	// FiberPool::queueJob every tick).
	class CommandTogglePointerDuplex : public CommandToggleLegacy
	{
	public:
		CommandTogglePointerDuplex(std::string name, std::string label, std::string description, bool* ptr) :
		    CommandToggleLegacy(name, label, description, ptr && *ptr),
		    m_Ptr(ptr),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, keeps this reflecting *ptr if something else changes it", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		void Tick()
		{
			if (m_Ptr && *m_Ptr != GetState())
				SetState(*m_Ptr);
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
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandTogglePointerDuplex* owner) :
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
			CommandTogglePointerDuplex* m_Owner;
		};

		bool* m_Ptr;
		Ticker m_Ticker;
	};
}
