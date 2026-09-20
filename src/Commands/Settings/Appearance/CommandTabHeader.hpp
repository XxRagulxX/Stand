#pragma once
#include "Commands/CommandListSelect.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/FileMgr.hpp"
#include "Menu/Click.hpp"
#include "Rendering/HeaderBanner.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand
{
	// Ported from real Stand's own CommandHeader
	// (origin/dev src/Commands/Widgets/CommandHeader.hpp/.cpp).
	//
	// Real Stand downloads named presets from stand.sh on demand.
	// This port instead resolves each preset to a filename in the local
	// Headers folder (same path the real Stand menu uses for its own
	// cache). If a preset file is not found the header is simply hidden.
	// The "Custom" option loads the Custom subfolder (all files, animated
	// like real Stand's value-200 branch).
	class CommandHeaderMode : public CommandListSelect
	{
		// Mapping from list value → filename inside the Headers folder.
		// Matches real Stand's own unanimated_headers[] order/names
		// (CommandHeader.cpp, origin/dev).
		struct Preset { int id; const char* filename; };
		static constexpr Preset kPresets[] = {
			{  1, "Stand And Fight by TGP.png"                           },
			{  2, "A Tribute by Super Saiyan Ginger.png"                 },
			{  3, "Logo Concept 5 by Hollywood Collins.png"              },
			{  4, "Minimalistic by PootisGodAnimations.png"              },
			{  5, "Rockstar Admin Tools.png"                             },
			{  6, "Paragon by Denzy.png"                                 },
			{  7, "Paragon (Purple) by Denzy.png"                        },
			{  8, "Paragon (Cyan) by Denzy.png"                          },
			{  9, "GlitchFav4 by SourGrapeApe.png"                      },
			{ 10, "Quantum by Denzy.png"                                 },
			{ 11, "Amogus by boah.jpg"                                   },
			{ 12, "JoJo by Spicy.png"                                    },
			{ 13, "Shitty Attempt #2 by super saiyan ginger.jpg"         },
			{ 14, "Logo Concept 3 by Hollywood Collins.png"              },
			{ 15, "Stan by synn420.jpg"                                  },
			{ 16, "Take-Two Anti-Profit Tool.png"                        },
			{ 17, "WillMa Techno by Cahya.jpg"                           },
			{ 18, "Untitled (2) by moue.png"                             },
			{ 19, "epic3dblenderrendernightmare by Thimo.png"            },
			{ 20, "Komp 1 by Cahya.jpg"                                  },
			{ 21, "Shitty Attempt #1 by super saiyan ginger.jpg"         },
			{ 22, "Shitty Attempt #3 by super saiyan ginger.jpg"         },
			// Animated multi-frame folder (value ≥ 100 = subfolder load)
			{ 100, "Untitled Animated by moue"                           },
		};

	public:
		explicit CommandHeaderMode(CommandList* const parent)
			: CommandListSelect(
				"header",
				"Header",
				"Show a header image above the menu. Preset files are looked up in the Headers folder (same path the real Stand menu uses).",
				{
					{   0, "Hide"                                              },
					{   1, "Stand And Fight by TGP"                           },
					{   2, "A Tribute by Super Saiyan Ginger"                 },
					{   3, "Logo Concept 5 by Hollywood Collins"              },
					{   4, "Minimalistic by PootisGodAnimations"              },
					{   5, "Rockstar Admin Tools"                             },
					{   6, "Paragon by Denzy"                                 },
					{   7, "Paragon (Purple) by Denzy"                        },
					{   8, "Paragon (Cyan) by Denzy"                          },
					{   9, "GlitchFav4 by SourGrapeApe"                      },
					{  10, "Quantum by Denzy"                                 },
					{  11, "Amogus by boah"                                   },
					{  12, "JoJo by Spicy"                                    },
					{  13, "Shitty Attempt #2 by super saiyan ginger"         },
					{  14, "Logo Concept 3 by Hollywood Collins"              },
					{  15, "Stan by synn420"                                  },
					{  16, "Take-Two Anti-Profit Tool"                        },
					{  17, "WillMa Techno by Cahya"                           },
					{  18, "Untitled (2) by moue"                             },
					{  19, "epic3dblenderrendernightmare by Thimo"            },
					{  20, "Komp 1 by Cahya"                                  },
					{  21, "Shitty Attempt #1 by super saiyan ginger"         },
					{  22, "Shitty Attempt #3 by super saiyan ginger"         },
					{ 100, "Untitled Animated by moue"                        },
					{ 200, "Custom"                                           },
				},
				0)
		{
		}

	protected:
		void OnChange() override
		{
			const int val = GetState();

			if (val == 0)
			{
				Rendering::HeaderBanner::Clear();
				return;
			}

			const auto headersRoot = FileMgr::GetProjectFolder("Headers").Path();

			if (val == 200)
			{
				// Custom: load all images from the Custom subfolder (animated
				// if multiple files, same as real Stand's value-200 branch).
				const auto customDir = headersRoot / "Custom Header";
				std::filesystem::create_directories(customDir);
				Rendering::HeaderBanner::LoadFromFolder(customDir);
				return;
			}

			// Named preset: resolve to a single file (unanimated).
			for (const auto& p : kPresets)
			{
				if (p.id != val)
					continue;

				if (val >= 100)
				{
					// Animated folder preset.
					const auto dir = headersRoot / p.filename;
					Rendering::HeaderBanner::LoadFromFolder(dir);
				}
				else
				{
					// Single-image preset — uses LoadFromFile so that having
					// other images in the Headers folder does not cause cycling.
					const auto file = headersRoot / p.filename;
					Rendering::HeaderBanner::LoadFromFile(file);
				}
				return;
			}

			Rendering::HeaderBanner::Clear();
		}
	};

	// Ported from real Stand's own CommandHeaderAnimationSpeed
	// (origin/dev src/Commands/Stand/CommandHeaderAnimationSpeed.hpp).
	// Writes to HeaderBanner::SetFrameIntervalMs() rather than the real
	// Stand's own g_renderer.header_speed field (same role, different owner).
	class CommandHeaderAnimationSpeed : public CommandSlider
	{
	public:
		explicit CommandHeaderAnimationSpeed(CommandList* const parent)
			: CommandSlider(parent, LIT("Frame Interval"), CMDNAMES("headerinterval"),
				LIT("Animation speed for multi-frame headers (animated preset or Custom), in milliseconds per frame."),
				1, 10000, 32, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::HeaderBanner::SetFrameIntervalMs(static_cast<int64_t>(value));
		}
	};

	// Ported from real Stand's own CommandTogglePointer to
	// g_renderer.header_bgblur (origin/dev CommandTabStand.cpp).
	// Writes to HeaderBanner::SetBgBlur().
	class CommandHeaderBgBlur : public CommandToggle
	{
	public:
		explicit CommandHeaderBgBlur(CommandList* const parent)
			: CommandToggle(parent, LIT("Background Blur"), CMDNAMES("headerbgblur"),
				LIT("Draws a background blur effect behind the header image."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::HeaderBanner::SetBgBlur(m_on);
		}
	};

	// Opens the Headers folder in Explorer — same action as real Stand's
	// CommandLambdaAction opening get_appdata_path(L"Stand\\Headers\\")
	// (origin/dev CommandTabStand.cpp).
	class CommandHeaderOpenFolder : public CommandPhysical
	{
	public:
		explicit CommandHeaderOpenFolder(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Open Folder"),
				CMDNAMES("openheadersfolder"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			const auto path = FileMgr::GetProjectFolder("Headers").Path();
			std::filesystem::create_directories(path);
			ShellExecuteW(nullptr, L"explore",
				path.wstring().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
		}
	};

	class CommandTabHeader : public CommandList
	{
	public:
		// CommandHeaderMode is a CommandListSelect (different hierarchy from
		// Command) — declared as a static in CommandTabHeader.cpp and
		// self-registered under "header"_J. Not a createChild<> child.
		CommandHeaderAnimationSpeed* const animationSpeed;
		CommandHeaderBgBlur* const bgBlur;
		CommandHeaderOpenFolder* const openFolder;

		explicit CommandTabHeader()
			: CommandList(nullptr, LIT("Header"), CMDNAMES())
			, animationSpeed(createChild<CommandHeaderAnimationSpeed>())
			, bgBlur(createChild<CommandHeaderBgBlur>())
			, openFolder(createChild<CommandHeaderOpenFolder>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabHeader& GetCommandTabHeader();
}
