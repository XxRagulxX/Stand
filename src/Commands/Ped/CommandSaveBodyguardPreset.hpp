#pragma once

#include "Commands/Stand/CommandSaveCommandBoxPreset.hpp"

#include "Commands/Widgets/CommandListActionItem.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandSaveBodyguardPreset : public CommandSaveCommandBoxPreset
	{
	public:
		using CommandSaveCommandBoxPreset::CommandSaveCommandBoxPreset;

		bool performSave(Click& click, std::ofstream& out) const final
		{
			this->parent->parent->recursiveForEachChild([&out](const std::unique_ptr<Command>& c)
			{
				auto physical = c->getPhysical();

				if (!physical || physical->command_names.empty())
				{
					return true;
				}

				const auto cmdname = cmdNameToUtf8(physical->command_names.at(0));

				if (physical->isToggle())
				{
					out << fmt::format("{} {}\n", cmdname, physical->as<CommandToggle>()->m_on ? "on" : "off");
				}
				else if (physical->isSlider())
				{
					out << fmt::format("{} {}\n", cmdname, std::to_string(physical->as<CommandSlider>()->value));
				}
				else if (physical->isListNameshare())
				{
					out << fmt::format("{} {}\n", cmdname, cmdNameToUtf8(physical->as<CommandListSelect>()->getCurrentValueCommand()->command_suffixes.at(0)));
				}
			  
				return true;
			});

			return true;
		}
	};
}