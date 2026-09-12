#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandPreserveWalkStyle final : public CommandToggle
	{
	public:
		explicit CommandPreserveWalkStyle(CommandList* parent)
			: CommandToggle(parent, LIT("Preserve Walk Style"), CMDNAMES("preservewalkstyle"), LIT("Prevents your character from switching to combat movement during violence."))
		{
		}

		void onEnable(Click& click) final;
		void onDisable(Click& click) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
