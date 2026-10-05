#include "Rendering/StandPort/CommandReadonlyValueCopy.hpp"
#include "Rendering/Clipboard.hpp"
#include "lib/soup/unicode.hpp"

namespace Stand
{
	void CommandReadonlyValueCopy::onClick(Click& click)
	{
		Rendering::Clipboard::SetText(soup::unicode::utf16_to_utf8(value));
	}
}
