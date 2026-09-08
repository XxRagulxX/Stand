#include "Core/DetourHook.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Core/Hooks.hpp"
#include "Game/CDynamicEntity.hpp"
#include "Ped/Ped.hpp"
#include "Core/Pointers.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	static CommandToggleLegacy _AimbotReleaseDeadPed{"aimbotreleasedeadped", "Release Dead Target", "Releases the lock-on to the target after they are dead"};

	bool Misc::AssistedAimShouldReleaseEntity(__int64 a1)
	{
		auto entity = *(CDynamicEntity**)(a1 + 0x38);

		if (entity && entity->IsPed() && Ped(entity).IsDead())
		{
			*(CDynamicEntity**)(a1 + 0x38) = nullptr;
			if (!Pointers.AssistedAimFindNewTarget(a1))
			{
				*(CDynamicEntity**)(a1 + 0x38) = entity; // failed, restore original entity to avoid issues
				if (_AimbotReleaseDeadPed.GetState())
					return true;
			}
		}

		return Hooking::Get<Misc::AssistedAimShouldReleaseEntity>()->Original<decltype(&Misc::AssistedAimShouldReleaseEntity)>()(a1);
	}
}