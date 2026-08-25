#include "Commands/Stand/CommandListCommandBoxPresets.hpp"
#include "Core/Exceptional.hpp"
#include "Util/get_appdata_path.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandSaveCommandBoxPreset::CommandSaveCommandBoxPreset(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, std::wstring&& folder_name)
		: CommandActionName(parent, std::move(menu_name), std::move(command_names), std::move(help_text)), folder_name(folder_name)
	{
	}

	// Has destructible locals (std::filesystem::path, temporary std::wstring), so this must not itself contain a __try.
	static void ensurePresetFolderAndAppend(std::wstring& prefill, const std::wstring& folder_name)
	{
		std::filesystem::path path = get_appdata_path(fmt::format(L"Scripts\\{}\\", folder_name));
		if (!std::filesystem::exists(path))
		{
			std::filesystem::create_directory(path);
		}
		prefill.append(folder_name + L"\\");
	}

	// No destructible locals here: only references, so it's safe to __try in this function.
	static void tryEnsurePresetFolderAndAppend(std::wstring& prefill, const std::wstring& folder_name)
	{
		__try
		{
			ensurePresetFolderAndAppend(prefill, folder_name);
		}
		__EXCEPTIONAL()
		{
		}
	}

	void CommandSaveCommandBoxPreset::onClick(Click& click)
	{
		std::wstring prefill{ cmdNameToUtf16(command_names.at(0)) };
		prefill.push_back(L' ');
		tryEnsurePresetFolderAndAppend(prefill, folder_name);
		click.showCommandBoxIfPossible(std::move(prefill));
	}

	void CommandSaveCommandBoxPreset::onCommand(Click& click, std::wstring& args)
	{
		if (!preCheck(click))
		{
			args.clear();
			return;
		}

		std::ofstream out(get_appdata_path(fmt::format(L"Scripts\\{}.txt", args)));
		args.clear();

		if (out.fail())
		{
			click.setResponse(LOC("BADFILE"));
			g_gui.showCommandBox(std::move(cmdNameToUtf16(command_names.at(0)).append(args)), click);
			return;
		}

		if (performSave(click, out))
		{
			this->parent->as<CommandListCommandBoxPresets<CommandList>>()->refreshScriptsLink();
		}
	}
}
