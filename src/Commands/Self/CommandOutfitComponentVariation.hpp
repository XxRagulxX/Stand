#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Ped/outfit_data.hpp"

namespace Stand
{
	class CommandOutfitComponentVariation : public CommandSlider
	{
	public:
		const PedComponent* const data;

		explicit CommandOutfitComponentVariation(CommandList* parent, const PedComponent* data)
			: CommandSlider(parent, LIT(data->var_label), std::vector<CommandName>{data->var_cmd_name}, NOLABEL, -1, 0, 0, 1, 0)
			, data(data)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}

		void onDrawableChange();
		void updateRange();
	};
}
