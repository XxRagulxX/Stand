#include "FmBanner.hpp"

#include <fmt/format.h>

#include "AbstractPlayer.hpp"
#include "ScriptGlobal.hpp"
#include "StringUtils.hpp"
#include "Util.hpp"

namespace Stand
{
	bool FmBanner::isSlotInUse(const int slot)
	{
		return ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_STATE).get<int>() != 0;
	}

	int FmBanner::getFreeSlot(const int fallback)
	{
		for (int i = 0; i != 4; ++i)
		{
			if (!isSlotInUse(i))
			{
				return i;
			}
		}
		return fallback;
	}

	FmBanner FmBanner::fromSlot(const int slot)
	{
		FmBanner b{};
		b.style = ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_STYLE).get<int>();
		strncpy_s(b.headline, sizeof(b.headline), ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_HEADLINE).as<const char*>(), _TRUNCATE);
		strncpy_s(b.subtext, sizeof(b.subtext), ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_SUBTEXT).as<const char*>(), _TRUNCATE);
		strncpy_s(b.customtext_1, sizeof(b.customtext_1), ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_1).as<const char*>(), _TRUNCATE);
		strncpy_s(b.customtext_2, sizeof(b.customtext_2), ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_2).as<const char*>(), _TRUNCATE);
		strncpy_s(b.customtext_3, sizeof(b.customtext_3), ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_3).as<const char*>(), _TRUNCATE);
		b.customint_1 = ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMINT_1).get<int>();
		b.customint_2 = ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMINT_2).get<int>();
		b.player_1 = ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_PLAYER_1).get<int>();
		return b;
	}

	std::string FmBanner::getSubtextString() const
	{
		std::string str = Util::GET_LABEL_TEXT(subtext);

		StringUtils::replace_all(str, "~s~", {});
		StringUtils::replace_all(str, "<C>", {});
		StringUtils::replace_all(str, "</C>", {});

		if (customtext_1[0] != 0)
		{
			StringUtils::replace_single(str, "~a~", customtext_1);
		}
		if (customtext_2[0] != 0)
		{
			StringUtils::replace_single(str, "~a~", Util::GET_LABEL_TEXT(customtext_2));
		}
		if (customtext_3[0] != 0)
		{
			StringUtils::replace_single(str, "~a~", Util::GET_LABEL_TEXT(customtext_3));
		}
		if (player_1 != -1)
		{
			StringUtils::replace_single(str, "~a~", AbstractPlayer(player_1).getName());
		}

		if (customint_1 != -1)
		{
			switch (style)
			{
			case 49:
			case 79:
			case 80:
			case 50:
				{
					int secs = customint_1 / 1000;
					StringUtils::replace_single(str, "~a~", fmt::format("{:0>2}:{:0>2}.{:0>3}", secs / 60, secs % 60, customint_1 % 1000));
				}
				break;

			default:
				StringUtils::replace_single(str, "~1~", fmt::to_string(customint_1));
				break;
			}
		}

		StringUtils::replace_single(str, "~1~", fmt::to_string(customint_2));

		return str;
	}

	void FmBanner::display(const int slot)
	{
		ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_STATE).set<int>(1);

		ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_STYLE).set<int>(style);
		strncpy_s(ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_HEADLINE).as<char*>(),sizeof(headline), headline, _TRUNCATE);
		strncpy_s(ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_SUBTEXT).as<char*>(), sizeof(subtext), subtext, _TRUNCATE);
		strncpy_s(ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_1).as<char*>(), sizeof(customtext_1), customtext_1, _TRUNCATE);
		strncpy_s(ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_2).as<char*>(), sizeof(customtext_2), customtext_2, _TRUNCATE);
		strncpy_s(ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMTEXT_3).as<char*>(), sizeof(customtext_3), customtext_3, _TRUNCATE);
		ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMINT_1).set<int>(customint_1);
		ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_CUSTOMINT_2).set<int>(customint_2);
		ScriptGlobal(GLOBAL_FMBNR_BASE).at(slot, GLOBAL_FMBNR_SIZE).at(GLOBAL_FMBNR_PLAYER_1).set<int>(player_1);
	}
}
