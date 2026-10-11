#include "Commands/Widgets/CommandHeader.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"

#include "Rendering/Gui.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"

namespace Stand
{
	CommandHeader::CommandHeader(CommandList* const parent)
		: CommandListSelect(parent, LOC("HDR"), CMDNAMES("header"), NOLABEL, {
			{0,   LOC("2HIDE"), CMDNAMES("hide", "gone")},
			{23,  LIT("Stand And Fight by TGP")},
			{22,  LIT("A Tribute by Super Saiyan Ginger")},
			{21,  LIT("Logo Concept 5 by Hollywood Collins")},
			{11,  LIT("Minimalistic by PootisGodAnimations")},
			{20,  LIT("Rockstar Admin Tools")},
			{10,  LIT("Paragon by Denzy")},
			{15,  LIT("Paragon (Purple) by Denzy")},
			{14,  LIT("Paragon (Cyan) by Denzy")},
			{16,  LIT("GlitchFav4 by SourGrapeApe")},
			{12,  LIT("Quantum by Denzy")},
			{9,   LIT("Amogus by boah")},
			{13,  LIT("JoJo by Spicy")},
			{6,   LIT("Shitty Attempt #2 by super saiyan ginger")},
			{18,  LIT("Logo Concept 3 by Hollywood Collins")},
			{19,  LIT("Stan by synn420")},
			{17,  LIT("Take-Two Anti-Profit Tool")},
			{1,   LIT("WillMa Techno by Cahya")},
			{2,   LIT("Untitled (2) by moue")},
			{3,   LIT("epic3dblenderrendernightmare by Thimo")},
			{4,   LIT("Komp 1 by Cahya")},
			{5,   LIT("Shitty Attempt #1 by super saiyan ginger")},
			{7,   LIT("Shitty Attempt #3 by super saiyan ginger")},
			{100, LIT("Untitled Animated by moue")},
			{200, LOC("CUS"), CMDNAMES("custom")},
		}, 0)
	{
	}

	void CommandHeader::onChange(Click& click, long long prev_value)
	{
		if (click.type == CLICK_AUTO || !g_gui.canUpdateThemeSetting())
		{
			return;
		}
		applyValue();
	}

	void CommandHeader::setState(Click& click, const std::string& state)
	{
		if (state == "Stand And Fight by PRB Dess")
		{
			return CommandListSelect::setState(click, "Stand And Fight by TGP");
		}
		return CommandListSelect::setState(click, state);
	}

	bool CommandHeader::applyValue() const
	{
		g_MenuGrid.updateSetting([] {});
		return true;
	}
}
