#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

#include "Rendering/Gui.hpp"
#include "Util/lang.hpp"

namespace Stand
{
	class CommandLang : public CommandListSelect
	{
	private:
		static inline bool working = false;

		[[nodiscard]] static std::vector<CommandListActionItemData> getOptions()
		{
			std::vector<CommandListActionItemData> options{};
			options.emplace_back(CommandListActionItemData{ (long long)LANG_EN, LIT("English"), CMDNAMES("en", "enuk", "engb", "enus") });
			return options;
		}

	public:
		explicit CommandLang(CommandList* const parent)
			: CommandListSelect(parent, LOC("LANG"), CMDNAMES("lang"), LOC("LANG_H"), getOptions(), (long long)LANG_EN)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			if (working)
				return;
			working = true;
			Lang::active_id = (lang_t)value;
			working = false;
		}

		void applyDefaultState() final
		{
			if (!g_gui.isUnloadPending())
			{
				CommandListSelect::applyDefaultState();
			}
		}
	};
}
