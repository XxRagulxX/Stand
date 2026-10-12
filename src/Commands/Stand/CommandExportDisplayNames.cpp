#include "Commands/Stand/CommandExportDisplayNames.hpp"

#include "Core/FileLogger.hpp"

namespace Stand
{
	CommandExportDisplayNames::CommandExportDisplayNames(CommandList* parent)
		: CommandAction(parent, LIT("Export Display Names"))
	{
	}

	void CommandExportDisplayNames::onClick(Click& click)
	{
		g_logger.log(std::string("---- Start Of Exported Display Names ----"));
		g_logger.log(std::string("---- End Of Exported Display Names ----"));
		click.setResponse(LIT("Your Log.txt just gained a lot of weight."));
	}
}
