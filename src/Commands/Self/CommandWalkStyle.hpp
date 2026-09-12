#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandWalkStyle final : public CommandSlider
	{
		int m_applied = -1;

	public:
		struct Entry
		{
			const char* clipset;
			const char* name;
		};
		static const Entry styles[56];

		explicit CommandWalkStyle(CommandList* parent);
		~CommandWalkStyle() final;

		[[nodiscard]] std::string getValueText() const override;
		void onChange(Click& click, int prev_value) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}
