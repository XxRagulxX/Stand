#pragma once

#include "Commands/Widgets/CommandWithEntityPreview.hpp"
#include "Commands/Widgets/CommandListFolderItemData.hpp"
#include "Commands/Widgets/CommandAction.hpp"

#include "Vehicle/ColonsVehicleCustomisationIo.hpp"

namespace Stand
{
	class CommandSavedVehicle : public CommandWithEntityPreview<CommandListFolderItem<CommandAction, CommandSavedVehicle>>
	{
	public:
		CommandListFolderItemData folder_item_data;
		ColonsVehicleCustomisationIo data{};

		explicit CommandSavedVehicle(CommandList* parent, std::wstring&& name, CommandListFolderItemData&& folder_item_data);

	private:
		bool ensureData();

	public:
		void onClick(Click& click) final;

	protected:
		[[nodiscard]] Hash getModelHash() final;
		[[nodiscard]] AbstractEntity createPreviewEntity() final;
	};
}
