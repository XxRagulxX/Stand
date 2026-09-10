#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	static StandWidgets::CommandLambdaAction _InputPresetFlexible{"inputpresetflexible",
	    "Flexible",
	    "",
	    [] {
		    Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::Flexible;
	    }};

	static StandWidgets::CommandLambdaAction _InputPresetTkl{"inputpresettkl",
	    "No Numpad",
	    "",
	    [] {
		    Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::NoNumpad;
	    }};

	static StandWidgets::CommandLambdaAction _InputPresetFull{"inputpresetfull",
	    "Numpad",
	    "",
	    [] {
		    Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::Numpad;
	    }};
}
