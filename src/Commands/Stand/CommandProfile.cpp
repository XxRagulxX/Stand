#include "Commands/Stand/CommandProfile.hpp"

#include "Commands/Stand/CommandProfileDelete.hpp"
#include "Commands/Stand/CommandProfileLoad.hpp"
#include "Commands/Stand/CommandProfileSave.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	CommandProfile::CommandProfile(CommandList* const parent, std::wstring&& name, CommandListFolderItemData&& folder_item_data)
		: CommandListFolderItem(parent, LIT(std::move(name)), {}, NOLABEL, CMDFLAGS_LIST | CMDFLAG_FEATURELIST_SKIP), folder_item_data(std::move(folder_item_data))
	{
		this->createChild<CommandProfileSave>();
		this->createChild<CommandProfileLoad>();
		this->createChild<CommandProfileDelete>();
	}

	std::wstring CommandProfile::getProfileNameW() const
	{
		auto name = getRelativeFilePath();
		name.erase(name.size() - 4, 4);
		return name;
	}

	std::wstring CommandProfile::getConfigName() const
	{
		auto name = getProfileNameW();
		name.insert(0, L"Profiles\\");
		return name;
	}
}
