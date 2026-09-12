#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Ped/outfit_data.hpp"

namespace Stand
{
	class CommandOutfitPropVariation : public CommandSlider
	{
	public:
		const PedProp* const data;

		explicit CommandOutfitPropVariation(CommandList* parent, const PedProp* data)
			: CommandSlider(parent, LIT(data->var_label), std::vector<CommandName>{data->var_cmd_name}, NOLABEL, -1, 0, -1, 1, 0)
			, data(data)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}

		void onDrawableChange();
		void updateRange();
	};
}
