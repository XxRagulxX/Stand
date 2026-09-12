#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandAnimation.hpp"
#include "Game/Animations.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandListAnimations final : public CommandList
	{
	public:
		explicit CommandListAnimations(CommandList* parent)
			: CommandList(parent, LIT("Animations"), CMDNAMES("animations"))
		{
			auto* dances = createChild<CommandList>(LIT("Dances"), CMDNAMES("dances"));
			for (const auto& d : Animations::dances)
				dances->createChild<CommandAnimation<ComplexAnimation>>(d);

			for (const auto& a : Animations::animations)
				createChild<CommandAnimation<ComplexAnimation>>(a);
		}
	};
}
