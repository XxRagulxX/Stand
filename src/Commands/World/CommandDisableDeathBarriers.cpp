#include "Commands/CommandToggleLegacy.hpp"
#include "Scripting/ScriptPatches.hpp"

namespace Stand::Features
{
	class DisableDeathBarriers : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		ScriptPatch m_DeathBarriersPatch{};

		virtual void OnEnable() override
		{
			if (!m_DeathBarriersPatch)
			{
				m_DeathBarriersPatch = ScriptPatches::AddPatch("freemode"_J, ScriptPointer("DeathBarriersPatch", "2D 01 09 00 00 5D ? ? ? 56 ? ? 3A").Add(5), {0x2E, 0x01, 0x00});
			}
			m_DeathBarriersPatch->Enable();
		}


		virtual void OnDisable() override
		{
			if (m_DeathBarriersPatch)
			{
				m_DeathBarriersPatch->Disable();
			}
		}
	};

	static DisableDeathBarriers _DisableDeathBarriers{"disabledeathbarriers", "Disable Death Barriers", "Disables death barriers found under the map. This also prevents dying randomly when spectating someone", true};
}