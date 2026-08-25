#pragma once

#include "Commands/Widgets/CommandWithEntityPreview.hpp"
#include "Commands/Widgets/CommandAction.hpp"

#include "Vehicle/vehicle_items.hpp"

namespace Stand
{
	class CommandVehicle : public CommandWithEntityPreview<CommandAction>
	{
	public:
		const VehicleItem* const data;

		explicit CommandVehicle(CommandList* const parent, const VehicleItem* data);

		[[nodiscard]] std::string getNameForConfig() const final;
		void updateName(bool root_readlocked, bool show_manufacturer);

		void onClick(Click& click) final;

	protected:
		[[nodiscard]] Hash getModelHash() final;
		[[nodiscard]] AbstractEntity createPreviewEntity() final;
	};
}
