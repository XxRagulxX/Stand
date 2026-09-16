#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandBGSelectEntry : public CommandPhysical
	{
		int* m_target;
		int  m_value;
	public:
		CommandBGSelectEntry(CommandList* parent, Label&& label, std::vector<CommandName>&& names, int value, int* target)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(label), std::move(names), NOLABEL)
			, m_target(target), m_value(value)
		{}
		void onClick(Click&) override { *m_target = m_value; }
	};

	class CommandBodyguardFormation : public CommandList
	{
		int m_formation = 0;
	public:
		explicit CommandBodyguardFormation(CommandList* parent)
			: CommandList(parent, LIT("Formation"), CMDNAMES("bodyguardformation"))
		{
			createChild<CommandBGSelectEntry>(LIT("None"),               CMDNAMES("bodyguardformation none"),      0, &m_formation);
			createChild<CommandBGSelectEntry>(LIT("Line"),               CMDNAMES("bodyguardformation line"),      1, &m_formation);
			createChild<CommandBGSelectEntry>(LIT("Circle"),             CMDNAMES("bodyguardformation circle"),    2, &m_formation);
			createChild<CommandBGSelectEntry>(LIT("Alternative Circle"), CMDNAMES("bodyguardformation altcircle"), 3, &m_formation);
		}
		[[nodiscard]] int GetFormation() const { return m_formation; }
	};

	class CommandBodyguardAggression : public CommandList
	{
		int m_aggression = 1;
	public:
		explicit CommandBodyguardAggression(CommandList* parent)
			: CommandList(parent, LIT("Aggression"), CMDNAMES("bodyguardaggression"))
		{
			createChild<CommandBGSelectEntry>(LIT("Friendly"),   CMDNAMES("bodyguardaggression friendly"),
				0, &m_aggression)->help_text = LIT("Defends you and others from AI attackers. Won't attack players.");
			createChild<CommandBGSelectEntry>(LIT("Defensive"),  CMDNAMES("bodyguardaggression defensive"),
				1, &m_aggression)->help_text = LIT("Defends you from any hostile targets.");
			createChild<CommandBGSelectEntry>(LIT("Aggressive"), CMDNAMES("bodyguardaggression aggressive"),
				2, &m_aggression)->help_text = LIT("Defends you from hostile targets, but often engages on sight.");
		}
		[[nodiscard]] int GetAggression() const { return m_aggression; }
	};

	class CommandBodyguardBehaviour : public CommandList
	{
	public:
		CommandBodyguardFormation* const formation;
		CommandBodyguardAggression* const aggression;
		CommandToggle* const ignorePlayers;
		CommandToggle* const ignoreFriends;
		CommandToggle* const ignoreAuthorities;
		CommandToggle* const ignoreCrewMembers;
		CommandToggle* const ignoreOrgMembers;

		explicit CommandBodyguardBehaviour(CommandList* parent)
			: CommandList(parent, LIT("Behaviour"), CMDNAMES("bodyguardbehaviour"))
			, formation(createChild<CommandBodyguardFormation>())
			, aggression(createChild<CommandBodyguardAggression>())
			, ignorePlayers(createChild<CommandToggle>(LIT("Exclude Players"),
				  CMDNAMES("bodyguardsignoreplayers"),
				  LIT("Prevents your bodyguards from attacking this demographic.")))
			, ignoreFriends(createChild<CommandToggle>(LIT("Exclude Friends"),
				  CMDNAMES("bodyguardsignorefriends"),
				  LIT("Prevents your bodyguards from attacking this demographic.")))
			, ignoreAuthorities(createChild<CommandToggle>(LIT("Exclude Authorities"),
				  CMDNAMES("bodyguardsignoreauthorities"),
				  LIT("Prevents your bodyguards from attacking this demographic.")))
			, ignoreCrewMembers(createChild<CommandToggle>(LIT("Exclude Crew Members"),
				  CMDNAMES("bodyguardsignorecrewmembers"),
				  LIT("Prevents your bodyguards from attacking this demographic.")))
			, ignoreOrgMembers(createChild<CommandToggle>(LIT("Exclude Organisation Members"),
				  CMDNAMES("bodyguardsignoreorganisationmembers"),
				  LIT("Prevents your bodyguards from attacking this demographic.")))
		{}
	};
}
