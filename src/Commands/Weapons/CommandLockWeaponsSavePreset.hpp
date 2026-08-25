#pragma once

#include "Commands/Stand/CommandSaveCommandBoxPreset.hpp"

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandLockWeaponsSavePreset : public CommandSaveCommandBoxPreset
	{
	public:
		using CommandSaveCommandBoxPreset::CommandSaveCommandBoxPreset;

		bool performSave(Click& click, std::ofstream& out) const final
		{
			this->parent->parent->recursiveForEachChild([&out, this](const std::unique_ptr<Command>& c)
			{
				auto physical = c->getPhysical();

				if (!physical || physical->command_names.empty())
				{
					return true;
				}

				const auto cmdname = cmdNameToUtf8(physical->command_names.at(0));

				if (c->isToggle())
				{
					out << fmt::format("{} {}\n", cmdname, c->as<CommandToggle>()->m_on ? "on" : "off");
				}
				else if (c->isSlider())
				{
					out << fmt::format("{} {}\n", cmdname, std::to_string(c->as<CommandSlider>()->value));
				}
				else if (c->isListNameshare())
				{
					out << fmt::format("{} {}\n", cmdname, cmdNameToUtf8(c->as<CommandListSelect>()->getCurrentValueCommand()->command_suffixes.at(0)));
				}

				return true;
			});

			return true;
		}
	};
}