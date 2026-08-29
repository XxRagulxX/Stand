#include "Commands/Extra/CommandPackages.hpp"

#include "Commands/Widgets/CommandListRefreshable.hpp"
#include "Game/PackageMgr.hpp"
#include "Config/Tunables.hpp"

#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Extra/CommandPackage.hpp"

namespace Stand
{
	CommandPackages::CommandPackages(CommandList* parent)
		: CommandList(parent, LOC("REPO"), {}, LOC("REPO_H_LUA"))
	{
		for (auto& pkg : PackageMgr::repo.packages)
		{
			if (pkg.isLua() && pkg.type == Package::STANDALONE && pkg.shouldShowToUser())
			{
				createChild<CommandPackage>(&pkg);
			}
		}
		createChild<CommandDivider>(LOC("DEPS"));
		for (auto& pkg : PackageMgr::repo.packages)
		{
			if (pkg.isLua() && pkg.type != Package::STANDALONE && pkg.shouldShowToUser())
			{
				createChild<CommandPackage>(&pkg);
			}
		}
		createChild<CommandDivider>(LOC("CMPTLYS"));
		for (auto& package : PackageMgr::repo.packages)
		{
			if (package.isLuaCompatLayer())
			{
				createChild<CommandPackage>(&package);
			}
		}
	}

	void CommandPackages::onBack(ThreadContext thread_context)
	{
		parent->as<CommandListRefreshable>()->refresh();
	}
}
