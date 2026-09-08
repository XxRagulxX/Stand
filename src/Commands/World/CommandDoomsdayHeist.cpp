#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "World/Stats.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Network/GPBD_FM_2.hpp"
#include "World/TeleportUtils.hpp"
#include "Rendering/Notifications.hpp"

namespace Stand::Features
{
	namespace DoomsdayHeist
	{
		struct TeleportLocation
		{
			float x;
			float y;
			float z;
			float heading;
			const char* name;
		};
		static CommandSliderLegacy _DoomsdayHeistCut1{"doomsdayheistcut1", "Player 1", "Player 1 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DoomsdayHeistCut2{"doomsdayheistcut2", "Player 2", "Player 2 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DoomsdayHeistCut3{"doomsdayheistcut3", "Player 3", "Player 3 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _DoomsdayHeistCut4{"doomsdayheistcut4", "Player 4", "Player 4 cut", std::nullopt, std::nullopt, 0};

		class SetCuts : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base = ScriptGlobal(1969071).At(812).At(50);

				*base.At(0, 1).As<int*>() = _DoomsdayHeistCut1.GetState();
				*base.At(1, 1).As<int*>() = _DoomsdayHeistCut2.GetState();
				*base.At(2, 1).As<int*>() = _DoomsdayHeistCut3.GetState();
				*base.At(3, 1).As<int*>() = _DoomsdayHeistCut4.GetState();
			}
		};
		static std::vector<TeleportLocation> DoomsDayHeistTeleportPoints = {
		    {515.528f, 4835.353f, -62.587f, 0.0f, "Heist Board"},
		    {512.888f, 4833.033f, -68.989f, 0.0f, "Prisoner Cell"}};

		static std::vector<std::pair<int, const char*>> DoomsDayHeistTeleportList = {
		    {0, "Heist Board"},
		    {1, "Prisoner Cell"}};

		static CommandListSelect _DoomsDayHeistTeleportList{"doomsdayheistteleportlist", "TP", "Teleport Location", DoomsDayHeistTeleportList, 0};

		static std::vector<std::pair<int, const char*>> DoomsDayHeistPlayers = {
		    {1, "1 Player"},
		    {2, "2 Players"},
		    {3, "3 Players"},
		    {4, "4 Players"}};

		static CommandListSelect _DoomsDayHeistPlayers{"doomsdayheistplayers", "Players", "How many players are in the heist", DoomsDayHeistPlayers, 1};


		class ForceReady : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto gpbd_fm_2 = GPBD_FM_2::Get())
				{
					gpbd_fm_2->Entries[0].GangopsData.ArePlayersReady[0] = TRUE;
					gpbd_fm_2->Entries[1].GangopsData.ArePlayersReady[1] = TRUE;
					gpbd_fm_2->Entries[2].GangopsData.ArePlayersReady[2] = TRUE;
					gpbd_fm_2->Entries[3].GangopsData.ArePlayersReady[3] = TRUE;
				}
			}
		};

		static std::vector<std::pair<int, const char*>> doomsdayHeistCategory = {
		    {0, "The Data Breaches"},
		    {1, "The Bogdan Problem"},
		    {2, "The Doomsday Senario"}};
		static CommandListSelect _DoomsdayHeistCategory{"doomsdayheistcategory", "Select Heist", "Heist categories", doomsdayHeistCategory, 0};

		class Setup : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				switch (_DoomsdayHeistCategory.GetState())
				{
				case 0:
					Stats::SetInt("MPX_GANGOPS_FLOW_MISSION_PROG", 503);
					Stats::SetInt("MPX_GANGOPS_HEIST_STATUS", -229383);
					Stats::SetInt("MPX_GANGOPS_FLOW_NOTIFICATIONS", 1557);
					break;
				case 1:
					Stats::SetInt("MPX_GANGOPS_FLOW_MISSION_PROG", 240);
					Stats::SetInt("MPX_GANGOPS_HEIST_STATUS", -229378);
					Stats::SetInt("MPX_GANGOPS_FLOW_NOTIFICATIONS", 1557);
					break;
				case 2:
					Stats::SetInt("MPX_GANGOPS_FLOW_MISSION_PROG", 16368);
					Stats::SetInt("MPX_GANGOPS_HEIST_STATUS", -229380);
					Stats::SetInt("MPX_GANGOPS_FLOW_NOTIFICATIONS", 1557);
					break;
				default: break;
				}

				Script::current()->yield(500);

				if (auto thread = Scripts::FindScriptThread("gb_gang_ops_planning"_J))
					*ScriptLocal(thread, 218).As<int*>() = 6;
			    
				Notifications::ShowInGame("Doomsday Heist", "Doomsday Heist Setup - Completed", "CHAR_LESTER", "Green");
			}
		};

		class SetMaxPayout : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

		public:
			virtual void OnCall() override
			{
				int category = _DoomsdayHeistCategory.GetState();
				int players = _DoomsDayHeistPlayers.GetState();

				if (players < 1 || players > 4)
					players = 1;

				int difficulty = *ScriptGlobal(4718592).At(3538).As<int*>();
				if (difficulty != 2)
					difficulty = 1;

				int cut = CalculateCut(category, difficulty);

				if (cut <= 0)
				{
					Notifications::ShowInGame("Doomsday Heist", "Invalid heist state. Setup board first.", "CHAR_LESTER", "Red");
					return;
				}

				ApplyCuts(cut, players);

				Notifications::ShowInGame( "Doomsday Heist", "Max Payout Set - Successful", "CHAR_LESTER", "Green");
			}

		private:
			int CalculateCut(int category, int difficulty)
			{
				static std::array<std::pair<int, int>, 3> payouts = {
				    std::make_pair(975000, 1218750),  // 0 = Data Breaches
				    std::make_pair(1425000, 1771250), // 1 = Bogdan Problem
				    std::make_pair(1800000, 2250000)  // 2 = Doomsday Scenario
				};

				if (category < 0 || category > 2)
					return 0;

				float payout = (difficulty == 2) ? payouts[category].second : payouts[category].first;

				constexpr float maxPayout = 2'550'000.0f;

				int cut = static_cast<int>(maxPayout / (payout / 100.0f));

				return std::clamp(cut, 100, 500);
			}

			void ApplyCuts(int totalCut, int players)
			{
				std::array<CommandSliderLegacy*, 4> cmds = {
				    &_DoomsdayHeistCut1,
				    &_DoomsdayHeistCut2,
				    &_DoomsdayHeistCut3,
				    &_DoomsdayHeistCut4};

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
					*ScriptLocal(thread, 1588).As<int*>() = 2;
					*ScriptLocal(thread, 1557).As<int*>() = 3;
					*ScriptLocal(thread, 1314).At(135).As<int*>() = 3;
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

					*ScriptLocal(thread, 20412).At(1725).At(0, 1).As<int*>() = 80;
					*ScriptLocal(thread, 20412).As<int*>() = 12;
					*ScriptLocal(thread, 29326).At(0, 1).As<int*>() = 99999;
					*ScriptLocal(thread, 32785).At(0, 294).At(68).As<int*>() = 99999;
				}
				// TODO: find a way of getting current heist info so that InstantFinishAct3 can be implemented here conditionally.
			}
		};

		class InstantFinishAct3 : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					Scripts::ForceScriptHost(thread);
					Script::current()->yield(500);

					*ScriptLocal(thread, 20412).As<int*>() = 12;
					*ScriptLocal(thread, 20412).At(1740).At(0, 1).As<int*>() = 150;
					*ScriptLocal(thread, 20412).At(1062).As<int*>() = 5;
					*ScriptLocal(thread, 29326).At(0, 1).As<int*>() = 99999;
					*ScriptLocal(thread, 32785).At(0, 294).At(68).As<int*>() = 99999;
				}
			}
		};

		class Teleport : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				int index = _DoomsDayHeistTeleportList.GetState();

				if (index >= 0 && index < 6)
				{
					const auto& tp = DoomsDayHeistTeleportPoints[index];

					TeleportHelpers::TeleportEntityTo(TeleportHelpers::MakePlace(tp.name, tp.x, tp.y, tp.z, tp.heading));
				}
			}
		};


		static SetCuts _DoomsdayHeistSetCuts{"doomsdayheistsetcuts", "Set Cuts", "Sets heist cut"};
		static ForceReady _DoomsdayHeistForceReady{"doomsdayheistforceready", "Force Ready", "Forces all players to be ready"};
		static Setup _DoomsdayHeistSetup{"doomsdayheistsetup", "Setup", "Sets up doomsday heist"};
		static SetMaxPayout _DoomsdayHeistSetMaxPayout{"doomsdayheistsetmaxpayout", "Auto Set Cuts", "Automatically calculates and sets player cuts"};
		static SkipHacking _DoomsdayHeistSkipHacking{"doomsdayheistskiphacking", "Skip Hacking", "Skips hacking process"};
		static InstantFinish _DoomsdayHeistInstantFinish{"doomsdayheistinstantfinish", "Instant Finish", "Instantly passes the heist"};
		static InstantFinishAct3 _DoomsdayHeistInstantFinishAct3{"doomsdayheistinstantfinishact3", "Instant Finish (Act: III)", "Instantly passes Doomsday Scenario Act: III"};
		static Teleport _DoomsdayHeistTeleport{"doomsdayheistteleport", "Teleport", "Teleport to selected dooms day location"};
	}
}