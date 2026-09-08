#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Core/Pointers.hpp"
#include "Core/tlsContext.hpp"
#include "Scripting/GtaThread.hpp"
#include "Scripting/scrProgram.hpp"
#include "Scripting/CGameScriptHandler.hpp"
#include "Scripting/CGameScriptHandlerNetComponent.hpp"
#include "Network/GlobalPlayerBD.hpp"
#include "Network/GPBD_FM_3.hpp"
#include "Network/LauncherServerData.hpp"
#include "Network/LauncherClientData.hpp"
#include "Scripting/Natives.hpp"
#include "Network/Packet.hpp"
#include "Network/Players.hpp"
#include "World/Self.hpp"
#include "Core/Pattern.hpp"
#include "Network/LauncherScripts.hpp"
#include "Network/ScriptEvent.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand::Scripts
{
	int StartScript(joaat_t hash, eStackSizes stackSize, void* args, int argCount)
	{
		if (!SCRIPT::DOES_SCRIPT_WITH_NAME_HASH_EXIST(hash))
			return 0;

		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(hash) > 0)
			return FindScriptThread(hash)->m_Context.m_ThreadId;

		for (int i = 0; !SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(hash); i++)
		{
			SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(hash);
			Script::current()->yield();

			if (i > 30)
				return 0;
		}

		int id = 0;
		if (args && argCount > 0)
		{
			id = BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH_AND_ARGS(hash, args, argCount, static_cast<int>(stackSize));
		}
		else
		{
			id = BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH(hash, static_cast<int>(stackSize));
		}

		SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(hash);
		return id;
	}

	bool IsScriptActive(joaat_t hash)
	{
		if (auto program = FindScriptProgram(hash))
			return (program->m_RefCount - 1) > 0;

		return false;
	}

	rage::scrThread* FindScriptThread(joaat_t hash)
	{
		for (auto& thread : *Pointers.ScriptThreads)
		{
			if (thread && thread->m_Context.m_ThreadId && thread->m_ScriptHash == hash)
			{
				return thread;
			}
		}

		return nullptr;
	}

	rage::scrThread* FindScriptThreadByID(std::uint32_t id)
	{
		for (auto& thread : *Pointers.ScriptThreads)
		{
			if (thread && thread->m_Context.m_ThreadId == id)
			{
				return thread;
			}
		}

		return nullptr;
	}

	rage::scrProgram* FindScriptProgram(joaat_t hash)
	{
		for (int i = 0; i < 176; i++)
		{
			if (Pointers.ScriptPrograms[i] && Pointers.ScriptPrograms[i]->m_NameHash == hash)
				return Pointers.ScriptPrograms[i];
		}

		return nullptr;
	}

	void RunAsScript(rage::scrThread* thread, std::function<void()> callback)
	{
		auto og_thread = rage::tlsContext::Get()->m_CurrentScriptThread;
		auto og_running_in_scrthread = rage::tlsContext::Get()->m_ScriptThreadActive;
		rage::tlsContext::Get()->m_CurrentScriptThread = thread;
		rage::tlsContext::Get()->m_ScriptThreadActive = true;
		callback();
		rage::tlsContext::Get()->m_ScriptThreadActive = og_running_in_scrthread;
		rage::tlsContext::Get()->m_CurrentScriptThread = og_thread;
	}

	void RunWithSpoofedThreadName(std::uint32_t name, std::function<void()> callback)
	{
		if (auto thread = reinterpret_cast<GtaThread*>(rage::tlsContext::Get()->m_CurrentScriptThread))
		{
			auto hash_1 = thread->m_Context.m_ScriptHash;
			auto hash_2 = thread->m_ScriptHash;
			auto hash_3 = thread->m_ScriptHash2;
			std::optional<std::uint32_t> hash_4;
			if (auto handler = thread->m_ScriptHandler)
				hash_4 = handler->GetId()->m_Hash;

			thread->m_Context.m_ScriptHash = name;
			thread->m_ScriptHash = name;
			thread->m_ScriptHash2 = name;
			if (hash_4)
				thread->m_ScriptHandler->GetId()->m_Hash = name;

			callback();

			thread->m_Context.m_ScriptHash = hash_1;
			thread->m_ScriptHash = hash_2;
			thread->m_ScriptHash2 = hash_3;
			if (hash_4)
				thread->m_ScriptHandler->GetId()->m_Hash = *hash_4;
		}
	}

	// we need this because modifying player broadcast data before they are registered causes the default values (when a new player joins) to be changed as well
	bool SafeToModifyFreemodeBroadcastGlobals()
	{
		if (!*Pointers.IsSessionStarted)
			return false;

		if (!FindScriptThread("freemode"_J))
			return false;

		if (auto gpbd = GlobalPlayerBD::Get())
		{
			return gpbd->Entries[Self::GetPlayer().GetId()].FreemodeState == eFreemodeState::RUNNING;
		}

		return false;
	}

	std::optional<std::uint32_t> GetCodeLocationByPattern(
	    rage::scrProgram* program,
	    SimplePattern pattern)
	{
		if (!program || pattern.m_Bytes.empty())
			return std::nullopt;

		const auto codeSize =
		    static_cast<std::size_t>(program->m_CodeSize);

		const auto patternSize = pattern.m_Bytes.size();

		if (patternSize > codeSize)
			return std::nullopt;

		for (std::size_t i = 0;
		    i + patternSize <= codeSize;
		    ++i)
		{
			bool found = true;

			for (std::size_t j = 0;
			    j < patternSize;
			    ++j)
			{
				if (pattern.m_Bytes[j].has_value()
				    && pattern.m_Bytes[j].value()
				        != *program->GetCodeAddress(
				            static_cast<std::uint32_t>(i + j)))
				{
					found = false;
					break;
				}
			}

			if (found)
				return static_cast<std::uint32_t>(i);
		}

		return std::nullopt;
	}

	void ForceScriptHost(rage::scrThread* thread)
	{
		if (auto net_component = reinterpret_cast<GtaThread*>(thread)->m_NetComponent)
		{
			if (net_component->IsLocalPlayerHost())
				return;

			net_component->DoHostMigration(Self::GetPlayer().GetHandle(), net_component->m_HostToken + 1, false);

			Packet pkt;
			pkt.WriteMessageHeader(rage::netMessage::Type::ScriptVerifyHostAck);
			net_component->m_ScriptHandler->GetId()->Serialize(&pkt.GetBuffer());
			pkt.GetBuffer().Write<bool>(true, 1);
			pkt.GetBuffer().Write<bool>(true, 1);
			pkt.GetBuffer().Write<std::uint16_t>(net_component->m_HostToken, 16);

			for (auto& player : Players::GetPlayers())
				if (!player.second.IsLocal())
					pkt.Send(player.second.GetMessageId());
		}
	}

	std::optional<int> GetLauncherIndexFromScript(joaat_t hash)
	{
		for (int i = 0; i < g_LauncherScripts.size(); i++)
			if (g_LauncherScripts[i] == hash)
				return i;

		return std::nullopt;
	}

	void StartLauncherScript(joaat_t hash)
	{
		auto index = GetLauncherIndexFromScript(hash);

		if (!index)
			return;

		static auto atLeastOnePlayerInState = [](GtaThread* launcher, eLauncherState state) -> bool {
			bool set = false;

			if (!launcher->m_NetComponent)
				return false;

			for (auto& [_, plyr] : Players::GetPlayers())
			{
				if (launcher->m_NetComponent->IsParticipant(plyr.GetId()))
				{
					if (LauncherClientData::Get(launcher)->Entries[plyr.GetId()].LauncherState == state)
					{
						set = true;
						break;
					}
				}
			}

			return set;
		};

		// 1) Get launcher
		if (auto launcher = reinterpret_cast<GtaThread*>(FindScriptThread("am_launcher"_J)); launcher && launcher->m_NetComponent)
		{
			// 2) Force host of launcher
			if (!launcher->m_NetComponent->IsLocalPlayerHost())
			{
				ForceScriptHost(launcher);
				Script::current()->yield(400);
			}

			launcher->m_Context.m_State = rage::scrThread::State::PAUSED;

			auto serverData = LauncherServerData::Get();

			// 3) Remove players from that annoying waiting stage
			if (atLeastOnePlayerInState(launcher, eLauncherState::WAIT_FOR_ACK))
			{
				for (int i = 0; atLeastOnePlayerInState(launcher, eLauncherState::WAIT_FOR_ACK); i++)
				{
					if (i > 200)
						break; // 3F) Timeout

					serverData->CurrentScript.LauncherIndex = 0;
					serverData->LauncherState = eLauncherState::START_SCRIPT;
					Script::current()->yield(10);
				}
			} // State should now be 6 or 0

			// 4) Check if a script is already being executed, and unstuck from that state if so
			if (atLeastOnePlayerInState(launcher, eLauncherState::START_SCRIPT))
			{
				for (int i = 0; atLeastOnePlayerInState(launcher, eLauncherState::START_SCRIPT); i++)
				{
					if (i > 200)
						break; // 4F) Timeout

					serverData->CurrentScript.LauncherIndex = 0;
					serverData->LauncherState = eLauncherState::WAIT_FOR_TERMINATION;
					Script::current()->yield(10);
				}
			} // State should now be 7 or 0

			// 5) Get everyone out of state 7
			if (atLeastOnePlayerInState(launcher, eLauncherState::WAIT_FOR_TERMINATION))
			{
				for (int i = 0; atLeastOnePlayerInState(launcher, eLauncherState::WAIT_FOR_TERMINATION); i++)
				{
					if (i > 200)
						break; // 5F) Timeout

					serverData->LauncherState = eLauncherState::EMPTY;
					Script::current()->yield(10);
				}
			} // State should now be 0

			// 6) Actually get the script to start
			serverData->Flags.Set(eLauncherFlags::RUN_IMMEDIATELY);
			serverData->CurrentScript.EventIndex = 0;
			serverData->CurrentScript.LauncherIndex = *index;
			serverData->CurrentScript.Terminated = false;
			serverData->LauncherState = eLauncherState::START_SCRIPT;
			LauncherClientData::Get(launcher)->Entries[Self::GetPlayer().GetId()].LauncherState = eLauncherState::START_SCRIPT;

			launcher->m_Context.m_State = rage::scrThread::State::RUNNING;
		}
	}

	void ForceScriptOnPlayer(joaat_t hash, int bits)
	{
		auto index = GetLauncherIndexFromScript(hash);

		if (!index)
			return;

		int random_int = time(0) ^ -*index;

		MP_SCRIPT_DATA data{};
		data.MissionIndex = *index;
		strcpy(data.CloudFileName.Data, "0");
		data.InstanceId = -1;
		data.UniqueId = random_int;
		data.GenericInt = 0;

		SCRIPT_EVENT_FORCE_PLAYER_ON_MISSION event;
		event.ScriptData = data;
		event.Flags = 0;
		event.ReplayProtectionValue = GPBD_FM_3::Get()->Entries[Self::GetPlayer().GetId()].ScriptEventReplayProtectionCounter;
		event.PAD_0026 = random_int ^ 917391583 ^ rand();

		// we need to REALLY make sure we're getting an unique random number since the script checks it against the last five events sent
		auto& rand_int_2 = event.GetEventIndex();
		rand_int_2 ^= (random_int ^ -2024791401); // TODO: this is a pretty ghetto way to create random numbers
		rand_int_2 ^= (random_int ^ 917391583);

		auto bypass_global = ScriptGlobal(1991386); // this global will bypass the new checks added this update
		auto old_val = *bypass_global.As<int*>();
		*bypass_global.As<int*>() = rand_int_2 ^ rand() ^ time(0);

		event.SetPlayerBits(bits);
		event.Send();

		for (int i = 0; i < 2; i++)
		{
			SCRIPT_EVENT_FORCE_PLAYER_ON_MISSION_SERVER_ACK ack; // not strictly necessary but still useful
			ack.ScriptData = data;
			ack.SetPlayerBits(bits);
			ack.Send();

			Script::current()->yield(20);
		}

		// restore global just in case
		*bypass_global.As<int*>() = old_val;
	}
}