#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandTabESP : public CommandList
	{
	public:
		CommandToggle* const drawPlayers;
		CommandToggle* const drawDeadPlayers;
		CommandToggle* const namePlayers;
		CommandToggle* const distancePlayers;
		CommandToggle* const skeletonPlayers;

		CommandToggle* const drawPeds;
		CommandToggle* const drawDeadPeds;
		CommandToggle* const modelPeds;
		CommandToggle* const netInfoPeds;
		CommandToggle* const scriptInfoPeds;
		CommandToggle* const distancePeds;
		CommandToggle* const skeletonPeds;

		CommandToggle* const drawObjects;
		CommandToggle* const netInfoObjects;
		CommandToggle* const scriptInfoObjects;
		CommandToggle* const distanceObjects;

		explicit CommandTabESP()
			: CommandList(nullptr, LIT("ESP"), CMDNAMES("esp")),
			  drawPlayers(createChild<CommandToggle>(
			      LIT("Draw Players"), CMDNAMES("espdrawplayers"), NOLABEL)),
			  drawDeadPlayers(createChild<CommandToggle>(
			      LIT("Draw Dead Players"), CMDNAMES("espdrawdeadplayers"), NOLABEL)),
			  namePlayers(createChild<CommandToggle>(
			      LIT("Player Name"), CMDNAMES("espnameplayers"), NOLABEL)),
			  distancePlayers(createChild<CommandToggle>(
			      LIT("Player Distance"), CMDNAMES("espdistanceplayers"), NOLABEL)),
			  skeletonPlayers(createChild<CommandToggle>(
			      LIT("Player Skeleton"), CMDNAMES("espskeletonplayers"), NOLABEL)),
			  drawPeds(createChild<CommandToggle>(
			      LIT("Draw Peds"), CMDNAMES("espdrawpeds"), NOLABEL)),
			  drawDeadPeds(createChild<CommandToggle>(
			      LIT("Draw Dead Peds"), CMDNAMES("espdrawdeadpeds"), NOLABEL)),
			  modelPeds(createChild<CommandToggle>(
			      LIT("Ped Hashes"), CMDNAMES("espmodelspeds"), NOLABEL)),
			  netInfoPeds(createChild<CommandToggle>(
			      LIT("Ped Network Info"), CMDNAMES("espnetinfopeds"), NOLABEL)),
			  scriptInfoPeds(createChild<CommandToggle>(
			      LIT("Ped Script Info"), CMDNAMES("espscriptinfopeds"), NOLABEL)),
			  distancePeds(createChild<CommandToggle>(
			      LIT("Ped Distance"), CMDNAMES("espdistancepeds"), NOLABEL)),
			  skeletonPeds(createChild<CommandToggle>(
			      LIT("Ped Skeleton"), CMDNAMES("espskeletonpeds"), NOLABEL)),
			  drawObjects(createChild<CommandToggle>(
			      LIT("Draw Special Objects"), CMDNAMES("espdrawobjects"), NOLABEL)),
			  netInfoObjects(createChild<CommandToggle>(
			      LIT("Object Network Info"), CMDNAMES("espnetinfoobjects"), NOLABEL)),
			  scriptInfoObjects(createChild<CommandToggle>(
			      LIT("Object Script Info"), CMDNAMES("espscriptinfoobjects"), NOLABEL)),
			  distanceObjects(createChild<CommandToggle>(
			      LIT("Object Distance"), CMDNAMES("espdistanceobjects"), NOLABEL))
		{}
	};
}

namespace Stand::Features
{
	Stand::CommandTabESP& GetCommandTabESP();
}
