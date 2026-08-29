#pragma once

#include "Commands/Widgets/CommandWithEntityPreview.hpp"
#include "Commands/Extra/CommandActionScript.hpp"

#include "Commands/Widgets/CommandListFolderItemData.hpp"
#include "Game/natives_decl.hpp"

namespace Stand
{
	class CommandOutfit : public CommandWithEntityPreview<CommandListFolderItem<CommandActionScript, CommandOutfit>>
	{
	private:
		Hash made_for_model = 0;
		bool current_model_indicator = false;
	public:
		const std::wstring outfit_name;
		CommandListFolderItemData folder_item_data;
		std::unordered_map<std::string, std::string> outfit;

	public:
		explicit CommandOutfit(CommandList* const parent, std::wstring&& outfit_name, CommandListFolderItemData&& folder_item_data);

	private:
		bool ensureOutfit();

	public:
		void onTickInGameViewport() final;
		void onTickInWebViewport() final;

		void onClickScriptThread(Click& click) final;

	protected:
		[[nodiscard]] Hash getModelHash() final;
		[[nodiscard]] AbstractEntity createPreviewEntity() final;
	};
}
