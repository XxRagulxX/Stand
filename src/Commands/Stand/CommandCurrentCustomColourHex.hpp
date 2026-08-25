#pragma once

#include "Commands/Widgets/CommandReadonlyValueCopy.hpp"

#include "Commands/Widgets/CommandColourCustom.hpp"

namespace Stand
{
	class CommandCurrentCustomColourHex : public CommandReadonlyValueCopy
	{
	private:
		CommandColourCustom* const colour = nullptr;

	public:
		explicit CommandCurrentCustomColourHex(CommandList* const parent, CommandColourCustom* const colour)
			: CommandReadonlyValueCopy(parent,LOC("CURRCLRHEX"),std::wstring{},CMDFLAGS_READONLY_VALUE_COPY | CMDFLAG_FEATURELIST_SKIP)
		{
		}

		void onTickInGameViewport() final
		{
			setValue(colour->getHex());
		}

		void onTickInWebViewport() final
		{
			return onTickInWebViewportImplRedirect();
		}

		void onPreScriptedAccess() final
		{
			return dispatchOnTickInGameViewportIfNotThisTick();
		}
	};
}