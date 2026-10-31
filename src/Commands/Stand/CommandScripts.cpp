#include "Commands/Stand/CommandScripts.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"

#include "Commands/Widgets/CommandReadonlyLink.hpp"

namespace Stand
{
	CommandScripts::CommandScripts(CommandList* parent)
		: CommandListFolder(parent, L"Scripts", { L".txt" }, LOC("CMDSCRPT"))
	{
		instance = this;

		CommandPhysical::collapse_command_names.emplace(L"exec");
	}

	CommandScripts::~CommandScripts()
	{
		if (instance == this)
		{
			instance = nullptr;
		}
	}

	void CommandScripts::populateHead()
	{
		createOpenFolderAction();

		createChild<CommandReadonlyLink>(LOC("CMDBXDCS"), "https://commands.chat/");
		createChild<CommandReadonlyLink>(LOC("CMDLST"), "https://stand.sh/commands");
	}

	void CommandScripts::postPopulateBody() const
	{
	}

	CommandListFolderItemData* CommandScripts::getItemData(Command* cmd) const noexcept
	{
		return nullptr;
	}

	void CommandScripts::onItemIsKept(Command* cmd) const
	{
	}

	void CommandScripts::createItem(CommandList* list, std::wstring&& filename_no_ext, CommandListFolderItemData&& data) const
	{
	}
}
