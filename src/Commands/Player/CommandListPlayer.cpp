#include "Commands/Player/CommandListPlayer.hpp"

#include "Commands/Widgets/CommandInputText.hpp"
#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Player/CommandPlayerSlider.hpp"
#include "Core/FlowEventReaction.hpp"
#include "Config/IncrementableStat.hpp"
#include "Config/Tunables.hpp"

#include "Commands/Player/CommandPlayerInfo.hpp"
#include "Commands/Player/CommandPlayerClassifier.hpp"

#include "Commands/Player/CommandPlayerTeleport.hpp"
#include "Commands/Vehicle/CommandPlayerTeleportIntoVeh.hpp"
#include "Commands/Player/CommandPlayerVisit.hpp"

#include "Commands/Player/CommandPlayerTpToMe.hpp"
#include "Commands/Player/CommandPlayerTpToPlayer.hpp"
#include "Commands/Player/CommandPlayerTpToMyWp.hpp"
#include "Commands/Player/CommandPlayerTpToMyObjective.hpp"
#include "Commands/Player/CommandPlayerTpToTheirWp.hpp"
#include "Commands/Player/CommandPlayerAptList.hpp"
#include "Commands/Player/CommandPlayerCasinoTp.hpp"

#include "Commands/Vehicle/CommandPlayerVehicleUpgrade.hpp"
#include "Commands/Vehicle/CommandPlayerVehRepair.hpp"
#include "Commands/Vehicle/CommandPlayerVehicleGodmode.hpp"
#include "Commands/Vehicle/CommandPlayerVehicleNoLockon.hpp"
#include "Commands/Vehicle/CommandPlayerVehEnginePowerMultiplier.hpp"
#include "Commands/Player/CommandPlayerGiveCollectibles.hpp"
#include "Commands/Player/CommandPlayerObtainFastRun.hpp"
#include "Commands/Player/CommandPlayerRelinquishFastRun.hpp"
//#include "CommandPlayerGift.hpp"
#include "Commands/Player/CommandPlayerSetRank.hpp"
#include "Commands/Player/CommandPlayerCeopay.hpp"
#include "Commands/Player/CommandPlayerRp.hpp"
#include "Commands/Player/CommandPlayerDropFigurines.hpp"
#include "Commands/Player/CommandPlayerDropCards.hpp"
#include "Commands/Player/CommandPlayerAutoheal.hpp"
#include "Commands/Player/CommandPlayerBail.hpp"
#include "Commands/Player/CommandPlayerOtr.hpp"
#include "Commands/Player/CommandPlayerSnack.hpp"
#include "Commands/Player/CommandPlayerExplosiveHits.hpp"
#include "Commands/Player/CommandPlayerImpactParticles.hpp"
#include "Commands/Player/CommandPlayerGiveSh.hpp"

#include "Commands/Player/CommandPlayerChatMock.hpp"
#include "Commands/Player/CommandPlayerChatOwoify.hpp"
#include "Commands/Player/CommandPlayerSendPM.hpp"

#include "Commands/Player/CommandPlayerArm.hpp"
#include "Commands/Player/CommandPlayerAmmo.hpp"
#include "Commands/Player/CommandPlayerParachute.hpp"
#include "Commands/Player/CommandPlayerDisarm.hpp"

#include "Commands/Player/CommandPlayerAttackers.hpp"

#include "Commands/Vehicle/CommandPlayerVehKill.hpp"
#include "Commands/Vehicle/CommandPlayerVehFlip.hpp"
#include "Commands/Vehicle/CommandPlayerVehSpin.hpp"
#include "Commands/Vehicle/CommandPlayerVehEMP.hpp"
#include "Commands/Vehicle/CommandPlayerVehDelete.hpp"
#include "Commands/Vehicle/CommandPlayerVehDestroyProp.hpp"
#include "Commands/Vehicle/CommandPlayerVehBreakTailBoom.hpp"
#include "Commands/Vehicle/CommandPlayerVehPopTyres.hpp"
#include "Commands/Vehicle/CommandPlayerVehSlingshot.hpp"
#include "Commands/Vehicle/CommandPlayerVehLockDoors.hpp"
#include "Commands/Vehicle/CommandPlayerVehDowngrade.hpp"
#include "Commands/Vehicle/CommandPlayerVehTurnAround.hpp"
#include "Commands/Vehicle/CommandPlayerVehSlipperyTyres.hpp"
#include "Commands/Vehicle/CommandPlayerVehRemoveDoors.hpp"
#include "Commands/Vehicle/CommandPlayerVehIgnite.hpp"
#include "Commands/Vehicle/CommandPlayerVehDetachWing.hpp"
#include "Commands/Vehicle/CommandPlayerVehDetachWheel.hpp"
#include "Commands/Vehicle/CommandPlayerVehKick.hpp"
#include "Commands/Player/CommandPlayerInteriorKick.hpp"
#include "Commands/Player/CommandPlayerRam.hpp"
#include "Commands/Player/CommandPlayerRamSpeed.hpp"
#include "Commands/Player/CommandPlayerSetWantedLevel.hpp"
#include "Commands/Player/CommandPlayerFreeze.hpp"
#include "Commands/Player/CommandPlayerFakeMoneyDrop.hpp"
#include "Commands/Player/CommandPlayerBlockPassiveMode.hpp"
#include "Commands/Player/CommandPlayerForceCamFront.hpp"
//#include "CommandPlayerTransactionError.hpp"
#include "Commands/Player/CommandPlayerRagdoll.hpp"
#include "Commands/Player/CommandPlayerSoundSpam.hpp"
#include "Commands/Player/CommandPlayerShakeCam.hpp"
#include "Commands/Ped/CommandGravitateNPCs.hpp"
#include "Commands/Ped/CommandAggressiveNPCs.hpp"
#include "Commands/Player/CommandPlayerMuggerLoop.hpp"
#include "Commands/Player/CommandPlayerBeast.hpp"
#include "Commands/Player/CommandPlayerKill.hpp"
#include "Commands/Player/CommandPlayerCage.hpp"
#include "Commands/Player/CommandPlayerSendtojob.hpp"
#include "Commands/Vehicle/CommandPlayerDisableDrivingVehicles.hpp"
#include "Commands/Player/CommandPlayerCeoKick.hpp"
#include "Commands/Player/CommandPlayerInfiniteLoading.hpp"
#include "Commands/Player/CommandPlayerInfiniteRing.hpp"
#include "Commands/Player/CommandPlayerForceFmMission.hpp"
#include "Commands/Player/CommandPlayerTriggerDefend.hpp"

#include "Commands/Player/CommandPlayerExplode.hpp"
//#include "CommandPlayerBlame.hpp"
#include "Commands/Player/CommandPlayerBounty.hpp"
#include "Commands/Player/CommandPlayerLoopBounty.hpp"

#include "Commands/Stand/CommandListSelectParticle.hpp"
#include "Commands/Player/CommandPlayerParticleSpam.hpp"

#include "Commands/Player/CommandPlayerNotifyRemoved.hpp"
#include "Commands/Player/CommandPlayerNotifyStolen.hpp"
#include "Commands/Player/CommandPlayerNotifyBanked.hpp"
#include "Commands/Player/CommandPlayerNotifySpam.hpp"

// Kinda useless since it's only usable on friends and members of the same non-Rockstar crew.
#define SMS_OPTIONS false

#if SMS_OPTIONS
#include "Commands/Player/CommandPlayerSmsText.hpp"
#include "Commands/Player/CommandPlayerSmsSend.hpp"
#include "Commands/Player/CommandPlayerSmsSpam.hpp"
#endif

#include "Commands/Player/CommandPlayerWaypoint.hpp"
#include "Commands/Player/CommandPlayerCopyWaypoint.hpp"

#include "Commands/Player/CommandPlayerSpectateNinja.hpp"
#include "Commands/Player/CommandPlayerSpectateLegit.hpp"

#include "Commands/Player/CommandPlayerShowcam.hpp"
#include "Commands/Player/CommandPlayerProfile.hpp"
#include "Commands/Player/CommandPlayerBefriend.hpp"
#include "Commands/Player/CommandPlayerInhistory.hpp"
#include "Commands/Player/CommandPlayerCopyOutfit.hpp"
#include "Commands/Vehicle/CommandPlayerCopyVehicle.hpp"
#include "Commands/Player/CommandPlayerAs.hpp"
#include "Commands/Player/CommandPlayerAsAll.hpp"
#include "Commands/Player/CommandPlayerCeoJoin.hpp"
#include "Commands/Player/CommandPlayerIncrementStatNeutral.hpp"
#include "Commands/Player/CommandPlayerIncrementStatAggressive.hpp"

#include "Commands/Player/CommandPlayerKickSmart.hpp"
#include "Commands/Player/CommandPlayerKickBreakup.hpp"
#if HAVE_BREAKUP_KICK
#include "Commands/Player/CommandPlayerKickConfusion.hpp"
#endif
#include "Commands/Player/CommandPlayerKickPoolsClosed.hpp"
#include "Commands/Player/CommandPlayerKickNonHost.hpp"
#include "Commands/Player/CommandPlayerKickOrgasm.hpp"
#include "Commands/Player/CommandPlayerKickLoveLetter.hpp"
#include "Commands/Player/CommandPlayerKickBlacklist.hpp"
#include "Commands/Player/CommandPlayerKickHost.hpp"
#include "Commands/Player/CommandPlayerKickHostType.hpp"
#include "Commands/Player/CommandPlayerKickPickup.hpp"

#include "Commands/Player/CommandPlayerCrashElegant.hpp"
#if defined(STAND_DEBUG) && HAVE_SEND_CLONE_SYNC_HOOK
#include "Commands/Player/CommandPlayerCrashTask.hpp"
#endif
#if HAVE_SEND_CLONE_CREATE_HOOK
#include "Commands/Player/CommandPlayerCrashFootlettuce.hpp"
#endif
#include "Commands/Vehicle/CommandPlayerCrashVehicle.hpp"
#include "Commands/Player/CommandPlayerCrashTrain.hpp"

#include "Commands/Player/CommandPlayerTimeout.hpp"
#include "Commands/Player/CommandPlayerFlowEventReaction.hpp"

namespace Stand
{
	CommandListPlayer::CommandListPlayer(CommandList* const parent, const PlayerProvider* pp, Label&& menu_name, std::vector<CommandName>&& command_names, commandflags_t flags)
		: CommandListConcealer(parent, std::move(menu_name), std::move(command_names), NOLABEL, flags, COMMAND_LIST_PLAYER), pp(pp)
	{
	}

	void CommandListPlayer::populate(compactplayer_t p)
	{
#if FREE_PROTECTIONS
		if (!g_gui.isRootStateFull())
		{
			this->createChild<CommandPlayerTimeout>();
			return;
		}
#endif

		const commandflags_t sublist_flags = getSublistFlags();

		// Exclusions
		if (!this->pp->single)
		{
			auto excludes = this->createChild<CommandList>(LOC("XCLDS"));

			PlayerProvider::excludes.populateList(excludes, {}, PlayerExcludes::SHOW_MODDER | PlayerExcludes::SHOW_ORG_MEMBERS | PlayerExcludes::SHOW_LIKELY_MODDER);
		}

		if (this->pp->single)
		{
			this->createChild<CommandPlayerInfo>();
			this->createChild<CommandPlayerClassifier>();
		}

		// Teleport
		{
			auto teleport = this->createChild<CommandList>(LOC("TP"), {}, NOLABEL, sublist_flags);

			if (this->pp->single)
			{
				teleport->createChild<CommandDivider>(LOC("PLYTP"));
				teleport->createChild<CommandPlayerTeleport>();
				teleport->createChild<CommandPlayerTeleportIntoVeh>();
				teleport->createChild<CommandPlayerVisit>();
			}

			teleport->createChild<CommandDivider>(LOC("PLYSMMN_D"));
			teleport->createChild<CommandPlayerTpToMe>();
			if (this->pp->single)
			{
				teleport->createChild<CommandPlayerTpToPlayer>();
			}
			teleport->createChild<CommandPlayerTpToMyWp>();
			teleport->createChild<CommandPlayerTpToMyObjective>();
			teleport->createChild<CommandPlayerTpToTheirWp>();
			teleport->createChild<CommandPlayerAptList>(sublist_flags);
			teleport->createChild<CommandPlayerCasinoTp>();
		}

		// Friendly
		{
			auto friendly = this->createChild<CommandList>(LOC("FRNDLY"), {}, NOLABEL, sublist_flags);

			// Vehicle
			{
				auto frienveh = friendly->createChild<CommandList>(LOC("VEH"));

				frienveh->createChild<CommandPlayerVehicleUpgrade>();
				frienveh->createChild<CommandPlayerVehRepair>();
				frienveh->createChild<CommandPlayerVehicleGodmode>();
				frienveh->createChild<CommandPlayerVehicleNoLockon>();
				frienveh->createChild<CommandPlayerVehEnginePowerMultiplier>();
			}

			// Give Collectibles
			{
				auto gc = friendly->createChild<CommandList>(LOC("GIVECOLL"));

				gc->createChild<CommandPlayerGiveCollectibles>(-1, LOC("ALL"));
				gc->createChild<CommandPlayerGiveCollectibles>(0, LOC("GIVECOLL_0"));
				gc->createChild<CommandPlayerGiveCollectibles>(1, LOC("GIVECOLL_1"));
				gc->createChild<CommandPlayerGiveCollectibles>(2, LOC("GIVECOLL_2"));
				gc->createChild<CommandPlayerGiveCollectibles>(3, LOC("GIVECOLL_3"));
				gc->createChild<CommandPlayerGiveCollectibles>(4, LOC("GIVECOLL_4"));
				gc->createChild<CommandPlayerGiveCollectibles>(5, LOC("GIVECOLL_5"));
				gc->createChild<CommandPlayerGiveCollectibles>(6, LOC("GIVECOLL_6"));
				gc->createChild<CommandPlayerGiveCollectibles>(8, LOC("GIVECOLL_8"));
				gc->createChild<CommandPlayerGiveCollectibles>(9, LOC("GIVECOLL_9"));
				gc->createChild<CommandPlayerGiveCollectibles>(10, LOC("GIVECOLL_10"));
				gc->createChild<CommandPlayerGiveCollectibles>(16, LOC("GIVECOLL_16"));
				gc->createChild<CommandPlayerGiveCollectibles>(17, LOC("GIVECOLL_17"));
				gc->createChild<CommandPlayerGiveCollectibles>(19, LOC("GIVECOLL_19"));
				gc->createChild<CommandPlayerGiveCollectibles>(21, LOC("GIVECOLL_21"));
			}

#if REMOTE_STAT_WRITING
			// Fast Run Ability
			{
				auto fast_run = friendly->createChild<CommandList>(LOC("FSTRUN"));
				fast_run->createChild<CommandPlayerObtainFastRun>();
				fast_run->createChild<CommandPlayerRelinquishFastRun>();
			}
#endif

			if (this->pp->single)
			{
				//friendly->createChild<CommandPlayerGift>();
			}
#if REMOTE_STAT_WRITING
			friendly->createChild<CommandPlayerSetRank>();
#endif
			friendly->createChild<CommandPlayerCeopay>();
			friendly->createChild<CommandPlayerRp>();
			friendly->createChild<CommandPlayerDropFigurines>();
			friendly->createChild<CommandPlayerDropCards>();
			friendly->createChild<CommandPlayerAutoheal>();
			friendly->createChild<CommandPlayerBail>();
			friendly->createChild<CommandPlayerOtr>();
			friendly->createChild<CommandPlayerSnack>();
			friendly->createChild<CommandPlayerExplosiveHits>();
			friendly->createChild<CommandPlayerImpactParticles>();
#if CAN_GIVE_SH
			if (this->pp->single)
			{
				friendly->createChild<CommandPlayerGiveSh>();
			}
#endif
		}

		// Chat
		{
			auto chat = this->createChild<CommandList>(LOC("CHT"), {}, NOLABEL, sublist_flags);

			chat->createChild<CommandPlayerChatMock>();
			chat->createChild<CommandPlayerChatOwoify>();
			if (this->pp->single)
			{
				chat->createChild<CommandPlayerSendPM>();
			}
		}

		// Weapons
		{
			auto weapons = this->createChild<CommandList>(LOC("WPN"), {}, NOLABEL, sublist_flags);

			weapons->createChild<CommandPlayerArm>();
			weapons->createChild<CommandPlayerAmmo>();
			weapons->createChild<CommandPlayerParachute>();
			weapons->createChild<CommandPlayerDisarm>();
		}

		// Trolling
		{
			auto trolling = this->createChild<CommandList>(LOC("TROLL"), {}, NOLABEL, sublist_flags);

			// Vehicle
			{
				auto vehicle = trolling->createChild<CommandList>(LOC("VEH"), {}, NOLABEL, sublist_flags);

				if (this->pp->single)
				{
					vehicle->createChild<CommandPlayerVehKill>();
					vehicle->createChild<CommandPlayerVehFlip>();
					vehicle->createChild<CommandPlayerVehSpin>();
					vehicle->createChild<CommandPlayerVehEMP>();
					vehicle->createChild<CommandPlayerVehIgnite>();
					vehicle->createChild<CommandPlayerVehDelete>();
					vehicle->createChild<CommandPlayerVehPopTyres>();
					vehicle->createChild<CommandPlayerVehSlingshot>();
					vehicle->createChild<CommandPlayerVehLockDoors>();
					vehicle->createChild<CommandPlayerVehDowngrade>();
					vehicle->createChild<CommandPlayerVehTurnAround>();
					vehicle->createChild<CommandPlayerVehDetachWing>();
					vehicle->createChild<CommandPlayerVehDetachWheel>();
				}

				vehicle->createChild<CommandPlayerVehSlipperyTyres>();

				if (this->pp->single)
				{
					vehicle->createChild<CommandPlayerVehRemoveDoors>();
					vehicle->createChild<CommandPlayerVehDestroyProp>();
					vehicle->createChild<CommandPlayerVehBreakTailBoom>();
				}
			}

			if (this->pp->single)
			{
				trolling->createChild<CommandPlayerAttackers>(sublist_flags);

				// Ram
				{
					auto ram = trolling->createChild<CommandList>(LOC("RAM"));
					auto action = ram->createChild<CommandPlayerRam>();
					action->speed = ram->createChild<CommandPlayerRamSpeed>();
					action->model = ram->createChild<CommandListSelectVehicleModel>(LOC("VEH"), {}, NOLABEL, CommandListSelectVehicleModel::RANDOM, true);
					ram->createChild<CommandTogglePointer>(&action->invisible, LOC("RAM_INVIS"));
				}
			}

			trolling->createChild<CommandPlayerSetWantedLevel>();
			trolling->createChild<CommandPlayerFreeze>();
			if (this->pp->single)
			{
				trolling->createChild<CommandPlayerFakeMoneyDrop>();
				trolling->createChild<CommandPlayerBlockPassiveMode>();
			}
			trolling->createChild<CommandPlayerForceCamFront>();
			//trolling->createChild<CommandPlayerTransactionError>();
			trolling->createChild<CommandPlayerRagdoll>();
			if (this->pp->single)
			{
				trolling->createChild<CommandPlayerSoundSpam>();
			}
			trolling->createChild<CommandPlayerShakeCam>();
			if (this->pp->single)
			{
				trolling->createChild<CommandGravitateNPCs>();
				trolling->createChild<CommandAggressiveNPCs>();
				trolling->createChild<CommandPlayerMuggerLoop>();
				trolling->createChild<CommandPlayerBeast>();
			}
			trolling->createChild<CommandPlayerKill>();
			trolling->createChild<CommandPlayerCage>();
			trolling->createChild<CommandPlayerExplode>();

			if (this->pp->single)
			{
				//trolling->createChild<CommandPlayerBlame>();

				// Particle Spam
				{
					auto particle = trolling->createChild<CommandList>(LOC("PLRPTFXSPM"));
					auto particles = particle->makeChild<CommandListSelectParticle>(LOC("PARTICLE"));
					auto scale = particle->makeChild<CommandPlayerParticleSpamScale>();
					auto delay = particle->makeChild<CommandPlayerParticleSpamDelay>();
					auto hide = particle->makeChild<CommandToggle>(LOC("HDELCL"), {}, LOC("HDELCL_H"), true);
					particle->createChild<CommandPlayerParticleSpam>(particles.get(), delay.get(), scale.get(), hide.get());
					particle->children.emplace_back(std::move(particles));
					particle->children.emplace_back(std::move(scale));
					particle->children.emplace_back(std::move(delay));
					particle->children.emplace_back(std::move(hide));
				}

				trolling->createChild<CommandPlayerBounty>(nullptr);
			}
			else
			{
				auto bounty_self = trolling->makeChild<CommandToggle>(LOC("BNTYSLF"), {}, LOC("BNTYSLF_H"));
				trolling->createChild<CommandPlayerBounty>(bounty_self.get());
				trolling->children.emplace_back(std::move(bounty_self));
			}

			// Loop Bounty
			{
				auto loop = trolling->createChild<CommandList>(LOC("LOOPBNTY"));
				auto val = loop->createChild<CommandPlayerSlider>(LOC("C_V"), CMDNAMES_OBF("loopbountyvalue"), NOLABEL, 1000, 10000, 10000, 1000);
				auto cmd = loop->createChild<CommandPlayerLoopBounty>(val);
				if (!this->pp->single)
				{
					cmd->self = loop->createChild<CommandToggle>(LOC("BNTYSLF"), {}, LOC("BNTYSLF_H"));
				}
			}

			// Send Notifications
			{
				auto notify = trolling->createChild<CommandList>(LOC("PLY_NOTIFY"), {}, NOLABEL, sublist_flags);

				notify->createChild<CommandPlayerNotifyRemoved>();
				notify->createChild<CommandPlayerNotifyStolen>();
				notify->createChild<CommandPlayerNotifyBanked>();
				notify->createChild<CommandPlayerNotifySpam>();
			}

			// Send To Job
			{
				auto sendtojob = trolling->createChild<CommandList>(LOC("SND2JB"), {}, NOLABEL, sublist_flags);

				sendtojob->createChild<CommandPlayerSendtojob>(0, LOC("SND2JB_0"), { CMDNAME("sendtojob") });
				sendtojob->createChild<CommandPlayerSendtojob>(1, LOC("SND2JB_1"));
				sendtojob->createChild<CommandPlayerSendtojob>(2, LOC("SND2JB_2"));
				sendtojob->createChild<CommandPlayerSendtojob>(3, LOC("SND2JB_3"));
				sendtojob->createChild<CommandPlayerSendtojob>(4, LOC("SND2JB_4"));
				sendtojob->createChild<CommandPlayerSendtojob>(5, LOC("SND2JB_5"));
				sendtojob->createChild<CommandPlayerSendtojob>(6, LOC("SND2JB_6"));
				sendtojob->createChild<CommandPlayerSendtojob>(7, LOC("SND2JB_7"));
			}

			trolling->createChild<CommandPlayerVehKick>();
			trolling->createChild<CommandPlayerInteriorKick>();
			trolling->createChild<CommandPlayerDisableDrivingVehicles>();
			if (this->pp->single)
			{
				trolling->createChild<CommandPlayerCeoKick>();
			}
			trolling->createChild<CommandPlayerInfiniteLoading>();
			trolling->createChild<CommandPlayerInfiniteRing>();
			trolling->createChild<CommandPlayerForceFmMission>();
			trolling->createChild<CommandPlayerTriggerDefend>();
		}

#if SMS_OPTIONS
		// Send Text Message
		{
			auto sms = this->createChild<CommandList>(LOC("SMS"), {}, NOLABEL, sublist_flags);

			auto text = sms->createChild<CommandPlayerSmsText>();

			auto sender = sms->createChild<CommandPlayerSmsSend>(text);
			sms->createChild<CommandPlayerSmsSpam>(sender);
		}
#endif

		if (this->pp->single)
		{
			this->createChild<CommandPlayerWaypoint>();
			this->createChild<CommandPlayerCopyWaypoint>();

			// Spectate
			{
				auto spectate = this->createChild<CommandList>(LOC("PLYSPEC"));
		
				spectate->createChild<CommandPlayerSpectateNinja>();
				spectate->createChild<CommandPlayerSpectateLegit>();
			}
		}

		this->createChild<CommandPlayerShowcam>();

		if (this->pp->single)
		{
			this->createChild<CommandPlayerProfile>();
			this->createChild<CommandPlayerBefriend>();
			this->createChild<CommandPlayerInhistory>();
			this->createChild<CommandPlayerCopyOutfit>();
			this->createChild<CommandPlayerCopyVehicle>();
			this->createChild<CommandPlayerAs>();
			this->createChild<CommandPlayerCeoJoin>();
			((CommandPlayer*)this)->pong_command = createChild<CommandPlayerPong>();
		}
		else
		{
			this->createChild<CommandPlayerAsAll>();
		}

		// Increment Commend/Report Stats
		{
			auto ise = this->createChild<CommandList>(LOC("PLYISE"), {}, NOLABEL, sublist_flags);

			ise->createChild<CommandDivider>(LOC("CMEND"));
			ise->createChild<CommandPlayerIncrementStatNeutral>(LOC("ISE_H"), CMDNAMES_OBF("commendhelpful"), STAT_HELPFUL);
			ise->createChild<CommandPlayerIncrementStatNeutral>(LOC("ISE_F"), CMDNAMES_OBF("commendfriendly"), STAT_FRIENDLY);
			ise->createChild<CommandDivider>(LOC("RPRPT"));
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_G2"), CMDNAMES_OBF("reportgriefing"), STAT_GRIEFING);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_X2"), CMDNAMES_OBF("reportexploits"), STAT_EXPLOITS);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_B2"), CMDNAMES_OBF("reportbugabuse"), STAT_GAME_EXPLOITS);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_TC_A"), CMDNAMES_OBF("reportannoying"), STAT_TC_ANNOYINGME);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_TC_H"), CMDNAMES_OBF("reporthate"), STAT_TC_HATE);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_VC_A"), CMDNAMES_OBF("reportvcannoying"), STAT_VC_ANNOYINGME);
			ise->createChild<CommandPlayerIncrementStatAggressive>(LOC("ISE_VC_H"), CMDNAMES_OBF("reportvchate"), STAT_VC_HATE);
		}

		{
			auto kick = this->createChild<CommandList>(LOC("PLYKCK"), {}, NOLABEL, sublist_flags);

			kick->createChild<CommandPlayerKickSmart>();
#if HAVE_BREAKUP_KICK
			kick->createChild<CommandPlayerKickBreakup>(LOC("PLYKCK_B"), CMDNAMES_OBF("breakup"), LOC("PLYKCK_H_DSCRT"), -1);
#endif
#if ENABLE_BAN_KICK
	#if HAVE_BREAKUP_KICK
			kick->createChild<CommandPlayerKickBreakup>(LOC("PLYBAN"), CMDNAMES_OBF("ban"), LOC("PLYKCK_H_DSCRT"), /* BAIL_CONSOLE_BAN */ 19);
	#else
			kick->createChild<CommandPlayerKickBreakup>(LOC("PLYBAN"), CMDNAMES_OBF("ban"), LOC("PLYKCK_H_HO"), /* BAIL_CONSOLE_BAN */ 19);
	#endif
#endif
#if HAVE_BREAKUP_KICK
			if (this->pp->single)
			{
				kick->createChild<CommandPlayerKickConfusion>();
			}
#endif
			kick->createChild<CommandPlayerKickPoolsClosed>();
			if (this->pp->single)
			{
				kick->createChild<CommandPlayerKickLoveLetter>();
				kick->createChild<CommandPlayerKickOrgasm>();
			}
			kick->createChild<CommandPlayerKickBlacklist>();
			kick->createChild<CommandPlayerKickHost>();
			if (g_tunables.getBool(ATSTRINGHASH("player_commands.bekick")))
			{
				kick->createChild<CommandPlayerKickHostType>(LOC("BE_K"), CMDNAMES("bekick"), LOC("PLYKCK_H_HO"), AbstractPlayer::BATTLEYE_KICK);
			}
			if (g_tunables.getBool(ATSTRINGHASH("player_commands.beban")))
			{
				kick->createChild<CommandPlayerKickHostType>(LOC("BE_B"), CMDNAMES("beban"), Label::combineWithSpace(LOC("BE_B_H"), LOC("PLYKCK_H_HO")), AbstractPlayer::BATTLEYE_BAN);
			}
			kick->createChild<CommandPlayerKickNonHost>();
			if (this->pp->single)
			{
				kick->createChild<CommandPlayerKickPickup>();
			}
		}

		{
			auto crash = this->createChild<CommandList>(LOC("PLYCRSH"), {}, NOLABEL, sublist_flags);

			crash->createChild<CommandPlayerCrashElegant>();
			if (this->pp->single)
			{
#if defined(STAND_DEBUG) && HAVE_SEND_CLONE_SYNC_HOOK
				crash->createChild<CommandPlayerCrashTask>();
#endif
#if HAVE_SEND_CLONE_CREATE_HOOK
				crash->createChild<CommandPlayerCrashFootlettuce>();
#endif
				crash->createChild<CommandPlayerCrashVehicle>();
#if HAVE_SEND_CLONE_CREATE_HOOK
				crash->createChild<CommandPlayerCrashTrain>();
#endif
			}
#if !HAVE_SEND_CLONE_CREATE_HOOK
			else
			{
				crash->createChild<CommandPlayerCrashTrain>();
			}
#endif
		}

		// Network Traffic
		if (this->pp->single)
		{
			this->createChild<CommandPlayerTimeout>();

			{
				auto netreactions = this->createChild<CommandList>(LOC("PTX_NET"), {}, NOLABEL, sublist_flags);

				netreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_net_event_reactions[p], REACTION_BLOCK, LOC("PTX_NET_R_B"), CMDNAMES_OBF("ignore"));
				netreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_net_event_reactions[p], REACTION_LOG_FILE, LOC("RCT_L_F"), CMDNAMES_OBF("netlog"));
				netreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_net_event_reactions[p], REACTION_LOG_CONSOLE, LOC("RCT_L_C"));
				netreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_net_event_reactions[p], REACTION_TOAST, LOC("RCT_T"));
			}

			{
				auto syncinreactions = this->createChild<CommandList>(LOC("PTX_SI"), {}, NOLABEL, sublist_flags);

				syncinreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_in_reactions[p], REACTION_BLOCK, LOC("PTX_NET_R_B"), CMDNAMES_OBF("blocksync"));
				syncinreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_in_reactions[p], REACTION_LOG_FILE, LOC("RCT_L_F"), CMDNAMES_OBF("syncinlog"));
				syncinreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_in_reactions[p], REACTION_LOG_CONSOLE, LOC("RCT_L_C"));
				syncinreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_in_reactions[p], REACTION_TOAST, LOC("RCT_T"));
			}
		}

		{
			auto syncoutreactions = this->createChild<CommandList>(LOC("PTX_SO"), {}, NOLABEL, sublist_flags);

			syncoutreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_out_reactions[p], REACTION_BLOCK, LOC("PTX_NET_R_B"), CMDNAMES_OBF("desync"), this->pp->single ? LOC("PLY_BOS_H_1") : LOC("PLY_BOS_H_A"));
			syncoutreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_out_reactions[p], REACTION_LOG_FILE, LOC("RCT_L_F"), CMDNAMES_OBF("syncoutlog"));
			syncoutreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_out_reactions[p], REACTION_LOG_CONSOLE, LOC("RCT_L_C"));
			syncoutreactions->createChild<CommandPlayerFlowEventReaction>(&g_hooking.player_sync_out_reactions[p], REACTION_TOAST, LOC("RCT_T"));
		}
	}

	commandflags_t CommandListPlayer::getSublistFlags() const
	{
		if (this->pp->single)
		{
			return CMDFLAGS_LIST | CMDFLAG_TEMPORARY;
		}
		return CMDFLAGS_LIST;
	}

	void CommandListPlayer::registerCommand(CommandPhysical* const cmd) const
	{
		if (isSingle())
		{
			cmd->flags |= CMDFLAG_TEMPORARY;
		}
		registerCommandNames(cmd);
	}

	void CommandListPlayer::registerCommandNames(CommandPhysical* const cmd) const
	{
		const CommandName suffix = getCommandNamesSuffix();
		for (auto& command_name : cmd->command_names)
		{
			CommandPhysical::collapse_command_names.emplace(cmdNameToUtf16(command_name));
			command_name.append(suffix);
		}
		if (cmd->isListAction())
		{
			cmd->as<CommandListAction>()->updateCommandNamesOfChildren();
		}
	}

	bool CommandListPlayer::isSingle() const noexcept
	{
		return pp->single;
	}

	void CommandListPlayer::ensureOnlineAndWarnSelfharm(Command* command_for_warning, Click& click, std::function<void(ThreadContext)>&& callback) const
	{
		if(click.inOnline())
		{
			if (!pp->isUser())
			{
				callback(click.thread_context);
				return;
			}
			command_for_warning->warnSelfHarm(click, [command_for_warning, callback{std::move(callback)}](ThreadContext thread_context)
			{
				if (command_for_warning->isT<CommandToggleNoCorrelation>())
				{
					command_for_warning->as<CommandToggleNoCorrelation>()->setOnIndication(true);
				}
				callback(thread_context);
			});
		}
		if (command_for_warning->isT<CommandToggleNoCorrelation>())
		{
			command_for_warning->as<CommandToggleNoCorrelation>()->setOnIndication(false);
		}
	}
}
