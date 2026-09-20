#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandLegacy.hpp"
#include "World/Stats.hpp"
#include "Network/Players.hpp"
#include "Scripting/Globals.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Network/GlobalPlayerBD.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	namespace ApartmentHeist
	{
		static CommandSliderLegacy _ApartmentHeistCut1{"apartmentheistcut1", "Player 1", "Player 1 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _ApartmentHeistCut2{"apartmentheistcut2", "Player 2", "Player 2 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _ApartmentHeistCut3{"apartmentheistcut3", "Player 3", "Player 3 cut", std::nullopt, std::nullopt, 0};
		static CommandSliderLegacy _ApartmentHeistCut4{"apartmentheistcut4", "Player 4", "Player 4 cut", std::nullopt, std::nullopt, 0};

		class SetCuts : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				auto base1 = Globals::HEIST_PRISON_BREAK.at(1);
				auto base2 = Globals::HEIST_HUMANE_LABS.at(3008);

				*base1.at(0, 1).as<int*>() = 100 - (_ApartmentHeistCut1.GetState() + _ApartmentHeistCut2.GetState() + _ApartmentHeistCut3.GetState() + _ApartmentHeistCut4.GetState());
				*base1.at(1, 1).as<int*>() = _ApartmentHeistCut2.GetState();
				*base1.at(2, 1).as<int*>() = _ApartmentHeistCut3.GetState();
				*base1.at(3, 1).as<int*>() = _ApartmentHeistCut4.GetState();

				Script::current()->yield(500);

				*base2.at(0, 1).as<int*>() = -1 * (*base1.at(0, 1).as<int*>() + *base1.at(1, 1).as<int*>() + *base1.at(2, 1).as<int*>() + *base1.at(3, 1).as<int*>() - 100);
				
				for (int i = 1; i <= 3; i++)
				{
					*base2.at(i, 1).as<int*>() = *base1.at(i, 1).as<int*>();
				}
			}
		};

		class ForceReady : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto gpbd = GlobalPlayerBD::Get(); gpbd && Scripts::SafeToModifyFreemodeBroadcastGlobals())
				{
					for (auto& player : Players::GetPlayers())
					{
						gpbd->Entries[player.second.GetId()].HeistCutSelectionStage = 6;
					}
				}
			}
		};

		class Setup : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				Stats::SetInt("MPX_HEIST_PLANNING_STAGE", -1);
			}
		};

		class SkipHacking : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					*ScriptLocal(thread, 12239).at(24).as<int*>() = 7;
					*ScriptLocal(thread, 10233).as<int*>() = *ScriptLocal(thread, 10233).as<int*>() | (1 << 9);
				}
			}
		};

		class SkipDrilling : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					*ScriptLocal(thread, 10527).at(11).as<float*>() = 100.0f;
				}
			}
		};

		class SkipSwiping : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					*ScriptLocal(thread, 32785).at(Self::GetPlayer().GetId(), 294).at(143).as<int*>() = 8;
					*ScriptLocal(thread, 64655).as<int*>() = 5;
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

					*ScriptLocal(thread, 20412).at(1725).at(0, 1).as<int*>() = 80;
					*ScriptLocal(thread, 20412).as<int*>() = 12;
					*ScriptLocal(thread, 29326).at(0, 1).as<int*>() = 99999;
					*ScriptLocal(thread, 32785).at(0, 294).at(68).as<int*>() = 99999;
				}
				// TODO: find a way of getting current heist info so that InstantFinishPacific can be implemented here conditionally.
			}
		};

		class InstantFinishPacific : public CommandLegacy
		{
			using CommandLegacy::CommandLegacy;

			virtual void OnCall() override
			{
				if (auto thread = Scripts::FindScriptThread("fm_mission_controller"_J))
				{
					Scripts::ForceScriptHost(thread);
					Script::current()->yield(500);

					*ScriptLocal(thread, 20412).at(2686).as<int*>() = 1875000;
					*ScriptLocal(thread, 20412).at(1062).as<int*>() = 5;
					*ScriptLocal(thread, 20412).as<int*>() = 12;
					*ScriptLocal(thread, 29326).at(0, 1).as<int*>() = 99999;
					*ScriptLocal(thread, 32785).at(0, 294).at(68).as<int*>() = 99999;
				}
			}
		};

		static SetCuts _ApartmentHeistSetCuts{"apartmentheistsetcuts", "Set Cuts", "Sets heist cut"};
		static ForceReady _ApartmentHeistForceReady{"apartmentheistforceready", "Force Ready", "Forces all players to be ready"};
		static Setup _ApartmentHeistSetup{"apartmentheistsetup", "Setup", "Sets up current apartment heist"};
		static SkipHacking _ApartmentHeistSkipHacking{"apartmentheistskiphacking", "Skip Hacking", "Skips hacking process"};
		static SkipDrilling _ApartmentHeistSkipDrilling{"apartmentheistskipdrilling", "Skip Drilling", "Skips drilling process"};
		static SkipSwiping _ApartmentHeistSkipSwiping{"apartmentheistskipswiping", "Skip Swiping", "Skips card swiping process"};
		static InstantFinish _ApartmentHeistInstantFinish{"apartmentheistinstantfinish", "Instant Finish", "Instantly passes the heist"};
		static InstantFinishPacific _ApartmentHeistInstantFinishPacific{"apartmentheistinstantfinishpacific", "Instant Finish (Pacific)", "Instantly passes Pacific Standard Job"};
	}
}