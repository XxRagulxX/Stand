#include "Commands/Stand/CommandWeb.hpp"

#include "Commands/Stand/CommandWebOpen.hpp"

namespace Stand
{
	CommandWeb::CommandWeb(CommandList* const parent)
		: CommandListQrcode(parent, LOC("WEB2"), {}, LOC("WEB2_H"))
	{
		this->createChild<CommandWebOpen>();
	}

	std::string CommandWeb::getText() const
	{
		return getUri();
	}

	std::string CommandWeb::getUri()
	{
		return "https://stand.sh/web";
	}
}
