#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandInvisibility final : public CommandSlider
	{
		int m_applied = -1;

	public:
		explicit CommandInvisibility(CommandList* parent);
		~CommandInvisibility() final;

		[[nodiscard]] std::string getValueText() const override;
		void onChange(Click& click, int prev_value) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
