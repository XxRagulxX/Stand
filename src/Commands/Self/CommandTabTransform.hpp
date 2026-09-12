#pragma once
#include "Commands/Self/CommandTransformModel.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandTabTransformPlayable : public CommandList
	{
	public:
		explicit CommandTabTransformPlayable(CommandList* parent);
	};

	class CommandTabTransformGroundAnimals : public CommandList
	{
	public:
		explicit CommandTabTransformGroundAnimals(CommandList* parent);
	};

	class CommandTabTransformWaterAnimals : public CommandList
	{
	public:
		explicit CommandTabTransformWaterAnimals(CommandList* parent);
	};

	class CommandTabTransformFlyingAnimals : public CommandList
	{
	public:
		explicit CommandTabTransformFlyingAnimals(CommandList* parent);
	};

	class CommandTabTransformNPCs : public CommandList
	{
	public:
		explicit CommandTabTransformNPCs(CommandList* parent);
	};

	class CommandFindModel : public CommandPhysical
	{
		CommandTabTransformNPCs* const m_npcs;

	public:
		CommandFindModel(CommandList* parent, CommandTabTransformNPCs* npcs)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Search"), CMDNAMES("findmodel"), LIT("Search for a model by name clue."))
			, m_npcs(npcs)
		{
		}

		void onCommand(Click& click, std::wstring& args) override;
	};

	class CommandTabTransform : public CommandList
	{
	public:
		CommandTabTransformPlayable* const playable;
		CommandTabTransformGroundAnimals* const groundAnimals;
		CommandTabTransformWaterAnimals* const waterAnimals;
		CommandTabTransformFlyingAnimals* const flyingAnimals;
		CommandTabTransformNPCs* const npcs;
		CommandFindModel* const findModel;

		explicit CommandTabTransform()
			: CommandList(nullptr, LIT("Transform")),
			  playable(createChild<CommandTabTransformPlayable>()),
			  groundAnimals(createChild<CommandTabTransformGroundAnimals>()),
			  waterAnimals(createChild<CommandTabTransformWaterAnimals>()),
			  flyingAnimals(createChild<CommandTabTransformFlyingAnimals>()),
			  npcs(createChild<CommandTabTransformNPCs>()),
			  findModel(createChild<CommandFindModel>(npcs))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabTransform& GetCommandTabTransform();
}
