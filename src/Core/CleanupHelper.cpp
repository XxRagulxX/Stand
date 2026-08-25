#include "Core/CleanupHelper.hpp"

#include "lib/soup/netIntel.hpp"

#include "Game/Animations.hpp"
#include "Network/BattlEyeServer.hpp"
#include "Rendering/Camgr.hpp"
#include "Game/DisableDripfeed.hpp"
#include "World/Firemangr.hpp"
#include "Rendering/Gui.hpp"
#include "Core/input.hpp"
#include "Core/tbLoadingSessionDetails.hpp"
#include "Game/natives.hpp"
#include "AntiCheat/NativeTableHooks.hpp"
#include "AntiCheat/NativeTableHooksBuiltin.hpp"
#include "Network/SessionSpoofing.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Vehicle/VehicleModelFlags.hpp"

namespace Stand
{
	void CleanupHelper::execNonScript()
	{
		if (!did_non_script_cleanup)
		{
			did_non_script_cleanup = true;

			Input::scheme = InputSchemeNumpad();

			if (SessionSpoofing::copy_from != nullptr)
			{
				SessionSpoofing::copy_from = nullptr;
				SessionSpoofing::onDisable();
			}

			DisableDripfeed::onDisable();
		}

		if (g_gui.inited_netIntel)
		{
			g_gui.inited_netIntel = false;
			g_gui.getNetIntel().deinit();
		}
	}

	void CleanupHelper::execScript()
	{
		Camgr::cleanup();

		NativeTableHooksBuiltin::deinit();

		{
			for (const auto& script_fire : Firemangr::script_fires)
			{
				FIRE::REMOVE_SCRIPT_FIRE(script_fire);
			}
			Firemangr::script_fires.clear();
		}

		g_tb_loading_session_details.forceDisable();

		VehicleModelFlags::resetAll();

		if (g_gui.user_was_in_online)
		{
			*ScriptGlobal(GLOBAL_DISABLE_DRIPFEED).as<BOOL*>() = FALSE;
		}

		Animations::reset();

		if (BattlEyeServer::isRunningDueToUs())
		{
			BattlEyeServer::stop();
		}
	}

	void CleanupHelper::execScriptForce()
	{
		// In case of force unload, don't leave natives tables dirty.
		NativeTableHooks::forceCleanup();
	}
}
