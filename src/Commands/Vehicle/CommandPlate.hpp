#pragma once

#include "Commands/Widgets/CommandInputTextLimitChars.hpp"

#include "lib/soup/string.hpp"

#include "Core/AbstractEntity.hpp"
#include "Commands/Stand/CommandExtraInfo.hpp"
#include "Util/get_next_arg.hpp"
#include "Rendering/Gui.hpp"
#include "Game/natives.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlate : public CommandInputTextLimitChars
	{
	public:
		static inline bool locked = false;

		explicit CommandPlate(CommandList* const parent)
			: CommandInputTextLimitChars(parent, LOC("TEXT"), CMDNAMES_OBF("plate"), NOLABEL, 8)
		{
		}

		void onCommandInner(Click& click, std::wstring& args) final
		{
			if (args.empty())
			{
				return onClick(click);
			}

			auto val = StringUtils::utf16_to_utf8(args);
			args.clear();

			if (!isProperPlate(click, val))
			{
				return onClick(click);
			}
			else
			{
				setValue(std::move(val));
			}

			if (auto veh = Util::getVehicle(click); veh.isValid())
			{
				ensureScriptThread(click, [this, veh]() mutable
				{
					veh.setPlateText(value.c_str());
				});
			}
		}

		void onTickInGameViewport() final
		{
			if (!locked)
			{
				auto veh = Util::getVehicle();
				std::string plate{};
				auto* pPlateText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
				if (pPlateText != nullptr)
				{
					plate = pPlateText;
					soup::string::trim(plate);
				}
				setValue(std::move(plate));
			}
		}

		void onTickInWebViewport() final
		{
			return onTickInWebViewportImplRedirect();
		}

	private:
		[[nodiscard]] static bool isProperPlate(Click& click, const std::string& plate)
		{
			for (auto& c : plate)
			{
				if (!soup::string::isAlphaNum(c) && c != ' ')
				{
					click.setResponse(LIT(LANG_FMT("BADCHAR", c)));
					return false;
				}
			}

			return true;
		}
	};
}
