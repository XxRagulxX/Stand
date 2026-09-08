#pragma once
#include "Commands/CommandSliderLegacy.hpp"

#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandSliderPointer - see
	// CommandTogglePointer.hpp's own doc comment for the shared reasoning (same
	// one-way sync, onto CommandSlider::OnChange() instead of CommandToggle's
	// OnEnable()/OnDisable() - no changes to CommandSlider.hpp needed
	// either).
	class CommandSliderPointer : public CommandSliderLegacy
	{
	public:
		CommandSliderPointer(std::string name, std::string label, std::string description, int* ptr, int min, int max) :
		    CommandSliderLegacy(std::move(name), std::move(label), std::move(description), min, max, ptr ? *ptr : min),
		    m_Ptr(ptr)
		{
		}

	protected:
		void OnChange() override
		{
			if (m_Ptr)
				*m_Ptr = GetState();
		}

	private:
		int* m_Ptr;
	};
}
