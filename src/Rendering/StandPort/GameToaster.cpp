#include "Rendering/StandPort/GameToaster.hpp"

#include "Rendering/StandPort/StandRendererCompat.hpp"
#include "Rendering/StandPort/UnicodePrivateUse.hpp"
#include "Scripting/Natives.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	static int getClosestHudColour(int tr, int tg, int tb, int ta)
	{
		int closest = 0;
		unsigned int closestDist = static_cast<unsigned int>(-1);
		for (int i = 0; i < 250; ++i)
		{
			int r, g, b, a;
			HUD::GET_HUD_COLOUR(i, &r, &g, &b, &a);
			const unsigned int dist = static_cast<unsigned int>(abs(tr - r) + abs(tg - g) + abs(tb - b) + abs(ta - a));
			if (dist < closestDist)
			{
				closest = i;
				closestDist = dist;
				if (dist == 0)
					break;
			}
		}
		return closest;
	}

	int GameToaster::doToast(const std::wstring& message) const
	{
		auto msg{ message };
		UnicodePrivateUse::destroyGta(msg);
		UnicodePrivateUse::toGta(msg);
		StringUtils::replace_all(msg, std::wstring(L"&"), std::wstring(L"&amp;"));
		StringUtils::replace_all(msg, std::wstring(L"<"), std::wstring(L"&lt;"));
		StringUtils::replace_all(msg, std::wstring(L">"), std::wstring(L"&gt;"));
		const auto& c = g_renderer.notifyBorderColour;
		HUD::THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(getClosestHudColour(
			static_cast<int>(c.x * 255.0f),
			static_cast<int>(c.y * 255.0f),
			static_cast<int>(c.z * 255.0f),
			static_cast<int>(c.w * 255.0f)));
		const std::string utf8 = StringUtils::utf16_to_utf8(msg);
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("CELL_EMAIL_BCON");
		const size_t len = utf8.length();
		size_t offset = 0;
		do
		{
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(utf8.substr(offset, 99).c_str());
			offset += 99;
		} while (offset < len);
		return HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, true);
	}

	void GameToaster::toast(const std::wstring& message) const
	{
		doToast(message);
	}

	void GameToaster::setPersistentToast(const std::wstring& message)
	{
		if (persistent != 0)
		{
			clearPersistentToast();
		}
		HUD::THEFEED_FREEZE_NEXT_POST();
		persistent = doToast(message);
	}

	void GameToaster::clearPersistentToast()
	{
		HUD::THEFEED_REMOVE_ITEM(persistent);
		persistent = 0;
	}
}
