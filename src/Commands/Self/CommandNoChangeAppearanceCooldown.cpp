#include "Commands/CommandToggleLegacy.hpp"
#include "Network/Tunables.hpp"


namespace Stand::Features
{
	class NoChangeAppearanceCooldown : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		Tunable m_CharacterAppearanceCooldown{"CHARACTER_APPEARANCE_COOLDOWN"_J};

		virtual void OnEnable() override
		{
			if (m_CharacterAppearanceCooldown.IsReady())
				m_CharacterAppearanceCooldown.Set(0);
		}

		virtual void OnDisable() override
		{
			if (m_CharacterAppearanceCooldown.IsReady())
				m_CharacterAppearanceCooldown.Set(2880000);
		}
	};

	static NoChangeAppearanceCooldown _NoChangeAppearanceCooldown{"nochangeappearancecooldown", "No Change Appearance Cooldown", "Removes the cooldown for changing appearance"};
}