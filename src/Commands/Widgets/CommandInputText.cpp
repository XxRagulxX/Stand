#include "Commands/Widgets/CommandInputText.hpp"

#include "Util/lang.hpp"

namespace Stand
{
	std::string CommandInputText::getCommandSyntax() const
	{
		std::string syntax{ LANG_GET("CMD") };
		syntax.append(": ").append(command_names.at(0)).push_back(' ');
		syntax.append(LANG_GET("ARGTXT"));
		return syntax;
	}
}
