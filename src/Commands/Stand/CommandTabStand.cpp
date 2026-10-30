#include "Commands/Stand/CommandTabStand.hpp"

#include "Commands/Stand/CommandMenuPosition.hpp"
#include "Commands/Stand/Appearance/CommandStreamproof.hpp"
#include "Commands/Widgets/CommandHeader.hpp"
#include "Commands/Widgets/CommandHeaderAnimationSpeed.hpp"
#include "Commands/Widgets/CommandHeaderBgBlur.hpp"
#include "Commands/Widgets/CommandHeaderOpenFolder.hpp"
#include "Commands/Stand/CommandMenuRainbow.hpp"
#include "Commands/Widgets/CommandBackgroundColour.hpp"
#include "Commands/Widgets/CommandBlurRightTextColour.hpp"
#include "Commands/Widgets/CommandBlurSpriteColour.hpp"
#include "Commands/Widgets/CommandBlurTextColour.hpp"
#include "Commands/Widgets/CommandColourPointerDirectx.hpp"
#include "Commands/Widgets/CommandColourRainbow.hpp"
#include "Commands/Widgets/CommandFocusRightTextColour.hpp"
#include "Commands/Widgets/CommandFocusSpriteColour.hpp"
#include "Commands/Widgets/CommandFocusTextColour.hpp"
#include "Commands/Widgets/CommandPrimaryColour.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include <climits>
#include "Commands/Stand/CommandApartments.hpp"
#include "Commands/Stand/CommandAutoOpen.hpp"
#include "Commands/Stand/CommandAutoProceedWarningsForCommands.hpp"
#include "Commands/Stand/CommandButtonInstructions.hpp"
#include "Commands/Stand/CommandChatGadget.hpp"
#include "Commands/Stand/CommandClearNotifications.hpp"
#include "Commands/Stand/CommandConsole.hpp"
#include "Commands/Stand/CommandDefault.hpp"
#include "Commands/Stand/CommandDiscord.hpp"
#include "Commands/Stand/CommandEmptyLog.hpp"
#include "Commands/Stand/CommandExportDisplayNames.hpp"
#include "Commands/Stand/CommandFeatureList.hpp"
#include "Commands/Stand/CommandForceWarningRead.hpp"
#include "Commands/Stand/CommandGallery.hpp"
#include "Commands/Stand/CommandHelpPos.hpp"
#include "Commands/Stand/CommandHelpText.hpp"
#include "Commands/Stand/CommandInfoAlign.hpp"
#include "Commands/Stand/CommandInfoDropShadow.hpp"
#include "Commands/Stand/CommandInfoPos.hpp"
#include "Commands/Stand/CommandInfoScale.hpp"
#include "Commands/Stand/CommandInfoTextPadding.hpp"
#include "Commands/Stand/CommandShowCmdOwners.hpp"
#include "Commands/Stand/CommandShowNonuseronly.hpp"
#include "Commands/Stand/CommandSyntax.hpp"
#include "Commands/Widgets/CommandShowSliderBehaviour.hpp"
#include "Commands/Stand/CommandLang.hpp"
#include "Commands/Stand/CommandListMillion.hpp"
#include "Commands/Stand/CommandListNonDefaults.hpp"
#include "Commands/Stand/CommandListStarred.hpp"
#include "Commands/Stand/CommandListTutorial.hpp"
#include "Commands/Stand/CommandNoExtraClear.hpp"
#include "Commands/Stand/CommandOpenStandFolder.hpp"
#include "Commands/Stand/CommandProfiles.hpp"
#include "Commands/Stand/CommandRabbitHole.hpp"
#include "Commands/Stand/CommandRootName.hpp"
#include "Commands/Stand/CommandShowAddressbar.hpp"
#include "Commands/Widgets/CommandAddressbarWidthAffectedByColumns.hpp"
#include "Commands/Widgets/CommandAddressbarWidthAffectedByScrollbar.hpp"
#include "Commands/Widgets/CommandAddressCurrentListOnly.hpp"
#include "Commands/Widgets/CommandAddressSeparator.hpp"
#include "Commands/Widgets/CommandCursorPos.hpp"
#include "Commands/Widgets/CommandCursorPadding.hpp"
#include "Commands/Widgets/CommandCursorPosIncludesDividers.hpp"
#include "Rendering/StandPort/CommandColour.hpp"
#include "Rendering/StandPort/CommandLambdaSlider.hpp"
#include "Commands/Stand/CommandScripts.hpp"
#include "Commands/Stand/CommandSelfChatCommandsFriendsPerms.hpp"
#include "Commands/Stand/CommandShowInt.hpp"
#include "Commands/Stand/CommandShowNavMesh.hpp"
#include "Commands/Stand/CommandShowNavMeshNeighbors.hpp"
#include "Commands/Stand/CommandShowPedTasks.hpp"
#include "Commands/Stand/CommandSuppressGenericResponsesCommandBox.hpp"
#include "Commands/Stand/CommandSuppressGenericResponsesHotkey.hpp"
#include "Commands/Stand/CommandUnload.hpp"
#include "Commands/Stand/CommandWeb.hpp"
#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Widgets/CommandInput.hpp"
#include "Commands/Widgets/CommandLambdaToggle.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListTextSettings.hpp"
#include "Commands/Stand/CommandPreviewColourInSprite.hpp"
#include "Commands/Stand/CommandPreviewColourInList.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"
#include "Commands/Widgets/CommandSearchMenu.hpp"
#include "Commands/Widgets/CommandReadonlyLink.hpp"
#include "Commands/Widgets/CommandReadonlyName.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandScrollbar.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "Rendering/StandPort/CommandSliderPointer.hpp"
#include "Rendering/StandPort/Commandbox.hpp"
#include "Rendering/StandPort/CommandboxGrid.hpp"
#include "Menu/Click.hpp"
#include "Core/ThreadContext.hpp"

namespace Stand
{
	class CommandTabsVisible : public CommandToggle
	{
	public:
		explicit CommandTabsVisible(CommandList* const parent)
			: CommandToggle(parent, LIT("Tabs"), CMDNAMES("tabs"), NOLABEL, true)
		{}
		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsVisible = m_on;
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandTabsPositionMode : public CommandListSelect
	{
	public:
		explicit CommandTabsPositionMode(CommandList* const parent)
			: CommandListSelect(parent, LOC("POS"), CMDNAMES("tabsposition"), NOLABEL, {
				{0, LIT("Left"),   CMDNAMES("left")},
				{1, LIT("Right"),  CMDNAMES("right")},
				{2, LIT("Top"),    CMDNAMES("top")},
				{3, LIT("Bottom"), CMDNAMES("bottom")},
			}, 0)
		{}
		void onChange(Click& click, long long prev_value) override
		{
			using P = Rendering::Theme::TabsPosition;
			switch (value)
			{
			case 1: Rendering::Theme::kTabsPosition = P::Right; break;
			case 2: Rendering::Theme::kTabsPosition = P::Top; break;
			case 3: Rendering::Theme::kTabsPosition = P::Bottom; break;
			default: Rendering::Theme::kTabsPosition = P::Left; break;
			}
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};

	class CommandTabsAlignment : public CommandListSelect
	{
	public:
		explicit CommandTabsAlignment(CommandList* const parent)
			: CommandListSelect(parent, LOC("TA"), CMDNAMES("tabsalignment"), NOLABEL, {
				{0, LIT("Left"),   CMDNAMES("left")},
				{1, LIT("Centre"), CMDNAMES("centre", "center")},
				{2, LIT("Right"),  CMDNAMES("right")},
			}, 0)
		{}
		void onChange(Click& click, long long prev_value) override
		{
			switch (value)
			{
			case 1: Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Centre; break;
			case 2: Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Right; break;
			default: Rendering::Theme::kTabsAlignment = Rendering::Theme::TabsAlignment::Left; break;
			}
		}
	};

	class CommandCursorBorderColour : public CommandColour
	{
	public:
		explicit CommandCursorBorderColour(CommandList* const parent)
			: CommandColour(parent, LIT("Colour"), CMDNAMES("cursorbordercolour"), LIT("The colour of the cursor border."), 255, 255, 255, 255)
		{}
		void onChange(Click& click) override
		{
			auto rgba = getRGBA();
			Rendering::Theme::kCursorBorderColour = { rgba.x, rgba.y, rgba.z, rgba.w };
		}
	};
}

namespace Stand
{
	CommandTabStand::CommandTabStand(CommandList* const parent)
		: CommandTab(parent, TAB_STAND, LIT("Stand"), { CMDNAME("tstand"), CMDNAME("stand") })
	{}

	void CommandTabStand::populate()
	{
		auto settings = this->createChild<CommandList>(LOC("STTNGS"), CMDNAMES("settings"));

		{
			auto appearance = settings->createChild<CommandList>(LOC("VIS"));

			{
				auto colours = appearance->createChild<CommandList>(LOC("CLRS"));
				auto* primary = colours->createChild<CommandPrimaryColour>();
				primary->children.insert(primary->children.begin(), std::make_unique<CommandMenuRainbow>(primary));
				auto* focusText = colours->createChild<CommandFocusTextColour>();
				colours->createChild<CommandFocusRightTextColour>(focusText);
				colours->createChild<CommandFocusSpriteColour>(focusText);
				colours->createChild<CommandBackgroundColour>();
				auto* blurText = colours->createChild<CommandBlurTextColour>();
				colours->createChild<CommandBlurRightTextColour>(blurText);
				colours->createChild<CommandBlurSpriteColour>(blurText);
				auto* hud = colours->createChild<CommandColourPointerDirectx>(
					reinterpret_cast<DirectX::SimpleMath::Color*>(&Rendering::Theme::kHud),
					LOC("CLRHU"), std::vector<CommandName>{ CMDNAME("hud") }, primary);
				hud->createChild<CommandColourRainbow>(LIT("Rainbow Mode"), std::vector<CommandName>{ CMDNAME("hudrainbow") }, Rendering::Theme::kHud);
				{ auto r = std::move(hud->children.back()); hud->children.pop_back(); hud->children.insert(hud->children.begin(), std::move(r)); }
				auto* ar = colours->createChild<CommandColourPointerDirectx>(
					reinterpret_cast<DirectX::SimpleMath::Color*>(&Rendering::Theme::kAr),
					LOC("CLRAR"), std::vector<CommandName>{ CMDNAME("ar") }, primary);
				ar->createChild<CommandColourRainbow>(LIT("Rainbow Mode"), std::vector<CommandName>{ CMDNAME("arrainbow") }, Rendering::Theme::kAr);
				{ auto r = std::move(ar->children.back()); ar->children.pop_back(); ar->children.insert(ar->children.begin(), std::move(r)); }
				auto* minigame = colours->createChild<CommandColourPointerDirectx>(
					reinterpret_cast<DirectX::SimpleMath::Color*>(&Rendering::Theme::kMinigame),
					LOC("CLRMGME"), std::vector<CommandName>{ CMDNAME("minigame") }, primary);
				minigame->createChild<CommandColourRainbow>(LIT("Rainbow Mode"), std::vector<CommandName>{ CMDNAME("minigamerainbow") }, Rendering::Theme::kMinigame);
				{ auto r = std::move(minigame->children.back()); minigame->children.pop_back(); minigame->children.insert(minigame->children.begin(), std::move(r)); }
			}
			position = appearance->createChild<CommandMenuPosition>();
			{
				auto* headerList = appearance->createChild<CommandList>(LOC("HDR"));
				headerList->createChild<CommandHeader>();
				headerList->createChild<CommandHeaderAnimationSpeed>();
				headerList->createChild<CommandHeaderBgBlur>();
				headerList->createChild<CommandHeaderOpenFolder>();
			}
			{
				auto addressbar = appearance->createChild<CommandList>(LOC("ADDRBAR"));
				addressbar->createChild<CommandShowAddressbar>();
				addressbar->createChild<CommandRootName>();
				addressbar->createChild<CommandAddressSeparator>();
				addressbar->createChild<CommandAddressCurrentListOnly>();
				addressbar->createChild<CommandAddressbarWidthAffectedByScrollbar>();
				addressbar->createChild<CommandAddressbarWidthAffectedByColumns>();
				addressbar->createChild<CommandLambdaSlider<CommandSlider>>(LOC("HHT"), CMDNAMES("addressbarheight"), NOLABEL, 0, SHRT_MAX, 24, 1, [](int value, Click& click)
				{
					Rendering::Theme::kAddressBarHeight = static_cast<int16_t>(value);
					Rendering::GridRenderer::InvalidateMenuLayout();
				});
				{
						auto textSettings = addressbar->createChild<CommandList>(LOC("TEXT"), std::vector<CommandName>{ CMDNAME("addressbartext") });
						textSettings->createChild<CommandLambdaSlider<CommandSlider>>(LOC("SCLE"), CMDNAMES("addressbartextscale"), NOLABEL, 1, SHRT_MAX, static_cast<int>(Rendering::Theme::kSmallTextScale * 10000.f), 1, [](int value, Click& click)
						{
							Rendering::Theme::kAddressbarText.scale = value / 10000.f;
						});
						textSettings->createChild<CommandLambdaSlider<CommandSlider>>(LOC("OFF_X"), CMDNAMES("addressbartextxoffset"), NOLABEL, -15, 15, -2, 1, [](int value, Click& click)
						{
							Rendering::Theme::kAddressbarText.x_offset = static_cast<float>(value);
						});
						textSettings->createChild<CommandLambdaSlider<CommandSlider>>(LOC("OFF_Y"), CMDNAMES("addressbartextyoffset"), NOLABEL, -10, 10, 2, 1, [](int value, Click& click)
						{
							Rendering::Theme::kAddressbarText.y_offset = static_cast<float>(value);
						});
					}
				addressbar->createChild<CommandCursorPos>();
				addressbar->createChild<CommandCursorPosIncludesDividers>();
			}
			{
				auto cursor = appearance->createChild<CommandList>(LIT("Cursor"));
				cursor->createChild<CommandCursorPadding>();
				cursor->createChild<CommandDivider>(LIT("Border"));
				cursor->createChild<CommandLambdaSlider<CommandSlider>>(LIT("Width"), CMDNAMES("cursorborderwidth"), NOLABEL, 0, SHRT_MAX, 0, 1, [](int value, Click& click)
				{
					Rendering::Theme::kCursorBorderWidth = static_cast<int16_t>(value);
				});
				cursor->createChild<CommandTogglePointer>(&Rendering::Theme::kCursorBorderRounded, LIT("Rounded"), CMDNAMES("cursorborderrounded"));
				auto* colour = cursor->createChild<CommandCursorBorderColour>();
				colour->createChild<CommandColourRainbow>(LIT("Rainbow Mode"), CMDNAMES("cursorborderrainbow"), Rendering::Theme::kCursorBorderColour);
				{ auto r = std::move(colour->children.back()); colour->children.pop_back(); colour->children.insert(colour->children.begin(), std::move(r)); }
			}
			{
				auto tabs = appearance->createChild<CommandList>(LIT("Tabs"));
				tabs->createChild<CommandTabsVisible>();
				tabs->createChild<CommandLambdaSlider<CommandSlider>>(LIT("Width"), CMDNAMES("tabswidth"), NOLABEL, 1, SHRT_MAX, Rendering::Theme::kSidebarWidth, 1, [](int value, Click& click)
				{
					Rendering::Theme::kTabsWidth = static_cast<int16_t>(value);
					Rendering::GridRenderer::InvalidateMenuLayout();
				});
				tabs->createChild<CommandLambdaSlider<CommandSlider>>(LIT("Height"), CMDNAMES("tabsheight"), NOLABEL, 1, SHRT_MAX, Rendering::Theme::kSidebarEntryHeight, 1, [](int value, Click& click)
				{
					Rendering::Theme::kTabsHeight = static_cast<int16_t>(value);
					Rendering::GridRenderer::InvalidateMenuLayout();
				});
				tabs->createChild<CommandTabsPositionMode>();
				{
					auto text = tabs->createChild<CommandList>(LIT("Text"), CMDNAMES("tabstext"));
					text->createChild<CommandLambdaSlider<CommandSlider>>(LIT("Scale"), CMDNAMES("tabstextscale"), NOLABEL, 1, SHRT_MAX, static_cast<int>(Rendering::Theme::kTextScale * 10000.f), 1, [](int value, Click& click)
					{
						Rendering::Theme::kTabsTextScale = value / 10000.f;
					});
					text->createChild<CommandLambdaSlider<CommandSlider>>(LIT("X Offset"), CMDNAMES("tabstextxoffset"), NOLABEL, -15, 15, 5, 1, [](int value, Click& click)
					{
						Rendering::Theme::kTabsTextXOffset = static_cast<float>(value);
					});
					text->createChild<CommandLambdaSlider<CommandSlider>>(LIT("Y Offset"), CMDNAMES("tabstextyoffset"), NOLABEL, -10, 10, 0, 1, [](int value, Click& click)
					{
						Rendering::Theme::kTabsTextYOffset = static_cast<float>(value);
					});
				}
				tabs->createChild<CommandTabsAlignment>();
				tabs->createChild<CommandTogglePointer>(&Rendering::Theme::kTabsShowLeftIcon, LIT("Show Icon Left"), CMDNAMES("tabslefticon"));
				tabs->createChild<CommandTogglePointer>(&Rendering::Theme::kTabsShowName, LIT("Show Name"), CMDNAMES("tabsname"));
				tabs->createChild<CommandTogglePointer>(&Rendering::Theme::kTabsShowRightIcon, LIT("Show Icon Right"), CMDNAMES("tabsrighticon"));
				tabs->createChild<CommandTogglePointer>(&Rendering::Theme::kTabsIconsUseTextColour, LIT("Icons Use Text Colour"), CMDNAMES("tabsiconsusetext"));
			}
			{
				auto scrollbar = appearance->createChild<CommandList>(LOC("SCRLBR"));
				scrollbar->createChild<CommandScrollbar>();
				scrollbar->createChild<CommandLambdaSlider<CommandSlider>>(LOC("WDT"), CMDNAMES("scrollbarwidth"), NOLABEL, 1, SHRT_MAX, 6, 1, [](int value, Click& click)
				{
					Rendering::Theme::kScrollbarWidthMutable = static_cast<int16_t>(value);
					Rendering::GridRenderer::InvalidateMenuLayout();
				});
			}
			{
				auto cmd_info_text = appearance->createChild<CommandList>(LOC("CMDINFOTXT"));
				cmd_info_text->createChild<CommandHelpPos>();
				cmd_info_text->createChild<CommandLambdaSlider<CommandSlider>>(LOC("WDT"), CMDNAMES("infowidth"), NOLABEL, 1, SHRT_MAX, 300, 1, [](int value, Click& click)
				{
					Rendering::Theme::kInfoWidth = static_cast<int16_t>(value);
					Rendering::GridRenderer::InvalidateMenuLayout();
				});
				cmd_info_text->createChild<CommandInfoTextPadding>();
				cmd_info_text->createChild<CommandHelpText>();
				cmd_info_text->createChild<CommandSyntax>();
				cmd_info_text->createChild<CommandShowSliderBehaviour>();
				cmd_info_text->createChild<CommandShowNonuseronly>();
				cmd_info_text->createChild<CommandShowCmdOwners>();
			}
			notifications = appearance->createChild<CommandListNotifySettings>(nullptr, nullptr);

			{
				auto commands = appearance->createChild<CommandList>(LOC("CMDS"));

				commands->createChild<CommandListTextSettings>(LOC("TEXT"), std::vector<CommandName>{ CMDNAME("commandtext") }, &g_renderer.command_text);

				{
					auto colour_commands = commands->createChild<CommandList>(LOC("CLRCMDS"));
					colour_commands->createChild<CommandPreviewColourInSprite>();
					colour_commands->createChild<CommandPreviewColourInList>();
				}

				{
					auto prefill_value = commands->createChild<CommandList>(LOC("PRFILVAL"));
					prefill_value->createChild<CommandTogglePointer>(&CommandSlider::prefill_value, LOC("CMDTP_S"));
					prefill_value->createChild<CommandTogglePointer>(&CommandInput::prefill_value, LOC("CMDTP_AV"));
				}

				{
					auto number_sliders = commands->createChild<CommandList>(LOC("CMDTP_S_N"));
					number_sliders->createChild<CommandTogglePointer>(&CommandSlider::selectable_na, LOC("SELNA"), std::vector<CommandName>{ CMDNAME("selectablenasliders") });
					number_sliders->createChild<CommandLambdaToggle>(LOC("RHTBNDVAL"), std::vector<CommandName>{ CMDNAME("slidersrightbound") }, [](bool on, Click& click)
					{
						g_MenuGrid.updateSetting([on]
						{
							g_MenuGrid.number_sliders_rightbound_values = on;
						});
					}, true);
				}

				commands->createChild<CommandTogglePointer>(&CommandDivider::selectable, LOC("SELDIV"), CMDNAMES("selectabledividers"));
			}
			border = appearance->createChild<CommandTabBorder>();
			appearanceDirect = appearance->createChild<CommandTabAppearanceDirect>();
			fontText = appearance->createChild<CommandTabFontText>();
			presetFont = appearance->createChild<CommandTabPresetFont>();
			textures = appearance->createChild<CommandTabTextures>();
			appearance->createChild<CommandStreamproof>();
			entityPreviews = appearance->createChild<CommandTabEntityPreviews>();
		}

		{
			auto inputList = settings->createChild<CommandList>(LOC("IPT"));
			input = inputList->createChild<CommandTabInput>();
		}

		{
			auto infoText = settings->createChild<CommandList>(LOC("INFOTXT"));
			infoText->createChild<CommandInfoPos>();
			infoText->createChild<CommandInfoAlign>();
			infoText->createChild<CommandInfoScale>();
			infoText->createChild<CommandInfoDropShadow>();
		}

		{
			auto warnings = settings->createChild<CommandList>(LOC("WARNS"));
			{
				auto autoProceed = warnings->createChild<CommandList>(LOC("WARNAUTOPROC"));
				autoProceed->createChild<CommandAutoProceedWarningsForCommands>();
				autoProceed->createChild<CommandTogglePointer>(&Click::auto_proceed_warnings_for_hotkeys, LOC("HOTKEY"), CMDNAMES("hotkeysskipwarnings", "hotkeyskipswarnings", "hotkeyskipwarnings"));
			}
			warnings->createChild<CommandForceWarningRead>();
			warnings->createChild<CommandTogglePointer>(&g_gui.skip_accepted_warnings, LOC("WARNSKIPPROC"), CMDNAMES("skiprepeatwarnings"));
		}

		{
			auto notifs = settings->createChild<CommandList>(LOC("NOTFS"));
			{
				auto suppressGen = notifs->createChild<CommandList>(LOC("SUPPGEN"));
				suppressGen->createChild<CommandSuppressGenericResponsesCommandBox>();
				suppressGen->createChild<CommandSuppressGenericResponsesHotkey>();
			}
		}

		settings->createChild<CommandLang>();
		settings->createChild<CommandButtonInstructions>();
		settings->createChild<CommandAutoOpen>();
		settings->createChild<CommandDiscord>();
		settings->createChild<CommandTogglePointer>(&g_gui.sound, LOC("SFX"), CMDNAMES("sfx"));
		settings->createChild<CommandTogglePointer>(&g_gui.menu_navigation_reminders, LOC("MNUNVRMD"));

		{
			auto command_box = settings->createChild<CommandList>(LOC("B_CB"));
			command_box->createChild<CommandSliderPointer<CommandSlider, size_t>>(LOC("MAXSHWMCHCMD"), CMDNAMES("cbmaxshownmatching"), NOLABEL, &g_commandbox_grid.max_shown_matching_commands, 1, 1000);
			command_box->createChild<CommandTogglePointer>(&g_commandbox.keep_content_on_close, LOC("KPDRFCLS"), CMDNAMES("cbkeepdraft"));
		}

		this->createChild<CommandProfiles>();
		this->createChild<CommandScripts>();

		{
			auto experiments = this->createChild<CommandList>(LOC("TESTS"));
			experiments->createChild<CommandChatGadget>();
			experiments->createChild<CommandShowNavMesh>();
			experiments->createChild<CommandShowNavMeshNeighbors>();
			experiments->createChild<CommandShowPedTasks>();
			experiments->createChild<CommandApartments>();
			experiments->createChild<CommandShowInt>();
			experiments->createChild<CommandExportDisplayNames>();
			experiments->createChild<CommandGallery>();
		}

		{
			auto credits = this->createChild<CommandList>(LOC("CRED"), CMDNAMES("credits"));

			credits->createChild<CommandReadonlyLink>(LIT("Pocakking"), "https://github.com/Pocakking/BigBaseV2", LOC("CRED_Pocakking"));
			credits->createChild<CommandReadonlyName>(LIT("QuickNET"), LOC("CRED_QuickNET"));
			credits->createChild<CommandReadonlyName>(LIT("Maestro-1337"), LOC("CRED_UPD"));
			credits->createChild<CommandReadonlyLink>(LIT("alloc8or"), "https://alloc8or.re/gta5/nativedb/", LOC("CRED_alloc8or"));
			credits->createChild<CommandReadonlyLink>(LIT("SirMestre"), "https://www.unknowncheats.me/forum/grand-theft-auto-v/246757-sirmestre-littleinjector-v0-1-a.html", LOC("CRED_SirMestre"));
			credits->createChild<CommandReadonlyLink>(LIT("Give-Two"), "https://github.com/Give-Two/ScriptHookV", LOC("CRED_Give-Two"));
			credits->createChild<CommandReadonlyLink>(LIT("gir489"), "https://bitbucket.org/gir489/bigbasev2-fix", LOC("CRED_gir489"));
			credits->createChild<CommandReadonlyLink>(LIT("well-in-that-case"), "https://github.com/well-in-that-case/Pluto", LOC("CRED_Ryan"));
			credits->createChild<CommandReadonlyName>(LIT("C5Hackr"), LOC("CRED_BE"));
			credits->createChild<CommandReadonlyLink>(LIT("UPX"), "https://github.com/upx/upx", LOC("CRED_UPX"));
			credits->createChild<CommandReadonlyLink>(LIT("UnknownCheats"), "https://www.unknowncheats.me/forum/grand-theft-auto-v/", LOC("CRED_UnknownCheats"));
			credits->createChild<CommandReadonlyLink>(LIT("StackOverflow"), "https://stackoverflow.com/", LOC("CRED_StackOverflow"));

			credits->createChild<CommandDivider>(LOC("CRED_LANG"));

			credits->createChild<CommandReadonlyName>(LIT("Thimo"), LOC("CRED_Thimo"));
			credits->createChild<CommandReadonlyName>(LIT("moue"), LOC("CRED_moue"));
			credits->createChild<CommandReadonlyName>(LIT("zzzz"), LOC("CRED_zzzz"));
			credits->createChild<CommandReadonlyName>(LIT("IceDoomfist"), LOC("CRED_IceDoomfist"));
			credits->createChild<CommandReadonlyName>(LIT("99Anvar99"), LOC("CRED_99Anvar99"));
			credits->createChild<CommandReadonlyName>(LIT("Lancito01"), LOC("CRED_Lancito01"));
			credits->createChild<CommandReadonlyName>(LIT("Itreax"), LOC("CRED_Itreax"));
			credits->createChild<CommandReadonlyName>(LIT("Emre"), LOC("CRED_Emre"));
			credits->createChild<CommandReadonlyName>(LIT("DumbBird"), LOC("CRED_DumbBird"));
			credits->createChild<CommandReadonlyName>(LIT("Rem1ria"), LOC("CRED_JA"));
			credits->createChild<CommandReadonlyName>(LIT("Nhất Linh"), LOC("CRED_VI"));
			credits->createChild<CommandReadonlyName>(LIT("Il Mac"), LOC("CRED_IT"));

			credits->createChild<CommandDivider>(LOC("MISC"));

			credits->createChild<CommandReadonlyName>(LOC("CRED_YOU"), LOC("CRED_YOU_H"));
		}

		this->createChild<CommandSearchMenu>();
		this->createChild<CommandDefault>();
		this->createChild<CommandListNonDefaults>();
		this->createChild<CommandReadonlyLink>(LOC("HELP"), "https://stand.sh/help/");
		this->createChild<CommandListStarred>();
		this->createChild<CommandWeb>();
		this->createChild<CommandConsole>();
		this->createChild<CommandOpenStandFolder>();
		this->createChild<CommandClearNotifications>();
		this->createChild<CommandEmptyLog>();
		this->createChild<CommandFeatureList>();
		this->createChild<CommandListTutorial>();
		this->createChild<CommandUnload>();
	}

	void CommandTabStand::onKonamiCodeEntered()
	{
		if (secret == nullptr)
		{
			secret = this->createChild<CommandList>(LIT("Super Secret Shit"), CMDNAMES(), NOLABEL, CMDFLAGS_LIST | CMDFLAG_TEMPORARY | CMDFLAG_FEATURELIST_SKIP);

			secret->createChild<CommandListMillion>();
			secret->createChild<CommandRabbitHole>();
			secret->createChild<CommandSelfChatCommandsFriendsPerms>();
			secret->createChild<CommandNoExtraClear>();

			this->updateWebState();
		}
		Click click(CLICK_AUTO, TC_OTHER);
		secret->onClick(click);
	}
}

namespace Stand::Features
{
	Stand::CommandTabStand& GetCommandTabStand()
	{
		static Stand::CommandTabStand instance{ nullptr };
		return instance;
	}
}
