#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandAnimation.hpp"
#include "Game/Animations.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandScenarios final : public CommandList
	{
	public:
		explicit CommandScenarios(CommandList* parent)
			: CommandList(parent, LIT("Scenarios"), CMDNAMES("scenarios"))
		{
			for (const auto& s : Animations::scenarios)
				createChild<CommandAnimation<SimpleAnimation>>(s);
		}
	};
}
