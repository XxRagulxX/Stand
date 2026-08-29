#include "Commands/Stand/CommandListPatches.hpp"

#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "AntiCheat/Hooking.hpp"

namespace Stand
{
	CommandListPatches::CommandListPatches(CommandList* const parent)
		: CommandList(parent, LOC("PTCHS"))
	{
		this->createChild<CommandTogglePointer>(&g_hooking.patch_projectile_pool, LOC("PRJTPOOL"), {}, LOC("PTCH_PRJTPOOL_H"));
	}
}
