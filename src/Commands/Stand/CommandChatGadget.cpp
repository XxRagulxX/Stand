#include "Commands/Stand/CommandChatGadget.hpp"

namespace Stand
{
	CommandChatGadget::CommandChatGadget(CommandList* const parent)
		: CommandList(parent, LIT("Chat Gadget"), CMDNAMES("chatgadget"))
	{
	}
}
