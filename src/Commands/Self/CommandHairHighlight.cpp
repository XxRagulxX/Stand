#include "Commands/Self/CommandHairHighlight.hpp"

#include "Commands/Self/CommandLockOutfit.hpp"

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
