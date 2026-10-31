#include "Commands/Widgets/CommandHeader.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

#include "Core/FileMgr.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/HeaderBanner.hpp"

#include <filesystem>

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
		CommandTickDispatch::AddCommand(this);
	}

	CommandHeader::~CommandHeader()
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	void CommandHeader::onTick()
	{
		const size_t count = Rendering::HeaderBanner::kFrameCount.load();
		if (count == m_lastFrameCount)
			return;
		m_lastFrameCount = count;
		if (count == 0)
		{
			setMenuName(LOC("HDR"));
		}
		else
		{
			auto s = LOC("HDR").getLocalisedUtf8();
			s += " (";
			s += std::to_string(count);
			s += "/";
			s += std::to_string(count);
			s += ")";
			setMenuName(LIT(std::move(s)));
		}
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
		static constexpr struct { int id; const char* filename; } kPresets[] = {
			{ 23, "Stand And Fight by TGP.png"                           },
			{ 22, "A Tribute by Super Saiyan Ginger.png"                 },
			{ 21, "Logo Concept 5 by Hollywood Collins.png"              },
			{ 11, "Minimalistic by PootisGodAnimations.png"              },
			{ 20, "Rockstar Admin Tools.png"                             },
			{ 10, "Paragon by Denzy.png"                                 },
			{ 15, "Paragon (Purple) by Denzy.png"                        },
			{ 14, "Paragon (Cyan) by Denzy.png"                          },
			{ 16, "GlitchFav4 by SourGrapeApe.png"                       },
			{ 12, "Quantum by Denzy.png"                                 },
			{  9, "Amogus by boah.jpg"                                   },
			{ 13, "JoJo by Spicy.png"                                    },
			{  6, "Shitty Attempt #2 by super saiyan ginger.jpg"         },
			{ 18, "Logo Concept 3 by Hollywood Collins.png"              },
			{ 19, "Stan by synn420.jpg"                                  },
			{ 17, "Take-Two Anti-Profit Tool.png"                        },
			{  1, "WillMa Techno by Cahya.jpg"                           },
			{  2, "Untitled (2) by moue.png"                             },
			{  3, "epic3dblenderrendernightmare by Thimo.png"            },
			{  4, "Komp 1 by Cahya.jpg"                                  },
			{  5, "Shitty Attempt #1 by super saiyan ginger.jpg"         },
			{  7, "Shitty Attempt #3 by super saiyan ginger.jpg"         },
			{ 100, "Untitled Animated by moue"                           },
		};

		const int val = static_cast<int>(value);

		if (val == 0)
		{
			Rendering::HeaderBanner::Clear();
			return true;
		}

		const auto headersRoot = FileMgr::GetProjectFolder("Headers");

		if (val == 200)
		{
			const auto customDir = headersRoot / "Custom Header";
			std::filesystem::create_directories(customDir);
			Rendering::HeaderBanner::LoadFromFolder(customDir);
			return true;
		}

		for (const auto& p : kPresets)
		{
			if (p.id != val)
				continue;
			if (val >= 100)
				Rendering::HeaderBanner::LoadFromFolder(headersRoot / p.filename);
			else
				Rendering::HeaderBanner::LoadFromFile(headersRoot / p.filename);
			return true;
		}

		Rendering::HeaderBanner::Clear();
		return true;
	}
}
