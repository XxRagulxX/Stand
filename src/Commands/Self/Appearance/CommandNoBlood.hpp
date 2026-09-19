#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandNoBlood final : public CommandToggle
	{
	public:
		explicit CommandNoBlood(CommandList* parent)
			: CommandToggle(parent, LIT("No Blood"), CMDNAMES("noblood"), LIT("Keeps your character free of blood."))
		{
		}

		void onEnable(Click& click) final;
		void onDisable(Click& click) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
