#include "Commands/CommandToggleLegacy.hpp"
#include "Scripting/ScriptPatches.hpp"

namespace Stand::Features
{
	// from https://github.com/TCRoid/StandEnhanced-Lua-RS-Missions/
	class LSCCustomsBypass : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		ScriptPatch m_CanUseVehiclePatch{};
		ScriptPatch m_BlockMenuOptionPatch{};

		virtual void OnEnable() override
		{
			if (!m_CanUseVehiclePatch)
			{
				m_CanUseVehiclePatch = ScriptPatches::AddPatch("carmod_shop"_J, ScriptPointer("CanUseVehiclePatch", "2D ? ? ? ? 38 ? 5D ? ? ? 56 ? ? 71 2E ? ? 5D").Add(5), {0x72, 0x2E, 0x03, 0x01});
			}
			m_CanUseVehiclePatch->Enable();

			if (!m_BlockMenuOptionPatch)
			{
				m_BlockMenuOptionPatch = ScriptPatches::AddPatch("carmod_shop"_J, ScriptPointer("BlockMenuOptionPatch", "2D ? ? ? ? 38 ? 5D ? ? ? 5D ? ? ? 56").Add(5), {0x71, 0x2E, 0x01, 0x01});
			}
			m_BlockMenuOptionPatch->Enable();
		}

		virtual void OnDisable() override
		{
			if (m_CanUseVehiclePatch)
			{
				m_CanUseVehiclePatch->Disable();
			}

			if (m_BlockMenuOptionPatch)
			{
				m_BlockMenuOptionPatch->Disable();
			}
		}
	};

	static LSCCustomsBypass _LSCCustomsBypass{"lsccustomsbypass", "Remove LSC Restrictions", "Removes Los Santos Customs vehicle modification restrictions"};
}