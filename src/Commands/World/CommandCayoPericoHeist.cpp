#include "Commands/CommandLegacy.hpp"
#include <unordered_map>
#include <array>
#include <cmath>
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"
#include "World/Stats.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Network/Tunables.hpp"
#include "World/TeleportUtils.hpp"
#include "Rendering/Notifications.hpp"
#include "World/DeleteObjectsByHash.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand::Features
{
	namespace CayoPericoHeist
	{
		struct TeleportLocation
		{
			float x;
			float y;
			float z;
			float heading;
			const char* name;
		};


		static CommandSliderLegacy _CayoPericoHeistCut1{"cayopericoheistcut1", "Player 1", "Player 1 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _CayoPericoHeistCut2{"cayopericoheistcut2", "Player 2", "Player 2 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _CayoPericoHeistCut3{"cayopericoheistcut3", "Player 3", "Player 3 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _CayoPericoHeistCut4{"cayopericoheistcut4", "Player 4", "Player 4 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _CayoPavelCut{"cayopavelcut", "Pavel Cut (%)", "Pavel cut percentage", std::nullopt, std::nullopt, 2};
		static CommandSliderLegacy _CayoFencingCut{"cayofencecut", "Fencing Fee (%)", "Fencing fee percentage", std::nullopt, std::nullopt, 10};


		class SetCuts : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base = ScriptGlobal(1980570).At(831).At(56);

				*base.At(0, 1).As<int*>() = _CayoPericoHeistCut1.GetState();
				*base.At(1, 1).As<int*>() = _CayoPericoHeistCut2.GetState();
				*base.At(2, 1).As<int*>() = _CayoPericoHeistCut3.GetState();
				*base.At(3, 1).As<int*>() = _CayoPericoHeistCut4.GetState();
				Notifications::ShowInGame("Cayo Perico", "Apply Cut - Successfull", "CHAR_PAVEL", "Green");
			}
		};

		class ForceReady : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base = ScriptGlobal(1973762);

				for (int i = 0; i <= 3; i++)
				{
					*base.At(i, 27).At(7).At(i, 1).As<int*>() = 1;
				}
				Notifications::ShowInGame("Cayo Perico", "Force Ready Set - Successfull", "CHAR_PAVEL", "Green");
			}
		};

		static std::vector<TeleportLocation> cayoPericoTeleportPoints = {
		    {5010.12f, -5750.1353f, 28.84334f, 325.0f, "El Rubio's Office"},
		    {4990.0386f, -5717.6895f, 19.880217f, 50.0f, "Front Gate Exit"},
		    {5044.001f, -5815.6426f, -11.808871f, 0.0f, "Drainage Pipe"},
		    {5053.773f, -5773.2266f, -5.40778f, 0.0f, "Drainage: 2nd Checkpoint"},
		    {5006.7f, -5756.2f, 14.8f, 145.0f, "Primary Target"},
		    {4999.764f, -5749.864f, 14.84f, 0.0f, "Secondary Target"}};

		static std::vector<std::pair<int, const char*>> cayoPericoTeleportList = {
		    {0, "El Rubio's Office"},
		    {1, "Front Gate Exit"},
		    {2, "Drainage Pipe"},
		    {3, "Drainage: 2nd Checkpoint"},
		    {4, "Primary Target"},
		    {5, "Secondary Target"},
		    {6, "Others"}};

		static CommandListSelect _CayoPericoTeleportList{"cayopericoteleportlist", "Teleport Locations", "Teleport Location", cayoPericoTeleportList, 0};

		static std::vector<TeleportLocation> cayoPericoOtherTeleportPoints = {
		    {5081.0415f, -5755.32f, 15.829645f, -45.0f, "North"},
		    {5006.722f, -5786.597f, 17.831688f, 35.0f, "West"},
		    {5027.603f, -5734.682f, 17.255005f, -50.0f, "South"},
		    {4503.587402f, -4555.740723f, 2.854459f, 0.0f, "Loot - #1 (Beach)"},
		    {4437.821777f, -4447.841309f, 3.028436f, 0.0f, "Loot - #2 (Beach)"},
		    {4447.091797f, -4442.184082f, 5.936794f, 0.0f, "Loot - #3 (Beach)"},
		    {5330.527f, -5269.7515f, 33.18603f, 0.0f, "Loot - #1 (Hill)"},
		    {5193.909668f, -5135.642578f, 2.045917f, 0.0f, "Loot - #2 (Hill)"},
		    {4963.184570f, -5108.933105f, 1.670808f, 0.0f, "Loot - #3 (Hill)"},
		    {4998.709473f, -5165.559570f, 1.464137f, 0.0f, "Loot - #4 (Hill)"},
		    {4924.693359f, -5243.244629f, 1.223599f, 0.0f, "Loot - #5 (Hill)"},
		    {5132.558594f, -4612.922852f, 1.162808f, 0.0f, "Loot - #1 (Dock)"},
		    {5065.255371f, -4591.706543f, 1.555012f, 0.0f, "Loot - #2 (Dock)"},
		    {5090.916016f, -4682.670898f, 1.107098f, 0.0f, "Loot - #3 (Dock)"},

		};

		static std::vector<std::pair<int, const char*>> cayoPericoOtherTeleportList = {
		    {0, "North"},
		    {1, "West"},
		    {2, "South"},
		    {3, "Loot - #1 (Beach)"},
		    {4, "Loot - #2 (Beach)"},
		    {5, "Loot - #3 (Beach)"},
		    {6, "Loot - #1 (Hill)"},
		    {7, "Loot - #2 (Hill)"},
		    {8, "Loot - #3 (Hill)"},
		    {9, "Loot - #4 (Hill)"},
		    {10, "Loot - #5 (Hill)"},
		    {11, "Loot - #1 (Dock)"},
		    {12, "Loot - #2 (Dock)"},
		    {13, "Loot - #3 (Dock)"},
		};

		static CommandListSelect _CayoPericoOtherTeleportList{"cayopericootherteleportlist", "Other TP", "Other teleport locations", cayoPericoOtherTeleportList, 0};

		static std::vector<std::pair<int, const char*>> cayoPlayers = {
		    {1, "1 Player"},
		    {2, "2 Players"},
		    {3, "3 Players"},
		    {4, "4 Players"}};

		static CommandListSelect _CayoPericoHeistPlayers{"cayopericoheistplayers", "Players", "How many players are in the heist", cayoPlayers, 1};

		static std::vector<std::pair<int, const char*>> cayoPericoHeistDifficulty = {
		    {126823, "Normal"},
		    {131055, "Hard"}};
		static CommandListSelect _CayoPericoHeistDifficulty{"cayopericoheistdifficulty", "Difficulty", "Heist difficulty", cayoPericoHeistDifficulty, 126823};

		static std::vector<std::pair<int, const char*>> cayoPericoHeistPrimaryTarget = {
		    {5, "Panther Statue"},
		    {3, "Pink Diamond"},
		    {4, "Madrazo Files"},
		    {2, "Bearer Bonds"},
		    {1, "Ruby Necklace"},
		    {0, "Sinsimito Tequila"}};
		static CommandListSelect _CayoPericoHeistPrimaryTarget{"cayopericoheistprimarytarget", "Primary Target", "Primary target", cayoPericoHeistPrimaryTarget, 5};

		static std::vector<std::pair<int, const char*>> cayoPericoHeistWeapon = {
		    {1, "Aggressor"},
		    {2, "Conspirator"},
		    {3, "Crack Shot"},
		    {4, "Saboteur"},
		    {5, "Marksman"}};
		static CommandListSelect _CayoPericoHeistWeapon{"cayopericoheistweapon", "Weapon", "Weapon category", cayoPericoHeistWeapon, 1};

		class RequestKosatka : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				*ScriptGlobal(2733326).At(613).As<int*>() = 1;
			}
		};

		class Setup : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				Stats::SetInt("MPX_H4CNF_TARGET", _CayoPericoHeistPrimaryTarget.GetState()); // Primary target
				Stats::SetInt("MPX_H4LOOT_CASH_I", 16711680);
				Stats::SetInt("MPX_H4LOOT_CASH_I_SCOPED", 16711680);
				Stats::SetInt("MPX_H4LOOT_CASH_C", 0);
				Stats::SetInt("MPX_H4LOOT_CASH_C_SCOPED", 0);
				Stats::SetInt("MPX_H4LOOT_COKE_I", 255);
				Stats::SetInt("MPX_H4LOOT_COKE_I_SCOPED", 255);
				Stats::SetInt("MPX_H4LOOT_COKE_C", 0);
				Stats::SetInt("MPX_H4LOOT_COKE_C_SCOPED", 0);
				Stats::SetInt("MPX_H4LOOT_GOLD_I", 0);
				Stats::SetInt("MPX_H4LOOT_GOLD_I_SCOPED", 0);
				Stats::SetInt("MPX_H4LOOT_GOLD_C", 255);
				Stats::SetInt("MPX_H4LOOT_GOLD_C_SCOPED", 255);
				Stats::SetInt("MPX_H4LOOT_WEED_I", 65280);
				Stats::SetInt("MPX_H4LOOT_WEED_I_SCOPED", 65280);
				Stats::SetInt("MPX_H4LOOT_WEED_C", 0);
				Stats::SetInt("MPX_H4LOOT_WEED_C_SCOPED", 0);
				Stats::SetInt("MPX_H4LOOT_PAINT", 127);
				Stats::SetInt("MPX_H4LOOT_PAINT_SCOPED", 127);
				Stats::SetInt("MPX_H4LOOT_CASH_V", 83250);
				Stats::SetInt("MPX_H4LOOT_COKE_V", 202500);
				Stats::SetInt("MPX_H4LOOT_GOLD_V", 333333);
				Stats::SetInt("MPX_H4LOOT_WEED_V", 135000);
				Stats::SetInt("MPX_H4LOOT_PAINT_V", 180000);
				Stats::SetInt("MPX_H4_PROGRESS", _CayoPericoHeistDifficulty.GetState()); // Difficulty
				Stats::SetInt("MPX_H4CNF_BS_GEN", 262143);
				Stats::SetInt("MPX_H4CNF_BS_ENTR", 63);
				Stats::SetInt("MPX_H4CNF_BS_ABIL", 63);
				Stats::SetInt("MPX_H4CNF_WEP_DISRP", 3);
				Stats::SetInt("MPX_H4CNF_ARM_DISRP", 3);
				Stats::SetInt("MPX_H4CNF_HEL_DISRP", 3);
				Stats::SetInt("MPX_H4CNF_APPROACH", -1);
				Stats::SetInt("MPX_H4CNF_BOLTCUT", 4424);
				Stats::SetInt("MPX_H4CNF_UNIFORM", 5256);
				Stats::SetInt("MPX_H4CNF_GRAPPEL", 5156);
				Stats::SetInt("MPX_H4_MISSIONS", -1);
				Stats::SetInt("MPX_H4CNF_WEAPONS", _CayoPericoHeistWeapon.GetState()); // Weapon
				Stats::SetInt("MPX_H4CNF_TROJAN", 5);
				Stats::SetInt("MPX_H4_PLAYTHROUGH_STATUS", 100);

				Script::current()->yield(500);

				if (auto thread = Scripts::FindScriptThread("heist_island_planning"_J))
					*ScriptLocal(thread, 1580).As<int*>() = 2;

				Notifications::ShowInGame("Cayo Perico", "Cayo Perio Heist Setup Completed", "CHAR_PAVEL", "Green");
			}
		};

		static CommandSliderLegacy _CayoPericoHeistPrimaryTargetValue{"cayopericoheistprimarytargetvalue", "Primary Target Value", "Updates primary target value", std::nullopt, std::nullopt, 0};

		class SetPrimaryTargetValue : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto primary_target = Stats::GetInt("MPX_H4CNF_TARGET");

				switch (primary_target)
				{
				case 0:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_TEQUILA"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				case 1:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_PEARL_NECKLACE"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				case 2:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_BEARER_BONDS"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				case 3:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_PINK_DIAMOND"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				case 4:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_MADRAZO_FILES"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				case 5:
				{
					static Tunable tunable{"IH_PRIMARY_TARGET_VALUE_SAPPHIRE_PANTHER_STATUE"_J};
					if (tunable.IsReady())
						tunable.Set(_CayoPericoHeistPrimaryTargetValue.GetState());
					break;
				}
				default: break;
				}
			}
		};

		static CommandSliderLegacy _CayoPericoHeistSecondaryTakeValue{"cayopericoheistsecondarytakevalue", "Secondary Take Value", "Updates secondary take value", std::nullopt, std::nullopt, 0};

		class SetSecondaryTakeValue : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
					*ScriptLocal(thread, 59986).At(1376).At(53).As<int*>() = _CayoPericoHeistSecondaryTakeValue.GetState();
			}
		};

		class SetMaxPayout : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

		public:
			virtual void OnCall() override
			{
				int target = _CayoPericoHeistPrimaryTarget.GetState();
				bool hardMode = (_CayoPericoHeistDifficulty.GetState() == 131055);
				float pavel = _CayoPavelCut.GetState() / 100.0f;
				float fencing = _CayoFencingCut.GetState() / 100.0f;

				int players = _CayoPericoHeistPlayers.GetState();

				int cut = CalculateCut(target, hardMode, pavel, fencing);
				if (cut > 0)
					ApplyCuts(cut, players);
				Notifications::ShowInGame("Cayo Perico", "Max Payout Set - Successfull", "CHAR_PAVEL", "Black");
			}

		private:
			int CalculateCut(int target, bool hard, float pavel, float fencing)
			{
				static std::unordered_map<int, std::pair<int, int>> payouts = {
				    {0, {630000, 693000}},   // Tequila
				    {1, {700000, 770000}},   // Ruby Necklace
				    {2, {770000, 847000}},   // Bearer Bonds
				    {3, {1300000, 1430000}}, // Pink Diamond
				    {4, {1100000, 1210000}}, // Madrazo Files
				    {5, {1900000, 2090000}}  // Panther
				};

				if (!payouts.contains(target))
					return 0;

				int base = hard ? payouts[target].second : payouts[target].first;

				constexpr float maxTake = 2'550'000.0f;

				float feeMultiplier = 1.0f - (std::abs(pavel) + std::abs(fencing));
				if (feeMultiplier <= 0.0f)
					return 0;

				float requiredGross = maxTake / feeMultiplier;

				int cut = static_cast<int>((requiredGross / base) * 100.0f);

				return std::clamp(cut, 100, 500);
			}

			void ApplyCuts(int totalCut, int players)
			{
				std::array<CommandSliderLegacy*, 4> cmds = {
				    &_CayoPericoHeistCut1,
				    &_CayoPericoHeistCut2,
				    &_CayoPericoHeistCut3,
				    &_CayoPericoHeistCut4};

				int perPlayer = totalCut / players;
				int remainder = totalCut % players;

				for (int i = 0; i < 4; ++i)
				{
					if (i < players)
					{
						int value = perPlayer + (i == 0 ? remainder : 0); // host gets remainder
						cmds[i]->SetState(value);
					}
					else
					{
						cmds[i]->SetState(0);
					}
				}
			}
		};

		class SkipHacking : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
					*ScriptLocal(thread, 26619).As<int*>() = 5;
			}
		};

		class CutSewer : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
					*ScriptLocal(thread, 31511).As<int*>() = 6;
				Hash drainagePipeHash = "prop_chem_grill_bit"_J;
				Stand::DeleteObjectsByHash(drainagePipeHash);
				Notifications::ShowInGame("Cayo Perico", "Cut Sewer Grill - Successfull", "CHAR_LESTER", "Black");
			}
		};

		class CutGlass : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
					*ScriptLocal(thread, 32751).At(3).As<float*>() = 100.0f;
			}
		};

		class TakePrimaryTarget : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
				{
					auto ped = Self::GetPed();

					if (!ped)
						return;

					auto pos = ped.GetPosition();
					auto heading = ped.GetHeading();
					auto primary_target = Stats::GetInt("MPX_H4CNF_TARGET");

					FiberPool::queueJob([&] {
						TASK::TASK_GO_STRAIGHT_TO_COORD(ped.GetHandle(), 5006.917, -5755.931, 15.484, 1.0, 3, 15, 5);

						switch (primary_target)
						{
						case 0:
						case 1:
						case 3:
						case 5:
							*ScriptLocal(thread, 32750).As<int*>() = 5;
							*ScriptLocal(thread, 32751).As<int*>() = 3;
							break;
						case 2:
						case 4:
							*ScriptLocal(thread, 32725).As<int*>() = 7;
							break;
						default: break;
						}

						Script::current()->yield(4000);


						TASK::TASK_GO_STRAIGHT_TO_COORD(ped.GetHandle(), pos.x, pos.y, pos.z, 1.0, 3, heading, 5);
					});
				}
			}
		};

		class InfinitePlasmaCutterHeat : public LoopedCommand
		{
			using LoopedCommand::LoopedCommand;

			virtual void OnTick() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
				{
					*ScriptLocal(thread, 32751).At(4).As<float*>() = 0.0f; //DLC_H4_anims_glass_cutter_Sounds
				}
			}
		};

		class InstantFinish : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller_2020"_J))
				{
					Scripts::ForceScriptHost(thread);
					Script::current()->yield(500);

					*ScriptLocal(thread, 56504).As<int*>() = 9;
					*ScriptLocal(thread, 56504).At(1776).At(0, 1).As<int*>() = 50;
				}
			}
		};

		class FencingFeeandPavelCut : public LoopedCommand
		{
			using LoopedCommand::LoopedCommand;

		private:
			Tunable m_pavel{"IH_DEDUCTION_PAVEL_CUT"_J};
			Tunable m_fee{"IH_DEDUCTION_FENCING_FEE"_J};

			void Apply()
			{
				if (m_pavel.IsReady())
					m_pavel.Set(0.0f);

				if (m_fee.IsReady())
					m_fee.Set(0.0f);

				_CayoPavelCut.SetState(0);
				_CayoFencingCut.SetState(0);
			}

			void Reset()
			{
				if (m_pavel.IsReady())
					m_pavel.Set(-0.02f);

				if (m_fee.IsReady())
					m_fee.Set(-0.1f);

				_CayoPavelCut.SetState(2);
				_CayoFencingCut.SetState(10);
			}

		public:
			virtual void OnEnable() override
			{
				Apply();
			}

			virtual void OnDisable() override
			{
				Reset();
			}

			virtual void OnTick() override
			{
				Apply();
			}
		};

		class RemoveCameras : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			static const inline std::vector<Hash> CayoPericoCameraHashes = {
			    ("prop_cctv_cam_01a"_J),
			    ("prop_cctv_cam_01b"_J),
			    ("prop_cctv_cam_02a"_J),
			    ("prop_cctv_cam_03a"_J),
			    ("prop_cctv_cam_04a"_J),
			    ("prop_cctv_cam_04c"_J),
			    ("prop_cctv_cam_05a"_J),
			    ("prop_cctv_cam_06a"_J),
			    ("prop_cctv_cam_07a"_J),
			    ("prop_cs_cctv"_J),
			    ("p_cctv_s"_J),
			    ("hei_prop_bank_cctv_01"_J),
			    ("hei_prop_bank_cctv_02"_J),
			    ("ch_prop_ch_cctv_cam_02a"_J),
			    ("xm_prop_x17_server_farm_cctv_01"_J),
			};

			virtual void OnCall() override
			{
				for (auto cam_hash : CayoPericoCameraHashes)
				{
					Stand::DeleteObjectsByHash(cam_hash);
				}
			}
		};

		class Teleport : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				int index = _CayoPericoTeleportList.GetState();

				if (index >= 0 && index < 6)
				{
					const auto& tp = cayoPericoTeleportPoints[index];

					TeleportHelpers::TeleportEntityTo(TeleportHelpers::MakePlace(tp.name, tp.x, tp.y, tp.z, tp.heading));
				}
				else if (index == 6)
				{
					int otherIndex = _CayoPericoOtherTeleportList.GetState();

					if (otherIndex < 0 || otherIndex >= cayoPericoOtherTeleportPoints.size())
						return;

					const auto& tp = cayoPericoOtherTeleportPoints[otherIndex];

					TeleportHelpers::TeleportEntityTo(TeleportHelpers::MakePlace(tp.name, tp.x, tp.y, tp.z, tp.heading));
				}
			}
		};

		class OtherTeleport : public LoopedCommand
		{
			using LoopedCommand::LoopedCommand;

			virtual void OnTick() override
			{
				bool show = (_CayoPericoTeleportList.GetState() == 6);
			}
		};

		static SetMaxPayout _CayoPericoHeistSetMaxPayout{"cayopericoheistsetmaxpayout", "Auto Set Cuts", "Automatically calculates and sets player cuts"};
		static SetCuts _CayoPericoHeistSetCuts{"cayopericoheistsetcuts", "Apply Cuts", "Applies heist cut in-game"};
		static ForceReady _CayoPericoHeistForceReady{"cayopericoheistforceready", "Force Ready", "Forces all players to be ready"};
		static RequestKosatka _CayoPericoHeistRequestKosatka{"cayopericoheistrequestforkosatka", "Request Kosatka", "Requesting for Kosatka"};
		static Setup _CayoPericoHeistSetup{"cayopericoheistsetup", "Setup", "Sets up cayo perico heist"};
		static FencingFeeandPavelCut _CayoPericoHeistRemoveFencingFeeAndPavelCut{"cayopericoheistremovefencingfeeandpavelcut", "Remove Fee&Cut", "Removes fencing fee and pavel cut"};
		static SetPrimaryTargetValue _CayoPericoHeistSetPrimaryTargetValue{"cayopericoheistsetprimarytargetvalue", "Set Primary Target Value", "Updates primary target value"};
		static SetSecondaryTakeValue _CayoPericoHeistSetSecondaryTakeValue{"cayopericoheistsetsecondarytakevalue", "Set Secondary Take Value", "Updates secondary take value"};
		static SkipHacking _CayoPericoHeistSkipHacking{"cayopericoheistskiphacking", "Skip Hacking", "Skips hacking process"};
		static CutSewer _CayoPericoHeistCutSewer{"cayopericoheistcutsewer", "Cut Sewer", "Cuts the sewer"};
		static CutGlass _CayoPericoHeistCutGlass{"cayopericoheistcutglass", "Cut Glass", "Cuts the glass"};
		static RemoveCameras _CayoPericoHeistRemoveCayoPericoCameras{"cayopericoheistremovecayopericocameras", "Remove Cams", "Removes all cameras"};
		static InfinitePlasmaCutterHeat _CayoPericoHeistInfinitePlasmaCutterHeat{"cayopericoheistinfiniteplasmacutterheat", "Infinite Plasma", "Infinite Plasma Cutter Heat"};
		static TakePrimaryTarget _CayoPericoHeistTakePrimaryTarget{"cayopericoheisttakeprimarytarget", "Take Primary Target", "Takes primary target"};
		static InstantFinish _CayoPericoHeistInstantFinish{"cayopericoheistinstantfinish", "Instant Finish", "Instantly passes the heist"};
		static Teleport _CayoPericoHeistTeleportCayoPerico{"cayopericoheistteleportcayo", "Teleport", "Teleport to selected Cayo Perico location"};
		// static OtherTeleport _CayoPericoHeistTeleportCayoPericOther{"cayopericoheistotherteleport", "Other Teleport", "Teleport to other Cayo Perico location"};
	};
}