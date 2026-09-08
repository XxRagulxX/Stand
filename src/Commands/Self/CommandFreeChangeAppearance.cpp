#include "Commands/CommandToggleLegacy.hpp"
#include "Network/Tunables.hpp"

namespace Stand::Features
{

	class FreeChangeAppearance : public CommandToggleLegacy
	{
		using CommandToggleLegacy::CommandToggleLegacy;

		Tunable m_CharacterAppearanceCharge{"CHARACTER_APPEARANCE_CHARGE"_J};

		virtual void OnEnable() override
		{
			if (m_CharacterAppearanceCharge.IsReady())
				m_CharacterAppearanceCharge.Set(0);
		}

		virtual void OnDisable() override
		{
			if (m_CharacterAppearanceCharge.IsReady())
				m_CharacterAppearanceCharge.Set(100000);
		}
	};

	static FreeChangeAppearance _FreeChangeAppearance{"freechangeappearance", "Change Appearance For Free", "Allows the player to change the character's appearance for free"};
}