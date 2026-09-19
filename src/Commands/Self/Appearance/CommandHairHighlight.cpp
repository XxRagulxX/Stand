#include "Commands/Self/Appearance/CommandHairHighlight.hpp"

#include "Commands/Self/Appearance/CommandLockOutfit.hpp"

namespace Stand
{
	void CommandHairHighlight::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, []
		{
			CommandLockOutfit::setHairColourFromCommands();
		});
	}
}
