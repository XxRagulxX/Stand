#pragma once
#include "Rendering/StandPort/CommandReadonlyValueCopy.hpp"
#include "Rendering/StandPort/CommandColourCustom.hpp"

namespace Stand
{
	class CommandCurrentCustomColourHex : public CommandReadonlyValueCopy
	{
	private:
		CommandColourCustom* const colour;

	public:
		explicit CommandCurrentCustomColourHex(CommandList* const parent, CommandColourCustom* const colour)
		    : CommandReadonlyValueCopy(parent, LOC("CURRCLRHEX"), {}, CMDFLAGS_READONLY_VALUE_COPY | CMDFLAG_FEATURELIST_SKIP)
		    , colour(colour)
		{
		}

		void onTickInGameViewport() final
		{
			setValue(colour->getHex());
		}

		void onTickInWebViewport() final {}
		void onPreScriptedAccess() final {}
	};
}
