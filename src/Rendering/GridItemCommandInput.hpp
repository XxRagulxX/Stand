#pragma once
#include "Rendering/GridItemTextInput.hpp"
#include "Util/Joaat.hpp"

#include <cstddef>
#include <optional>
#include <string>

namespace Stand::Rendering
{
	// A GridItemTextInput wired to a real Stand::CommandInput, looked
	// up by joaat hash - the Grid equivalent of StringCommandItem (src/
	// StringCommandItem.cpp) for the ImGui menu.
	//
	// maxLength (optional) - ported from real Stand's own
	// CommandInputTextLimitChars, rescoped to this field's own inline
	// typing - see GridItemTextInput.hpp's own class comment for why.
	// Left unset (falling back to GridItemTextInput's own 255 default)
	// for every existing call site; a future one that needs a tighter
	// cap (e.g. a plate/name field) passes it here, and gets a
	// rejected-keystroke Notifications::Show() warning for free - real
	// Stand's own LANG_FMT("CHARLIMIT", ...) response message, using
	// this project's own Notifications overlay in place of its console-
	// style command box response line (this project's CommandInput has
	// no command-box pathway of its own - see GridItemTextInput.hpp).
	class GridItemCommandInput : public GridItemTextInput
	{
	public:
		GridItemCommandInput(int16_t width,
		    int16_t height,
		    joaat_t id,
		    std::optional<std::string> labelOverride = std::nullopt,
		    bool scrolling = false,
		    std::optional<std::size_t> maxLength = std::nullopt);

		[[nodiscard]] std::string GetDescription() const override;

	private:
		// GridItemTextInput itself has no notion of a Command at all
		// (see ResolveLabel()'s own comment in the .cpp) - kept here
		// purely so GetDescription() has something to look the command
		// back up by, the same "look it up again rather than plumb a
		// pointer through the base class" trade-off ResolveLabel()/
		// ResolveInitialValue() already make.
		joaat_t m_Id;
	};
}
