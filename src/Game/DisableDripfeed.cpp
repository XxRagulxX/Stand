#include "Game/DisableDripfeed.hpp"

#include "lib/soup/macros.hpp"

#include "Game/atStringHash.hpp"
#include "Game/script_func.hpp"

namespace Stand
{
	void DisableDripfeed::onTick()
	{
		if (enabled)
		{
			patch.onTick(ATSTRINGHASH("appinternet"), FUNC_AI_CANPRCHS, ScriptFuncPatch::patchBytesReturnTrue_2, COUNT(ScriptFuncPatch::patchBytesReturnTrue_2));
		}
	}

	void DisableDripfeed::onDisable()
	{
		patch.onDisable(ATSTRINGHASH("appinternet"));
	}
}
