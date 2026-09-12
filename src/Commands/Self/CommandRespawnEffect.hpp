#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandRespawnEffect final : public CommandSlider
	{
		bool m_dead_last_tick = false;

	public:
		explicit CommandRespawnEffect(CommandList* parent);
		~CommandRespawnEffect() final;

		[[nodiscard]] std::string getValueText() const override;
		void onChange(Click& click, int prev_value) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
