#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Commands/LoopedCommand.hpp"
#include "World/Stats.hpp"
#include "Game/Pools.hpp"
#include "Scripting/Natives.hpp"
#include "World/DeleteObjectsByHash.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Network/Tunables.hpp"
#include "Rendering/Notifications.hpp"
#include "World/TeleportUtils.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	namespace DiamondCasinoHeist
	{
		struct TeleportLocation
		{
			float x;
			float y;
			float z;
			float heading;
			const char* name;
		};

		static CommandSliderLegacy _DiamondCasinoHeistCut1{"diamondcasinoheistcut1", "Player 1", "Player 1 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DiamondCasinoHeistCut2{"diamondcasinoheistcut2", "Player 2", "Player 2 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DiamondCasinoHeistCut3{"diamondcasinoheistcut3", "Player 3", "Player 3 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DiamondCasinoHeistCut4{"diamondcasinoheistcut4", "Player 4", "Player 4 cut", std::nullopt, std::nullopt, 0};

		class SetCuts : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base = ScriptGlobal(1973762).At(1497).At(736).At(92);

				*base.At(0, 1).As<int*>() = _DiamondCasinoHeistCut1.GetState();
				*base.At(1, 1).As<int*>() = _DiamondCasinoHeistCut2.GetState();
				*base.At(2, 1).As<int*>() = _DiamondCasinoHeistCut3.GetState();
				*base.At(3, 1).As<int*>() = _DiamondCasinoHeistCut4.GetState();
			}
		};

		static std::vector<TeleportLocation> DiamondCasinoHeistTeleportPoints = {
		    {2542.052f, -214.3084f, -58.722965f, 0.0f, "Waste Disposal"},
		    {2547.9192f, -273.16754f, -58.723003f, 0.0f, "Staff Lobby"},
		    {2465.4746f, -279.2276f, -70.694145f, 0.0f, "Mantrap Door"},
		    {2515.1252f, -238.91661f, -70.73713f, 0.0f, "Inside The Vault"},
		    {2497.5098f, -238.50768f, -70.7388f, 0.0f, "Outside The Vault"},
		    {2520.8645f, -286.30685f, -58.723007f, 0.0f, "Daily Cash Storage"}};

		static std::vector<std::pair<int, const char*>> DiamondCasinoHeistTeleportList = {
		    {0, "Waste Disposal"},
		    {1, "Staff Lobby"},
		    {2, "Mantrap Door"},
		    {3, "Inside The Vault"},
		    {4, "Outside The Vault"},
		    {5, "Daily Cash Storage"}};

		static CommandListSelect _DiamondCasinoHeistTeleportList{"diamondcasinoheistteleportlist", "TP", "Teleport Location", DiamondCasinoHeistTeleportList, 0};

		static std::vector<TeleportLocation> DiamondCasinoHeistOtherTeleportPoints = {
		    {917.24634f, 48.989567f, 80.89892f, 0.0f, "Main Gate"},
		    {965.14856f, -9.05023f, 80.63045f, 0.0f, "Staff Lobby"},
		    {997.5346f, 84.51491f, 80.990555f, 0.0f, "Waste Disposal"},
		    {988.54395f, 80.86057f, 80.9906f, 0.0f, "The Music Locker"},
		    {922.816223f, 47.206078f, 81.106331f, 0.0f, "Casino Stand"}};

		static std::vector<std::pair<int, const char*>> DiamondCasinoHeistOtherTeleportList = {
		    {0, "Main Gate"},
		    {1, "Staff Lobby"},
		    {2, "Waste Disposal"},
		    {3, "The Music Locker"},
		    {4, "Casino Stand"}};

		static CommandListSelect _DiamondCasinoHeistOtherTeleportList{"diamondcasinoheistotherteleportlist", "Other TP", "Other teleport locations", DiamondCasinoHeistOtherTeleportList, 0};

		static std::vector<std::pair<int, const char*>> DiamondCasinoHeistPlayers = {
		    {1, "1 Player"},
		    {2, "2 Players"},
		    {3, "3 Players"},
		    {4, "4 Players"}};

		static CommandListSelect _DiamondCasinoHeistPlayers{"diamondcasinoheistplayers", "Players", "How many players are in the heist", DiamondCasinoHeistPlayers, 1};

		class ForceReady : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base = ScriptGlobal(1978125);

				for (int i = 0; i <= 3; i++)
				{
					*base.At(i, 68).At(7).At(i, 1).As<int*>() = 1;
				}
			}
		};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistDifficulty = {
		    {0, "Normal"},
		    {1, "Hard"}};
		static CommandListSelect _DiamondCasinoHeistDifficulty{"diamondcasinoheistdifficulty", "Difficulty", "Heist difficulty", diamondCasinoHeistDifficulty, 0};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistPrimaryTarget = {
		    {3, "Diamonds"},
		    {1, "Gold"},
		    {2, "Artwork"},
		    {0, "Cash"}};
		static CommandListSelect _DiamondCasinoHeistPrimaryTarget{"diamondcasinoheistprimarytarget", "Primary Target", "Primary target", diamondCasinoHeistPrimaryTarget, 3};

		static CommandListSelect* _DiamondCasinoHeistGunmanPtr = nullptr;
		static CommandListSelect* _DiamondCasinoHeistApproachPtr = nullptr;

		static std::vector<std::vector<std::vector<std::pair<int, const char*>>>> diamondCasinoHeistWeapon = {
		    {{{0, "MK II Shotgun Loadout"}, {1, "MK II Rifle Loadout"}},
		        {{0, "MK II SMG Loadout"}, {1, "MK II Rifle Loadout"}},
		        {{0, "MK II Shotgun Loadout"}, {1, "MK II Rifle Loadout"}}},
		    {{{0, "Rifle Loadout"}, {1, "Shotgun Loadout"}},
		        {{0, "Rifle Loadout"}, {1, "Shotgun Loadout"}},
		        {{0, "Rifle Loadout"}, {1, "Shotgun Loadout"}}},
		    {{{0, "Combat PDW Loadout"}, {1, "Rifle Loadout"}},
		        {{0, "Shotgun Loadout"}, {1, "Rifle Loadout"}},
		        {{0, "Shotgun Loadout"}, {1, "Combat MG Loadout"}}},
		    {{{0, "SMG Loadout"}, {1, "Shotgun Loadout"}},
		        {{0, "Machine Pistol Loadout"}, {1, "Shotgun Loadout"}},
		        {{0, "SMG Loadout"}, {1, "Shotgun Loadout"}}},
		    {{{0, "Micro SMG Loadout"}, {1, "Machine Pistol Loadout"}},
		        {{0, "Micro SMG Loadout"}, {1, "Shotgun Loadout"}},
		        {{0, "Shotgun Loadout"}, {1, "Revolver Loadout"}}},
		    {{{0, "                                        "}, {1, ""}},
		        {{0, ""}, {1, ""}},
		        {{0, ""}, {1, ""}}}};
		static CommandListSelect _DiamondCasinoHeistWeapon{"diamondcasinoheistweapon", "Weapon", "Weapon", diamondCasinoHeistWeapon[5][0], 0};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistGunman = {
		    {0, "Chester McCoy"},
		    {1, "Gustavo Mota"},
		    {2, "Patrick McReary"},
		    {3, "Charlie Reed"},
		    {4, "Karl Abolaji"},
		    {5, "Remove Gunman"}};

		class Gunman : public CommandListSelect
		{
			using CommandListSelect::CommandListSelect;

			virtual void OnChange() override
			{
				_DiamondCasinoHeistWeapon.SetList(diamondCasinoHeistWeapon[this->GetState()][_DiamondCasinoHeistApproachPtr->GetState()]);
				_DiamondCasinoHeistWeapon.SetState(0);
			}
		};
		static Gunman _DiamondCasinoHeistGunman{"diamondcasinoheistgunman", "Gunman", "Gunman", diamondCasinoHeistGunman, 5};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistApproach = {
		    {0, "Silent & Sneaky"},
		    {1, "The Big Con"},
		    {2, "Aggressive"}};

		class Approach : public CommandListSelect
		{
			using CommandListSelect::CommandListSelect;

			virtual void OnChange() override
			{
				_DiamondCasinoHeistWeapon.SetList(diamondCasinoHeistWeapon[_DiamondCasinoHeistGunmanPtr->GetState()][this->GetState()]);
				_DiamondCasinoHeistWeapon.SetState(0);
			}
		};
		static Approach _DiamondCasinoHeistApproach{"diamondcasinoheistapproach", "Approach", "Heist approach", diamondCasinoHeistApproach, 0};

		static struct InitPtrs
		{
			InitPtrs()
			{
				_DiamondCasinoHeistGunmanPtr = &_DiamondCasinoHeistGunman;
				_DiamondCasinoHeistApproachPtr = &_DiamondCasinoHeistApproach;
			}
		} _initPtrs;

		static std::vector<std::vector<std::pair<int, const char*>>> diamondCasinoHeistVehicle = {
		    {{0, "Zhaba"}, {1, "Vagrant"}, {2, "Outlaw"}, {3, "Everon"}},
		    {{0, "Sultan Classic"}, {1, "Gauntlet Classic"}, {2, "Ellie"}, {3, "Komoda"}},
		    {{0, "Retinue MK II"}, {1, "Drifty Yosemite"}, {2, "Sugoi"}, {3, "Jugular"}},
		    {{0, "Manchez"}, {1, "Stryder"}, {2, "Defiler"}, {3, "Lectro"}},
		    {{0, "Issi Classic"}, {1, "Asbo"}, {2, "Kanjo"}, {3, "Sentinel Classic"}},
		    {{0, "                           "}, {1, ""}, {2, ""}, {3, ""}}};
		static CommandListSelect _DiamondCasinoHeistVehicle{"diamondcasinoheistvehicle", "Vehicle", "Vehicle", diamondCasinoHeistVehicle[5], 0};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistDriver = {
		    {0, "Chester McCoy"},
		    {1, "Eddie Toh"},
		    {2, "Taliana Martinez"},
		    {3, "Zach Nelson"},
		    {4, "Karim Denz"},
		    {5, "Remove Driver"}};

		class Driver : public CommandListSelect
		{
			using CommandListSelect::CommandListSelect;

			virtual void OnChange() override
			{
				_DiamondCasinoHeistVehicle.SetList(diamondCasinoHeistVehicle[this->GetState()]);
				_DiamondCasinoHeistVehicle.SetState(0);
			}
		};
		static Driver _DiamondCasinoHeistDriver{"diamondcasinoheistdriver", "Driver", "Driver", diamondCasinoHeistDriver, 5};

		static std::vector<std::pair<int, const char*>> diamondCasinoHeistHacker = {
		    {4, "Avi Schwartzman"},
		    {5, "Paige Harris"},
		    {2, "Christian Feltz"},
		    {3, "Yohan Blair"},
		    {1, "Rickie Lukens"},
		    {6, "Remove Hacker"}};
		static CommandListSelect _DiamondCasinoHeistHacker{"diamondcasinoheisthacker", "Hacker", "Hacker", diamondCasinoHeistHacker, 6};

		class Setup : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				Stats::SetInt("MPX_H3_COMPLETEDPOSIX", -1);
				Stats::SetInt("MPX_H3OPT_MASKS", 4);
				Stats::SetInt("MPX_H3OPT_WEAPS", _DiamondCasinoHeistWeapon.GetState());
				Stats::SetInt("MPX_H3OPT_VEHS", _DiamondCasinoHeistVehicle.GetState());
				Stats::SetInt("MPX_CAS_HEIST_FLOW", -1);
				Stats::SetInt("MPX_H3_LAST_APPROACH", 0);
				Stats::SetInt("MPX_H3OPT_APPROACH", _DiamondCasinoHeistApproach.GetState() + 1); // 1 = Silent & Sneaky, 2 = The Big Con, 3 = Aggressive

				switch (_DiamondCasinoHeistDifficulty.GetState())
				{
				case 0: Stats::SetInt("MPX_H3_HARD_APPROACH", 0); break;
				case 1: Stats::SetInt("MPX_H3_HARD_APPROACH", _DiamondCasinoHeistApproach.GetState() + 1); break;
				default: break;
				}

				Stats::SetInt("MPX_H3OPT_TARGET", _DiamondCasinoHeistPrimaryTarget.GetState());
				Stats::SetInt("MPX_H3OPT_POI", 1023);
				Stats::SetInt("MPX_H3OPT_ACCESSPOINTS", 2047);

				switch (_DiamondCasinoHeistGunman.GetState())
				{
				case 0: Stats::SetInt("MPX_H3OPT_CREWWEAP", 4); break;
				case 1: Stats::SetInt("MPX_H3OPT_CREWWEAP", 2); break;
				case 2: Stats::SetInt("MPX_H3OPT_CREWWEAP", 5); break;
				case 3: Stats::SetInt("MPX_H3OPT_CREWWEAP", 3); break;
				case 4: Stats::SetInt("MPX_H3OPT_CREWWEAP", 1); break;
				case 5: Stats::SetInt("MPX_H3OPT_CREWWEAP", 6); break;
				default: break;
				}

				switch (_DiamondCasinoHeistDriver.GetState())
				{
				case 0: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 5); break;
				case 1: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 3); break;
				case 2: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 2); break;
				case 3: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 4); break;
				case 4: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 1); break;
				case 5: Stats::SetInt("MPX_H3OPT_CREWDRIVER", 6); break;
				default: break;
				}

				Stats::SetInt("MPX_H3OPT_CREWHACKER", _DiamondCasinoHeistHacker.GetState());
				Stats::SetInt("MPX_H3OPT_DISRUPTSHIP", 3); // Security Strength: 1 = Strong, 2 = Medium, 3 = Weak
				Stats::SetInt("MPX_H3OPT_BODYARMORLVL", -1);
				Stats::SetInt("MPX_H3OPT_KEYLEVELS", 2); // 1 = Level1 Security Pass, 2 = Level2 Security Pass

				Stats::SetInt("MPX_H3OPT_BITSET0", rand() % (INT_MAX)); // Refresh board
				Stats::SetInt("MPX_H3OPT_BITSET1", rand() % (INT_MAX)); // Refresh board

				Script::current()->yield(500);

				Stats::SetInt("MPX_H3OPT_BITSET0", -1); // Refresh board
				Stats::SetInt("MPX_H3OPT_BITSET1", -1); // Refresh board

				Notifications::ShowInGame("Diamond Casino", "Diamond Casino Hesit Setup Completed", "CHAR_LESTER", "Green");
			}
		};

		static CommandSliderLegacy _DiamondCasinoHeistPotentialTake{"diamondcasinoheistpotentialtake", "Potential Take", "Updates potential take", std::nullopt, std::nullopt, 0};

		class SetPotentialTake : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				int primary_target = Stats::GetInt("MPX_H3OPT_TARGET");

				switch (primary_target)
				{
				case 0:
				{
					static Tunable tunable{static_cast<joaat_t>(-1638885821)};
					if (tunable.IsReady())
						tunable.Set(_DiamondCasinoHeistPotentialTake.GetState());
					break;
				} // "Cash"
				case 1:
				{
					static Tunable tunable{static_cast<joaat_t>(-582734553)};
					if (tunable.IsReady())
						tunable.Set(_DiamondCasinoHeistPotentialTake.GetState());
					break;
				} // "Gold"
				case 2:
				{
					static Tunable tunable{1934398910};
					if (tunable.IsReady())
						tunable.Set(_DiamondCasinoHeistPotentialTake.GetState());
					break;
				} // "Artwork"
				case 3:
				{
					static Tunable tunable{1277889925};
					if (tunable.IsReady())
						tunable.Set(_DiamondCasinoHeistPotentialTake.GetState());
					break;
				} // "Diamond"
				default: break;
				}
			}
		};

		static CommandSliderLegacy _DiamondCasinoHeistActualTake{"diamondcasinoheistactualtake", "Actual Take", "Updates actual take", std::nullopt, std::nullopt, 0};

		class SetActualTake : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
					*ScriptLocal(thread, 20412).At(2686).As<int*>() = _DiamondCasinoHeistActualTake.GetState();
			}
		};

		class SetMaxPayout : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

		public:
			virtual void OnCall() override
			{
				bool hard = (_DiamondCasinoHeistDifficulty.GetState() == 1);
				int target = _DiamondCasinoHeistPrimaryTarget.GetState();

				int gunman = MapGunman(_DiamondCasinoHeistGunman.GetState());
				int driver = MapDriver(_DiamondCasinoHeistDriver.GetState());
				int hacker = _DiamondCasinoHeistHacker.GetState();

				int players = _DiamondCasinoHeistPlayers.GetState();

				int cut = CalculateCut(target, hard, gunman, driver, hacker);

				if (cut > 0)
					ApplyCuts(cut, players);

				Notifications::ShowInGame("Diamond Casino", "Max Payout Set - Successful", "CHAR_LESTER", "Green");
			}

		private:
			int MapGunman(int state)
			{
				switch (state)
				{
				case 0: return 4;
				case 1: return 2;
				case 2: return 5;
				case 3: return 3;
				case 4: return 1;
				default: return 6;
				}
			}

			int MapDriver(int state)
			{
				switch (state)
				{
				case 0: return 5;
				case 1: return 3;
				case 2: return 2;
				case 3: return 4;
				case 4: return 1;
				default: return 6;
				}
			}

			int CalculateCut(int target, bool hard, int gunman, int driver, int hacker)
			{
				static std::unordered_map<int, std::pair<int, int>> payouts = {
				    {0, {2115000, 2326500}}, // Cash
				    {2, {2350000, 2585000}}, // Artwork
				    {1, {2585000, 2843500}}, // Gold
				    {3, {3290000, 3619000}}  // Diamonds
				};

				if (!payouts.contains(target))
					return 0;

				float payout = hard ? payouts[target].second : payouts[target].first;
				payout += 819000.0f;

				constexpr float maxPayout = 3'619'000.0f;

				static std::unordered_map<int, float> gunmanCuts = {
				    {1, 0.05f},
				    {3, 0.07f},
				    {5, 0.08f},
				    {2, 0.09f},
				    {4, 0.10f}};
				static std::unordered_map<int, float> driverCuts = {
				    {1, 0.05f},
				    {4, 0.06f},
				    {2, 0.07f},
				    {3, 0.09f},
				    {5, 0.10f}};
				static std::unordered_map<int, float> hackerCuts = {
				    {1, 0.03f},
				    {3, 0.05f},
				    {2, 0.07f},
				    {5, 0.09f},
				    {4, 0.10f}};

				if (!gunmanCuts.contains(gunman) || !driverCuts.contains(driver) || !hackerCuts.contains(hacker))
					return 0;

				float buyerFee = 0.10f;
				float lester = 0.05f;

				float feePayout = payout * (1.0f - buyerFee);

				float crew =
				    (feePayout * lester) + (feePayout * gunmanCuts[gunman]) + (feePayout * driverCuts[driver]) + (feePayout * hackerCuts[hacker]);

				float playerTake = feePayout - crew;

				if (playerTake <= 0.0f)
					return 0;

				int cut = static_cast<int>(maxPayout / (playerTake / 100.0f));
				return std::clamp(cut, 100, 500);
			}

			void ApplyCuts(int totalCut, int players)
			{
				std::array<CommandSliderLegacy*, 4> cmds = {
				    &_DiamondCasinoHeistCut1,
				    &_DiamondCasinoHeistCut2,
				    &_DiamondCasinoHeistCut3,
				    &_DiamondCasinoHeistCut4};

				int per = totalCut / players;
				int rem = totalCut % players;

				for (int i = 0; i < 4; ++i)
				{
					if (i < players)
						cmds[i]->SetState(per + (i == 0 ? rem : 0));
					else
						cmds[i]->SetState(0);
				}
			}
		};


		class SkipHacking : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					*ScriptLocal(thread, 55028).As<int*>() = 5;
					*ScriptLocal(thread, 56098).As<int*>() = 5;
				}
			}
		};

		class SkipDrilling : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
					*ScriptLocal(thread, 10567).At(7).As<int*>() = *ScriptLocal(thread, 10567).At(37).As<int*>();
			}
		};

		class SoloMantrap : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					*ScriptLocal(thread, 32785).At(Self::GetPlayer().GetId(), 294).At(143).As<int*>() = 8;
					*ScriptLocal(thread, 64655).As<int*>() = 5;
				}
			}
		};

		class InstantFinish : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					Scripts::ForceScriptHost(thread);
					Script::current()->yield(500);

					*ScriptLocal(thread, 20412).At(1740).At(0, 1).As<int*>() = 80;
					*ScriptLocal(thread, 20412).At(2686).As<int*>() = 4443220;
					*ScriptLocal(thread, 20412).At(1062).As<int*>() = 5;
					*ScriptLocal(thread, 20412).As<int*>() = 12;
					*ScriptLocal(thread, 29326).At(0, 1).As<int*>() = 99999;
					*ScriptLocal(thread, 32785).At(0, 294).At(68).As<int*>() = 99999;
				}
			}
		};

		class RemoveCameras : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			static const inline std::vector<Hash> CasinoCameraHashes = {
			    "prop_cctv_cam_01a"_J,
			    "prop_cctv_cam_01b"_J,
			    "prop_cctv_cam_02a"_J,
			    "prop_cctv_cam_03a"_J,
			    "prop_cctv_cam_04a"_J,
			    "prop_cctv_cam_04c"_J,
			    "prop_cctv_cam_05a"_J,
			    "prop_cctv_cam_06a"_J,
			    "prop_cctv_cam_07a"_J,
			    "prop_cs_cctv"_J,
			    "p_cctv_s"_J,
			    "hei_prop_bank_cctv_01"_J,
			    "hei_prop_bank_cctv_02"_J,
			    "ch_prop_ch_cctv_cam_02a"_J,
			    "xm_prop_x17_server_farm_cctv_01"_J,
			};

			virtual void OnCall() override
			{
				for (auto object : Stand::Pools::GetObjects())
				{
					if (!object)
						continue;

					int handle = object.GetHandle();
					Hash model = ENTITY::GET_ENTITY_MODEL(handle);

					if (std::find(CasinoCameraHashes.begin(), CasinoCameraHashes.end(), model) != CasinoCameraHashes.end())
					{
						if (object.RequestControl(3000))
						{
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(handle, -8271.156f, -1293.2153f, -100.0f, false, false, false);
						}
					}
				}
			}
		};

		class RemoveKeycard : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				Hash keycard = "ch_prop_fingerprint_scanner_01d"_J;
				Stand::DeleteObjectsByHash(keycard);
				Hash tunnerdoorright = "ch_prop_ch_tunnel_door_01_r"_J;
				Stand::DeleteObjectsByHash(tunnerdoorright);
				Hash tunnerdoorleft = "ch_prop_ch_tunnel_door_01_l"_J;
				Stand::DeleteObjectsByHash(tunnerdoorleft);
			}
		};

		class Teleport : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				int index = _DiamondCasinoHeistTeleportList.GetState();

				if (index >= 0 && index < 6)
				{
					const auto& tp = DiamondCasinoHeistTeleportPoints[index];

					TeleportHelpers::TeleportEntityTo(TeleportHelpers::MakePlace(tp.name, tp.x, tp.y, tp.z, tp.heading));
				}
				else if (index == 6)
				{
					int otherIndex = _DiamondCasinoHeistOtherTeleportList.GetState();

					if (otherIndex < 0 || otherIndex >= DiamondCasinoHeistOtherTeleportPoints.size())
						return;

					const auto& tp = DiamondCasinoHeistOtherTeleportPoints[otherIndex];

					TeleportHelpers::TeleportEntityTo(TeleportHelpers::MakePlace(tp.name, tp.x, tp.y, tp.z, tp.heading));
				}
			}
		};

		class OtherTeleport : public LoopedCommand
		{
			using LoopedCommand::LoopedCommand;

			virtual void OnTick() override
			{
				bool show = (_DiamondCasinoHeistTeleportList.GetState() == 6);
			}
		};

		static SetMaxPayout _DiamondCasinoHeistSetMaxPayout{"diamondcasinoheistsetmaxpayout", "Set Max Cut", "Automatically calculates and sets player cuts"};
		static SetCuts _DiamondCasinoHeistSetCuts{"diamondcasinoheistsetcuts", "Auto Set Cuts", "Sets heist cut"};
		static ForceReady _DiamondCasinoHeistForceReady{"diamondcasinoheistforceready", "Force Ready", "Forces all players to be ready"};
		static Setup _DiamondCasinoHeistSetup{"diamondcasinoheistsetup", "Setup", "Sets up diamond casino heist"};
		static SetPotentialTake _DiamondCasinoHeistSetPotentialTake{"diamondcasinoheistsetpotentialtake", "Set Potential Take", "Updates potential take"};
		static SetActualTake _DiamondCasinoHeistSetActualTake{"diamondcasinoheistsetactualtake", "Set Actual Take", "Updates actual take"};
		static SkipHacking _DiamondCasinoHeistSkipHacking{"diamondcasinoheistskiphacking", "Skip Hacking", "Skips hacking process"};
		static SkipDrilling _DiamondCasinoHeistSkipDrilling{"diamondcasinoheistskipdrilling", "Skip Drilling", "Skips drilling process"};
		static SoloMantrap _DiamondCasinoHeistSoloMantrap{"diamondcasinoheistsolomantrap", "Solo Mantrap", "Skips card swiping process"};
		static RemoveCameras _DiamondCasinoHeistRemoveCameras{"diamondcasinoheistremovecameras", "Remove Cam", "Removes all cameras"};
		static RemoveKeycard _DiamondCasinoHeistKeycard{"diamondcasinoheistkeycard", "Remove Keycard", "Removes keycard"};
		static InstantFinish _DiamondCasinoHeistInstantFinish{"diamondcasinoheistinstantfinish", "Instant Finish", "Instantly passes the heist"};
		static Teleport _DiamondCasinoHeistTeleport{"diamondcasinoheistteleport", "Teleport", "Teleport to selected casino location"};
		// static OtherTeleport _DiamondCasinoHeistOtherTeleport{"diamondcasinoheistotherteleport", "Other Teleport", "Teleport to selected other casino location"};
	}
}