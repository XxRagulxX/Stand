#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Commands/Stand/CommandLeftbound.hpp"
#include "Rendering/StandPort/ThemeIcons.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandTabTextures : public CommandList
	{
	public:
		explicit CommandTabTextures(CommandList* parent = nullptr)
			: CommandList(parent, LOC("SPRS"))
		{
			createChild<CommandLeftbound>();
			createChild<CommandLambdaAction>(LOC("RLDSPR"), CMDNAMES("reloadtextures", "reloadsprites"), NOLABEL, [](Click& click)
			{
				click.stopInputIfAllowed();
				Rendering::ThemeIcons::Reload();
			});
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabTextures& GetCommandTabTextures();
}
