#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	struct CommandPlayer
	{
		inline static bool force_flag_update = false;
		inline static const wchar_t* flag_prefix = L" [";
		inline static const wchar_t* flag_suffix = L"]";
	};

	class CommandSelectBrackets : public CommandListSelect
	{
	public:
		explicit CommandSelectBrackets(CommandList* const parent)
			: CommandListSelect(parent, LOC("BRCKTS"), {}, NOLABEL, {
				{0, LIT("STD")},
				{1, LIT("[STD]")},
				{2, LIT("(STD)")},
				{3, LIT("{STD}")},
				{4, LIT("<STD>")},
			}, 1)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			switch (value)
			{
			case 0:
				CommandPlayer::flag_prefix = L" ";
				CommandPlayer::flag_suffix = L"";
				break;

			default:
				CommandPlayer::flag_prefix = L" [";
				CommandPlayer::flag_suffix = L"]";
				break;

			case 2:
				CommandPlayer::flag_prefix = L" (";
				CommandPlayer::flag_suffix = L")";
				break;

			case 3:
				CommandPlayer::flag_prefix = L" {";
				CommandPlayer::flag_suffix = L"}";
				break;

			case 4:
				CommandPlayer::flag_prefix = L" <";
				CommandPlayer::flag_suffix = L">";
				break;
			}
			CommandPlayer::force_flag_update = true;
		}
	};
}
