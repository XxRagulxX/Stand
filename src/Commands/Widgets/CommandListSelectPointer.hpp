#pragma once
#include "Commands/CommandListSelect.hpp"

#include <utility>
#include <vector>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandListSelectPointer - the
	// CommandListSelect equivalent of CommandTogglePointer.hpp/CommandSliderPointer.hpp (see
	// either's own doc comment for the shared reasoning: a one-way sync
	// onto CommandListSelect's own OnChange() hook, no changes to
	// CommandListSelect.hpp needed).
	class CommandListSelectPointer : public CommandListSelect
	{
	public:
		CommandListSelectPointer(std::string name,
		    std::string label,
		    std::string description,
		    std::vector<std::pair<int, const char*>> list,
		    int* ptr) :
		    CommandListSelect(std::move(name), std::move(label), std::move(description), std::move(list), ptr ? *ptr : 0),
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
