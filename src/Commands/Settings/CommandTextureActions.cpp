#include "Commands/Widgets/CommandLambdaAction.hpp"

namespace Stand::Features
{
	static StandWidgets::CommandLambdaAction _ReloadTextures{"reloadtextures",
	    "Reload Textures",
	    "Reloads textures from your theme folder.",
	    [] {
	    }};
}
