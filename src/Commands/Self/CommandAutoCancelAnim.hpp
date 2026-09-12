#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandAutoCancelAnim final : public CommandToggle
	{
	public:
		explicit CommandAutoCancelAnim(CommandList* parent);

		void onEnable(Click& click) override;
		void onDisable(Click& click) override;
		void onTick() override;
		void applyDefaultState() override;
	};
}
