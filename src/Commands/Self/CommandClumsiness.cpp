#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <chrono>

namespace Stand::Features
{
	namespace
	{
		// Ported from real Stand's own CommandClumsiness: SET_PED_RAGDOLL_
		// ON_COLLISION is left on continuously, except for a brief window
		// (2 seconds real Stand's own reenable_timer) right after the ped
		// actually ragdolls, otherwise re-enabling it mid-ragdoll snaps
		// the ped back upright immediately instead of letting the ragdoll
		// play out.
		std::chrono::steady_clock::time_point g_ReenableAt{};
	}

	class Clumsiness : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnEnable() override
		{
			// Real Stand's own toast when Gracefulness (its own "grace"
			// toggle - this project's "noragdoll") is already on, since
			// that toggle actively fights this one (it forces ragdoll
			// OFF every tick, this one wants it back on) - see
			// CommandNoRagdoll.cpp.
			if (auto* grace = Commands::GetCommand<CommandToggleLegacy>("noragdoll"_J); grace && grace->GetState())
			{
				Notifications::Show("Clumsiness",
				    "Gracefulness (No Ragdoll) is on - Clumsiness won't work until it's off.",
				    NotificationType::Warning);
			}

			g_ReenableAt = {};
		}

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			const auto handle = Self::GetPed().GetHandle();
			if (PED::IS_PED_RAGDOLL(handle))
			{
				g_ReenableAt = std::chrono::steady_clock::now() + std::chrono::seconds(2);
			}
			else if (std::chrono::steady_clock::now() >= g_ReenableAt)
			{
				PED::SET_PED_RAGDOLL_ON_COLLISION(handle, true);
			}
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			PED::SET_PED_RAGDOLL_ON_COLLISION(Self::GetPed().GetHandle(), false);
		}
	};

	static Clumsiness _Clumsiness{"clumsiness", "Clumsiness", "Makes you ragdoll on collisions"};
}
