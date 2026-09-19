#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Ped/outfit_data.hpp"

namespace Stand
{
	class CommandOutfitProp : public CommandSlider
	{
	public:
		const PedProp* const data;

		explicit CommandOutfitProp(CommandList* parent, const PedProp* data)
			: CommandSlider(parent, LIT(data->label), std::vector<CommandName>{data->cmd_name}, NOLABEL, -1, 0, -1, 1, 0)
			, data(data)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}
	};
}
