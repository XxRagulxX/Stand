#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandAddressSeparator : public CommandList
	{
	private:
		int m_idx = 0;

		static constexpr const char* k_labels[] = {
			"Stand > Online", "Stand>Online",
			"Stand / Online", "Stand/Online",
			"Stand : Online", "Stand:Online",
			"Stand \\ Online", "Stand\\Online",
		};
		static constexpr const char* k_seps[] = {
			" > ", ">", " / ", "/", " : ", ":", " \\ ", "\\",
		};
		static constexpr int k_count = 8;

	public:
		explicit CommandAddressSeparator(CommandList* const parent)
			: CommandList(parent, LOC("ADDRSEP"), {}, NOLABEL, CMDFLAGS_LIST, COMMAND_LIST_SELECT)
		{
			for (int i = 0; i < k_count; ++i)
			{
				createChild<CommandLambdaAction>(LIT(k_labels[i]), std::vector<CommandName>{}, NOLABEL, [this, i](Click& click) {
					m_idx = i;
					Rendering::Theme::kAddressSeparator = k_seps[i];
				});
			}
		}

		[[nodiscard]] Label getCurrentValueMenuName() const override
		{
			return LIT(k_labels[m_idx]);
		}

		bool onLeft(Click& click, bool) override
		{
			m_idx = (m_idx - 1 + k_count) % k_count;
			Rendering::Theme::kAddressSeparator = k_seps[m_idx];
			return true;
		}

		bool onRight(Click& click, bool) override
		{
			m_idx = (m_idx + 1) % k_count;
			Rendering::Theme::kAddressSeparator = k_seps[m_idx];
			return true;
		}
	};
}
