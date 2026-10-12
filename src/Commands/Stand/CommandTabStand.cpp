#include "Commands/Stand/CommandTabStand.hpp"

#include "Commands/Stand/Appearance/CommandStreamproof.hpp"
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
#include "Commands/Stand/CommandInfoAlign.hpp"
#include "Commands/Stand/CommandInfoDropShadow.hpp"
#include "Commands/Stand/CommandInfoPos.hpp"
#include "Commands/Stand/CommandInfoScale.hpp"
#include "Commands/Stand/CommandLang.hpp"
#include "Commands/Stand/CommandListMillion.hpp"
#include "Commands/Stand/CommandListNonDefaults.hpp"
#include "Commands/Stand/CommandListStarred.hpp"
#include "Commands/Stand/CommandListTutorial.hpp"
#include "Commands/Stand/CommandNoExtraClear.hpp"
#include "Commands/Stand/CommandOpenStandFolder.hpp"
#include "Commands/Stand/CommandProfiles.hpp"
#include "Commands/Stand/CommandRabbitHole.hpp"
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
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSearchMenu.hpp"
#include "Commands/Widgets/CommandReadonlyLink.hpp"
#include "Commands/Widgets/CommandReadonlyName.hpp"
#include "Menu/Click.hpp"
#include "Core/ThreadContext.hpp"

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

			colours = appearance->createChild<CommandTabColours>();
			position = appearance->createChild<CommandTabPosition>();
			header = appearance->createChild<CommandTabHeader>();
			addressBar = appearance->createChild<CommandTabAddressBar>();
			cursor = appearance->createChild<CommandTabCursor>();
			tabs = appearance->createChild<CommandTabTabs>();
			scrollbar = appearance->createChild<CommandTabScrollbar>();
			commandInfoText = appearance->createChild<CommandTabCommandInfoText>();
			notifications = appearance->createChild<CommandTabNotifications>();
			commands = appearance->createChild<CommandTabCommands>();
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
			}
			warnings->createChild<CommandForceWarningRead>();
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
