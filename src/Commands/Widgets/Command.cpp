#include "Commands/Widgets/Command.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"

#include <algorithm>
#include <vector>

namespace Stand
{
	CommandPhysical* Command::getPhysical() noexcept
	{
		return isPhysical() ? as<CommandPhysical>() : nullptr;
	}

	const CommandPhysical* Command::getPhysical() const noexcept
	{
		return isPhysical() ? as<CommandPhysical>() : nullptr;
	}

	bool Command::shouldShowUntrimmedName() const
	{
		if (type == COMMAND_LIST_SELECT)
			return as<CommandListSelect>()->getCurrentValueHelpText().empty();
		return true;
	}

	std::string Command::getPathConfig() const
	{
		std::vector<std::string> parts;
		const Command* node = this;
		while (node)
		{
			if (node->isPhysical())
			{
				auto* p = static_cast<const CommandPhysical*>(node);
				if (!p->command_names.empty())
					parts.push_back(cmdNameToUtf8(p->command_names.front()));
				else
					parts.push_back(p->menu_name.getLocalisedUtf8());
			}
			node = static_cast<const Command*>(node->parent);
		}
		std::reverse(parts.begin(), parts.end());
		std::string result;
		for (const auto& part : parts)
		{
			if (!result.empty()) result += '.';
			result += part;
		}
		return result;
	}

	std::string Command::getLocalisedAddress(const std::string& separator) const
	{
		std::vector<std::string> parts;
		const Command* node = this;
		while (node)
		{
			if (node->isPhysical())
				parts.push_back(static_cast<const CommandPhysical*>(node)->menu_name.getLocalisedUtf8());
			node = static_cast<const Command*>(node->parent);
		}
		std::reverse(parts.begin(), parts.end());
		std::string result;
		for (const auto& part : parts)
		{
			if (!result.empty()) result += separator;
			result += part;
		}
		return result;
	}

	std::wstring Command::getLocalisedAddressW(const std::wstring& separator) const
	{
		auto utf8 = getLocalisedAddress(std::string(separator.begin(), separator.end()));
		return std::wstring(utf8.begin(), utf8.end());
	}

	std::wstring Command::getEnglishAddressW(const std::wstring& separator) const
	{
		return getLocalisedAddressW(separator);
	}

	void Command::openHotkeysList(ThreadContext thread_context)
	{
	}
}
