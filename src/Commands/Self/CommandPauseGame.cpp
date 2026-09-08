#include "Commands/CommandToggleLegacy.hpp"
#include "Core/Pointers.hpp"

namespace Stand::Features
{
	class PauseGame : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		virtual void OnEnable() override
		{
			Pointers.AllowPausingInSessionPatch->Apply();
		}

		virtual void OnDisable() override
		{
			Pointers.AllowPausingInSessionPatch->Restore();
		}
	};

	static PauseGame _PauseGame{"pausegame", "Pause Game", "Allows you to pause the game in online (local only)."};
}