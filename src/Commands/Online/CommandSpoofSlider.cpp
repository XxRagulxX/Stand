#include "Commands/Online/CommandSpoofSlider.hpp"

#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Online/CommandSpoof.hpp"

namespace Stand
{
	template <>
	void CommandSpoofSlider<CommandSlider>::onChange(Click& click, int prev_value)
	{
		reinterpret_cast<CommandSpoof<CommandSlider>*>(parent)->onValueChange();
	}

	template <>
	void CommandSpoofSlider<CommandSliderFloat>::onChange(Click& click, int prev_value)
	{
		reinterpret_cast<CommandSpoof<CommandSliderFloat>*>(parent)->onValueChange();
	}

	static_assert(sizeof(CommandSpoof<CommandSlider>) == sizeof(CommandSpoof<CommandSliderFloat>)); // CommandSpoofToggle relies on this, although it's only the vftable, but whatever.
}
