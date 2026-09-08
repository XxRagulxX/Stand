#include "Commands/LoopedCommand.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/ScriptGlobal.hpp"

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
			*ScriptGlobal(1970586).As<bool*>() = true;
			*ScriptGlobal(1970586).At(3).As<int*>() = (1 << 3) | (1 << 1);
		}

		virtual void OnDisable() override
		{
			*Pointers.HasGTAPlus = m_OldGTAPlus;
			*ScriptGlobal(1970058).As<bool*>() = false;
			*ScriptGlobal(1970058).At(3).As<int*>() = 2;
		}
	};

	static UnlockGTAPlus _UnlockGTAPlus{"unlockgtaplus", "Unlock GTA+", "Force-unlocks GTA+ content. Use with caution"};
}