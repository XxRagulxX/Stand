#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandRootName : public CommandList
	{
	private:
		int m_idx = 0;

		static constexpr const char* k_names[] = {
			"StandEnhanced",
			"Stand",
			"Self",
			"Vehicle",
			"Online",
			"Players",
			"World",
			"Game",
			"Hidden",
			"Stand 26.10.1",
			"Stand 26.10.1 (Ultimate)",
			"Stand 26.10.1 Team Lavender",
			"Stand 26.10.1 Red Edition",
			"Stand 26.10.1 Model X",
			"Stand \U0001F4AF",
			"Stand 26.10.1 Early Access",
			"Sit 26.10.1",
		};
		static constexpr int k_count = 17;

	public:
		explicit CommandRootName(CommandList* const parent)
			: CommandList(parent, LOC("RN"), {}, NOLABEL, CMDFLAGS_LIST, COMMAND_LIST_SELECT)
		{
			for (int i = 0; i < k_count; ++i)
			{
				createChild<CommandLambdaAction>(LIT(k_names[i]), std::vector<CommandName>{}, NOLABEL, [this, i](Click& click) {
					m_idx = i;
					Rendering::Theme::kAddressBarRootName = k_names[i];
				});
			}
		}

		[[nodiscard]] Label getCurrentValueMenuName() const override
		{
			return LIT(k_names[m_idx]);
		}

		bool onLeft(Click& click, bool) override
		{
			m_idx = (m_idx - 1 + k_count) % k_count;
			Rendering::Theme::kAddressBarRootName = k_names[m_idx];
			return true;
		}

		bool onRight(Click& click, bool) override
		{
			m_idx = (m_idx + 1) % k_count;
			Rendering::Theme::kAddressBarRootName = k_names[m_idx];
			return true;
		}
	};
}
