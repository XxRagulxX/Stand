#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandDeathEffect final : public CommandSlider
	{
		bool m_sent_fx = false;

	public:
		explicit CommandDeathEffect(CommandList* parent);
		~CommandDeathEffect() final;

		[[nodiscard]] std::string getValueText() const override;
		void onChange(Click& click, int prev_value) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
