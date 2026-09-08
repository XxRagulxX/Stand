#pragma once
#include "Commands/CommandLegacy.hpp"

#include <functional>
#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandReadonlyValueLambda (Commands/
	// Widgets/CommandReadonlyValueLambda.hpp on origin/stand-reference) -
	// a read-only display row whose text is recomputed from a supplied
	// lambda every time it's read, rather than a fixed string set once
	// (real Stand's own plain CommandReadonlyValue/CommandReadonlyValueCopy
	// cover the "fixed string" case - this project's existing GridItemText
	// already covers that one, just not as a real Command: no joaat-hash
	// lookup, no GetLabel()/GetDescription(), nothing else can find it by
	// name - so only the live/lambda-driven half needed porting).
	//
	// This class only holds the lambda; the actual row
	// (GridItemCommandReadonlyValue, Rendering/
	// GridItemCommandReadonlyValue.hpp) is what calls GetValue() every
	// frame and copies it to the clipboard on click - ported from real
	// Stand's own CommandReadonlyValueCopy (Commands/Widgets/
	// CommandReadonlyValueCopy.hpp), folded into the same row rather than
	// kept as a separate class, since a read-only value with nothing to
	// copy would be a strange thing to show at all.
	class CommandReadonlyValue : public CommandLegacy
	{
	public:
		CommandReadonlyValue(std::string name, std::string label, std::string description, std::function<std::string()> valueFn) :
		    CommandLegacy(std::move(name), std::move(label), std::move(description), 0),
		    m_ValueFn(std::move(valueFn))
		{
		}

		std::string GetValue() const
		{
			return m_ValueFn ? m_ValueFn() : std::string();
		}

	protected:
		// Nothing to do on a direct Call() (e.g. from Lua/console) -
		// clicking the row copies via the GridItem instead (see this
		// class's own comment above), same as real Stand's own
		// CommandReadonlyValueCopy's click handler being what copies,
		// not onCommand().
		void OnCall() override
		{
		}

	private:
		std::function<std::string()> m_ValueFn;
	};
}
