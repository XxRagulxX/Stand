#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Ped/outfit_data.hpp"

namespace Stand
{
	class CommandOutfitComponent : public CommandSlider
	{
	public:
		const PedComponent* const data;

		explicit CommandOutfitComponent(CommandList* parent, const PedComponent* data)
			: CommandSlider(parent, LIT(data->label), std::vector<CommandName>{data->cmd_name}, NOLABEL, -1, 0, 0, 1, 0)
			, data(data)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}
	};
}
