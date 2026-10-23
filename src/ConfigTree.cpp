#include "ConfigTree.hpp"

#include <filesystem>
#include "lib/soup/cat.hpp"

#include "Commands/Stand/CommandLuaScript.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/RecursiveScopedSpinlock.hpp"
#include "Rendering/Gui.hpp"
#include "Util/Util.hpp"

#include <fmt/format.h>
#include <fstream>

namespace Stand
{
	void ConfigTree::loadImpl(std::unordered_map<std::string, std::string>&& data)
	{
		loadImplTree(std::move(data));
	}

	bool ConfigTree::canSaveNow() const
	{
		return g_gui.isRootStateFull()
			&& !g_gui.root_update_in_progress
			&& !CommandLuaScript::hold_up_save
			;
	}

	void ConfigTree::saveToFileNow()
	{
		if (!canSaveNow())
			return;

		auto buf = std::string();
		g_gui.root_mtx.lockRead();
		buf.append("Tree Compatibility Version: ");
		buf.append(fmt::to_string(Util::menu_names_version));
		buf.push_back('\n');
		saveImpl(buf, {}, {}, g_gui.root_list.get(), [](){});
		g_gui.root_mtx.unlockRead();

		auto path = getPath();
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		std::ofstream file_out(path);
		file_out << std::move(buf);
		file_out.close();
	}

	void ConfigTree::saveImpl(std::string& buf, const std::string& path_prefix, const std::string& file_prefix, const CommandList* list, std::function<void()>&& first_entry_func) const
	{
		bool any_entry_in_list = false;
		for (const auto& _command : list->children)
		{
			if (!_command->isPhysical()
				|| !_command->canBeResolved()
				)
			{
				continue;
			}
			auto* command = (const CommandPhysical*)_command.get();
			if ((command->flags & CMDFLAG_STATE_AND_CHECK_FINISHLIST)
				&& (command->flags & CMDFLAG_HOTKEYS_FINISHLIST)
				)
			{
				break;
			}
			std::string name = command->getNameForConfig();
			std::string path = std::move(std::string(path_prefix).append(name));
			bool had_entry = false;
			if (std::string value; getCommandValue(command, path, value))
			{
				if (!any_entry_in_list)
				{
					first_entry_func();
					any_entry_in_list = true;
				}
				buf.append(file_prefix);
				{
					std::string n = command->getNameForConfig();
					soup::cat::encodeName(n);
					buf.append(std::move(n));
				}
				buf.append(": ");
				soup::cat::encodeValue(value);
				buf.append(std::move(value));
				buf.push_back('\n');
				had_entry = true;
			}
			if (command->isList())
			{
				path.push_back('>');
				std::string file_prefix_{ file_prefix };
				file_prefix_.push_back('\t');
				saveImpl(buf, path, std::move(file_prefix_), (const CommandList*)command, [&any_entry_in_list, first_entry_func, had_entry, &buf, file_prefix, name{ std::move(name) }]() mutable
				{
					if (!any_entry_in_list)
					{
						first_entry_func();
						any_entry_in_list = true;
					}
					if (!had_entry)
					{
						buf.append(file_prefix);
						buf.append(name);
						buf.push_back('\n');
					}
				});
			}
		}
	}
}
