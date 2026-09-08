#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "World/Stats.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	namespace KortzCenterHeist
	{
		static std::vector<std::pair<int, const char*>> kortzCenterTargets = {
		    {0, "La Dernière Débauche"},
		    {1, "Hare Oneself Think"},
		    {2, "The Downfall Rome"},
		    {3, "Brother Brother"},
		    {4, "A Cast Characters"},
		    {5, "Gone To Seed"},
		    {6, "True Love"},
		    {7, "Breathless"},
		    {8, "Consumato"},
		    {9, "I Hear Voices"},
		    {10, "Winter, Nowhere in Particular"},
		    {11, "The Girl With Pearl Necklace"},
		    {12, "Chat on Fruit"},
		    {13, "Pumpkin"},
		    {14, "Twindifference"},
		    {15, "Stacks Study V"},
		    {16, "I, Fruit"},
		    {17, "To Beat About Bush"},
		    {18, "In Excess of Success"},
		    {19, "Juiced"},
		    {20, "A Winding Road Home"},
		    {21, "Teckels"},
		    {22, "Trust"},
		    {23, "Until Death"},
		    {24, "What Melons?"},
		    {25, "The Outcome Endeavour"},
		    {26, "Mi O Melee"}
		};

		static CommandListSelect _KortzCenterPrimaryTarget{"kortzcenterheistprimarytarget", "Primary Target", "Primary target", kortzCenterTargets, 0};

		static CommandToggleLegacy _KortzCenterGuardRoutes{"kortzcenterheistguardroutes", "Guard Routes", "Guard routes purchased", true};
		static CommandToggleLegacy _KortzCenterGlassCutter{"kortzcenterheistglasscutter", "Glass Cutter", "Glass cutter purchased", true};
		static CommandToggleLegacy _KortzCenterPowerDrills{"kortzcenterheistpowerdrills", "Power Drills", "Power drills purchased", true};
		static CommandToggleLegacy _KortzCenterEMPCharges{"kortzcenterheistempcharges", "EMP Charges", "EMP charges purchased", true};
		static CommandToggleLegacy _KortzCenterScopeOut{"kortzcenterheistscopeout", "Scope Out", "Scope out Kortz Center", true};
		static CommandToggleLegacy _KortzCenterAlphaMail{"kortzcenterheistalphamail", "Alpha Mail Disguise", "Alpha mail disguise", true};
		static CommandToggleLegacy _KortzCenterHazmat{"kortzcenterheisthazmat", "Hazmat Suit", "Hazmat suit", true};
		static CommandToggleLegacy _KortzCenterStaffKeycard{"kortzcenterheiststaffkeycard", "Staff Key Card", "Staff key card", true};
		static CommandToggleLegacy _KortzCenterTacticalEquip{"kortzcenterheisttacticalequip", "Tactical Equipment", "Tactical equipment", true};
		static CommandToggleLegacy _KortzCenterHackingDevice{"kortzcenterheisthackingdevice", "Hacking Device", "Hacking device", true};
		static CommandToggleLegacy _KortzCenterAccessCode{"kortzcenterheistaccesscode", "Access Code", "Access code", true};
		static CommandToggleLegacy _KortzCenterUnmarkedWeapons{"kortzcenterheistunmarkedweapons", "Unmarked Weapons", "Unmarked weapons", true};
		static CommandToggleLegacy _KortzCenterCaracara{"kortzcenterheistcaracara", "Armored Caracara", "Armored Caracara", true};
		static CommandToggleLegacy _KortzCenterAnnihilator{"kortzcenterheistannihilator", "Annihilator Stealth", "Annihilator stealth", true};
		static CommandToggleLegacy _KortzCenterManchez{"kortzcenterheistmanchez", "Manchez", "Manchez", true};
		static CommandToggleLegacy _KortzCenterPrepEMP{"kortzcenterheistprepemp", "EMP Charges (Prep)", "EMP charges prep", true};
		static CommandToggleLegacy _KortzCenterGuardShipments{"kortzcenterheistguardshipments", "Guard Shipments", "Guard shipments", true};
		static CommandToggleLegacy _KortzCenterGuardRoutesPrep{"kortzcenterheistguardroutesprep", "Guard Routes (Prep)", "Guard routes prep", true};
		static CommandToggleLegacy _KortzCenterGlassCutterPrep{"kortzcenterheistglasscutterprep", "Glass Cutter (Prep)", "Glass cutter prep", true};
		static CommandToggleLegacy _KortzCenterPowerDrillsPrep{"kortzcenterheistpowerdrillsprep", "Power Drills (Prep)", "Power drills prep", true};
		static CommandToggleLegacy _KortzCenterEMPChargesPrep{"kortzcenterheistempchargesprep", "EMP Charges 2 (Prep)", "EMP charges prep", true};
		static CommandToggleLegacy _KortzCenterCaracaraPrep{"kortzcenterheistcaracaraprep", "Caracara (Prep)", "Armored Caracara prep", true};
		static CommandToggleLegacy _KortzCenterAnnihilatorPrep{"kortzcenterheistannihilatorprep", "Annihilator (Prep)", "Annihilator stealth prep", true};
		static CommandToggleLegacy _KortzCenterManchezPrep{"kortzcenterheistmanchezprep", "Manchez (Prep)", "Manchez prep", true};
		static CommandToggleLegacy _KortzCenterScopeSecondary{"kortzcenterheistscopesecondary", "Secondary Targets", "Scope secondary targets", true};
		static CommandToggleLegacy _KortzCenterScopePOI{"kortzcenterheistscopepoi", "Points of Interest", "Scope points of interest", true};

		class Setup : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				int generalBits = -1;
				if (!_KortzCenterGuardRoutes.GetState()) generalBits &= ~(1 << 5);
				if (!_KortzCenterGlassCutter.GetState()) generalBits &= ~(1 << 6);
				if (!_KortzCenterPowerDrills.GetState()) generalBits &= ~(1 << 7);
				if (!_KortzCenterEMPCharges.GetState()) generalBits &= ~(1 << 8);

				int robberyProg = -1;
				if (!_KortzCenterScopeOut.GetState()) robberyProg &= ~(1 << 0);
				if (!_KortzCenterAlphaMail.GetState()) robberyProg &= ~(1 << 1);
				if (!_KortzCenterHazmat.GetState()) robberyProg &= ~(1 << 2);
				if (!_KortzCenterStaffKeycard.GetState()) robberyProg &= ~(1 << 3);
				if (!_KortzCenterTacticalEquip.GetState()) robberyProg &= ~(1 << 4);
				if (!_KortzCenterHackingDevice.GetState()) robberyProg &= ~(1 << 5);
				if (!_KortzCenterAccessCode.GetState()) robberyProg &= ~(1 << 6);
				if (!_KortzCenterUnmarkedWeapons.GetState()) robberyProg &= ~(1 << 7);
				if (!_KortzCenterCaracara.GetState()) robberyProg &= ~(1 << 8);
				if (!_KortzCenterAnnihilator.GetState()) robberyProg &= ~(1 << 9);
				if (!_KortzCenterManchez.GetState()) robberyProg &= ~(1 << 10);
				if (!_KortzCenterPrepEMP.GetState()) robberyProg &= ~(1 << 11);
				if (!_KortzCenterGuardShipments.GetState()) robberyProg &= ~(1 << 12);
				if (!_KortzCenterGuardRoutesPrep.GetState()) robberyProg &= ~(1 << 13);
				if (!_KortzCenterGlassCutterPrep.GetState()) robberyProg &= ~(1 << 14);
				if (!_KortzCenterPowerDrillsPrep.GetState()) robberyProg &= ~(1 << 15);

				int scopingBS = _KortzCenterScopeSecondary.GetState() ? -1 : 0;
				int poiBS = _KortzCenterScopePOI.GetState() ? -1 : 0;
				int caracaraPrepBS = _KortzCenterCaracaraPrep.GetState() ? -1 : 0;
				int annihilatorPrepBS = _KortzCenterAnnihilatorPrep.GetState() ? -1 : 0;
				int manchezPrepBS = _KortzCenterManchezPrep.GetState() ? -1 : 0;
				int empChargesPrepBS = _KortzCenterEMPChargesPrep.GetState() ? -1 : 0;

				Stats::SetInt("MPX_K26_GENERAL_BS", generalBits);
				Stats::SetInt("MPX_K26_GENERAL_BS2", -1);
				Stats::SetInt("MPX_K26_ROBBERY_PROG", robberyProg);
				Stats::SetInt("MPX_K26_SCOPING_BS", scopingBS);
				Stats::SetInt("MPX_K26_POI_BS", poiBS);
				Stats::SetInt("MPX_K26_HEIST_TARGET", _KortzCenterPrimaryTarget.GetState());
			}
		};

		class SkipFingerprint : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
					*ScriptLocal(thread, 26866).As<int*>() = 5;
			}
		};

		class SkipSignalNodes : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
					*ScriptLocal(thread, 27914).As<int*>() = 5;
			}
		};

		class SkipDataCrack : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
				{
					for (int i = 0; i <= 7; i++)
						*ScriptLocal(thread, 1388).At(i, 4).As<int*>() = 1;
				}
			}
		};

		class CutGlass : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
					*ScriptLocal(thread, 32855).At(4, 13).At(3).As<float*>() = 100.0f;
			}
		};

		class DisableLaserGrid : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
				{
					*ScriptLocal(thread, 70416).As<int*>() = 4294784;
					*ScriptGlobal(1935711).As<int*>() |= 1;
				}
			}
		};

		class TakePrimaryTarget : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
					*ScriptLocal(thread, 29355).At(11).As<int*>() = 10;
			}
		};

		class TakeSecondaryTarget : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
					*ScriptLocal(thread, 29355).At(11).As<int*>() = 3;
			}
		};

		class AutoEnterPcAccessCode : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_v3"_J))
				{
					for (int i = 0; i <= 2; i++)
					{
						*ScriptLocal(thread, 32818).At(1).At(i, 2).At(1).As<int*>() = 0;
						Script::current()->yield(100);
						PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 237, 1.0);
					}
				}
			}
		};


		static SkipFingerprint _KortzCenterSkipFingerprint{"kortzcenterheistskipfingerprint", "Skip Fingerprint Hack", "Skips fingerprint hacking minigame in computer room"};
		static SkipSignalNodes _KortzCenterSkipSignalNodes{"kortzcenterheistskipsignalnodes", "Skip Signal Nodes", "Skips signal nodes hacking at vault keypad"};
		static SkipDataCrack _KortzCenterSkipDataCrack{"kortzcenterheistskipdatacrack", "Skip Data Crack", "Skips data crack minigame"};
		static CutGlass _KortzCenterCutGlass{"kortzcenterheistcutglass", "Cut Glass", "Cuts display case glass instantly"};
		static DisableLaserGrid _KortzCenterDisableLaser{"kortzcenterheistdisablelaser", "Disable Laser Grid", "Disables laser security grid"};
		static TakePrimaryTarget _KortzCenterTakePrimary{"kortzcenterheisttakeprimary", "Take Primary Target", "Takes primary target painting (stand near it)"};
		static TakeSecondaryTarget _KortzCenterTakeSecondary{"kortzcenterheisttakesecondary", "Take Secondary Target", "Takes secondary loot (stand near it)"};
		static Setup _KortzCenterSetup{"kortzcenterheistsetup", "Setup", "Sets up Kortz Center heist"};
		static AutoEnterPcAccessCode _KortzCenterAutoEnterPcAccessCode{"kortzcenterheistautoenterpcaccesscode", "Auto-Enter PC Access Code", "Automatically enters the PC access code"};
	}
}