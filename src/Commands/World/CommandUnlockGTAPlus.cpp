#include "Commands/LoopedCommand.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/Globals.hpp"

namespace Stand::Features
{
	class UnlockGTAPlus : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		bool m_OldGTAPlus;

		virtual void OnEnable() override
		{
			m_OldGTAPlus = *Pointers.HasGTAPlus;
		}

		virtual void OnTick() override
		{
			*Pointers.HasGTAPlus = true;
			*Globals::HEIST_DOOMSDAY_ACT1.as<bool*>() = true;
			*Globals::HEIST_DOOMSDAY_ACT1.at(3).as<int*>() = (1 << 3) | (1 << 1);
		}

		virtual void OnDisable() override
		{
			*Pointers.HasGTAPlus = m_OldGTAPlus;
			*Globals::HEIST_PACIFIC_STANDARD.as<bool*>() = false;
			*Globals::HEIST_PACIFIC_STANDARD.at(3).as<int*>() = 2;
		}
	};

	static UnlockGTAPlus _UnlockGTAPlus{"unlockgtaplus", "Unlock GTA+", "Force-unlocks GTA+ content. Use with caution"};
}