#include "Core/Hooking.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <MinHook.h>
#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Pointers.hpp"

namespace Stand
{
	Hooking::Hooking()
	{
		// Window hooks

		auto* hook = AddHook<Hooks::Window::WndProc>("WndProc", Pointers.WndProc, reinterpret_cast<void*>(Hooks::Window::WndProc), HookGroup::Minimal);

		// Raw input hooks

		auto* rawInputHook = AddHook<Hooks::RawInput::GetRawInputData>("GetRawInputData", reinterpret_cast<void*>(GetProcAddress(LoadLibraryA("user32.dll"), "GetRawInputData")), reinterpret_cast<void*>(Hooks::RawInput::GetRawInputData), HookGroup::Minimal);

		// DXGI hooks

		auto swapchain_vft = *reinterpret_cast<void***>(*Pointers.SwapChain);

		auto* presentHook = AddHook<Hooks::SwapChain::Present>("Present", swapchain_vft[Hooks::SwapChain::VMTPresentIdx], reinterpret_cast<void*>(Hooks::SwapChain::Present), HookGroup::Minimal);

		auto* resizeBuffersHook = AddHook<Hooks::SwapChain::ResizeBuffers>("ResizeBuffers", swapchain_vft[Hooks::SwapChain::VMTResizeBuffersIdx], reinterpret_cast<void*>(Hooks::SwapChain::ResizeBuffers), HookGroup::Minimal);

		// Anticheat hooks

		auto* gameSkeletonUpdateHook = AddHook<Hooks::Anticheat::GameSkeletonUpdate>("GameSkeletonUpdate", Pointers.GameSkeletonUpdate, reinterpret_cast<void*>(Hooks::Anticheat::GameSkeletonUpdate));

		auto* prepareMetricForSendingHook = AddHook<Hooks::Anticheat::PrepareMetricForSending>("PrepareMetricForSending", Pointers.PrepareMetricForSending, reinterpret_cast<void*>(Hooks::Anticheat::PrepareMetricForSending));

		auto* getThreadContextHook = AddHook<Hooks::Anticheat::GetThreadContext>("GetThreadContext", reinterpret_cast<void*>(GetProcAddress(LoadLibraryA("kernel32.dll"), "GetThreadContext")), reinterpret_cast<void*>(Hooks::Anticheat::GetThreadContext));

		auto* httpStartRequestHook = AddHook<Hooks::Anticheat::HttpStartRequest>("HttpStartRequest", Pointers.HttpStartRequest, reinterpret_cast<void*>(Hooks::Anticheat::HttpStartRequest));

		auto* battlEyeServerProcessPlayerJoinHook = AddHook<Hooks::Anticheat::BattlEyeServerProcessPlayerJoin>("BattlEyeServerProcessPlayerJoin", Pointers.BattlEyeServerProcessPlayerJoin, reinterpret_cast<void*>(Hooks::Anticheat::BattlEyeServerProcessPlayerJoin));

		auto* getAnticheatInitializedHashHook = AddHook<Hooks::Anticheat::GetAnticheatInitializedHash>("GetAnticheatInitializedHash", Pointers.GetAnticheatInitializedHash, reinterpret_cast<void*>(Hooks::Anticheat::GetAnticheatInitializedHash));

		auto* getAnticheatInitializedHash2Hook = AddHook<Hooks::Anticheat::GetAnticheatInitializedHash2>("GetAnticheatInitializedHash2", Pointers.GetAnticheatInitializedHash2, reinterpret_cast<void*>(Hooks::Anticheat::GetAnticheatInitializedHash2));

		// Script hooks

		auto* runScriptThreadsHook = AddHook<Hooks::Script::RunScriptThreads>("RunScriptThreads", Pointers.RunScriptThreads, reinterpret_cast<void*>(Hooks::Script::RunScriptThreads));

		auto* initNativeTablesHook = AddHook<Hooks::Script::InitNativeTables>("InitNativeTables", Pointers.InitNativeTables, reinterpret_cast<void*>(Hooks::Script::InitNativeTables));

		auto* scriptVMHook = AddHook<Hooks::Script::ScriptVM>("ScriptVM", reinterpret_cast<void*>(Pointers.ScriptVM), reinterpret_cast<void*>(Hooks::Script::ScriptVM));

		auto* assignPhysicalIndexHook = AddHook<Hooks::Info::AssignPhysicalIndex>("AssignPhysicalIndex", Pointers.AssignPhysicalIndex, reinterpret_cast<void*>(Hooks::Info::AssignPhysicalIndex));

		auto* networkPlayerMgrShutdownHook = AddHook<Hooks::Info::NetworkPlayerMgrShutdown>("NetworkPlayerMgrShutdown", Pointers.NetworkPlayerMgrShutdown, reinterpret_cast<void*>(Hooks::Info::NetworkPlayerMgrShutdown));

		// Spoofing hooks

		auto* isNodeInScopeHook = AddHook<Hooks::Spoofing::IsNodeInScope>("IsNodeInScope", Pointers.IsNodeInScope, reinterpret_cast<void*>(Hooks::Spoofing::IsNodeInScope));

		auto* shouldUseNodeCacheHook = AddHook<Hooks::Spoofing::ShouldUseNodeCache>("ShouldUseNodeCache", Pointers.ShouldUseNodeCache, reinterpret_cast<void*>(Hooks::Spoofing::ShouldUseNodeCache));

		auto* writeNetArrayDataHook = AddHook<Hooks::Spoofing::WriteNetArrayData>("WriteNetArrayData", Pointers.WriteNetArrayData, reinterpret_cast<void*>(Hooks::Spoofing::WriteNetArrayData));

		auto* writeNodeDataHook = AddHook<Hooks::Spoofing::WriteNodeData>("WriteNodeData", Pointers.WriteNodeData, reinterpret_cast<void*>(Hooks::Spoofing::WriteNodeData));

		auto* writeSyncTreeHook = AddHook<Hooks::Spoofing::WriteSyncTree>("WriteSyncTree", Pointers.WriteSyncTree, reinterpret_cast<void*>(Hooks::Spoofing::WriteSyncTree));

		// Network hooks

		auto* receiveNetMessageHook = AddHook<Hooks::Network::ReceiveNetMessage>("ReceiveNetMessage", Pointers.ReceiveNetMessage, reinterpret_cast<void*>(Hooks::Network::ReceiveNetMessage));

		auto* getPoolTypeHook = AddHook<Hooks::Network::GetPoolType>("GetPoolType", Pointers.GetPoolType, reinterpret_cast<void*>(Hooks::Network::GetPoolType));

		auto* getDLCHashHook = AddHook<Hooks::Network::GetDLCHash>("GetDLCHash", Pointers.GetDLCHash, reinterpret_cast<void*>(Hooks::Network::GetDLCHash));

		// Matchmaking hooks

		auto* matchmakingAdvertiseHook = AddHook<Hooks::Matchmaking::MatchmakingAdvertise>("MatchmakingAdvertise", Pointers.MatchmakingAdvertise, reinterpret_cast<void*>(Hooks::Matchmaking::MatchmakingAdvertise));

		auto* matchmakingSessionDetailSendResponseHook = AddHook<Hooks::Matchmaking::MatchmakingSessionDetailSendResponse>("MatchmakingSessionDetailSendResponse", Pointers.MatchmakingSessionDetailSendResponse, reinterpret_cast<void*>(Hooks::Matchmaking::MatchmakingSessionDetailSendResponse));


		auto* matchmakingUnadvertiseHook = AddHook<Hooks::Matchmaking::MatchmakingUnadvertise>("MatchmakingUnadvertise", Pointers.MatchmakingUnadvertise, reinterpret_cast<void*>(Hooks::Matchmaking::MatchmakingUnadvertise));

		auto* matchmakingUpdateHook = AddHook<Hooks::Matchmaking::MatchmakingUpdate>("MatchmakingUpdate", Pointers.MatchmakingUpdate, reinterpret_cast<void*>(Hooks::Matchmaking::MatchmakingUpdate));

		// Misc hooks

		auto* assistedAimShouldReleaseEntityHook = AddHook<Hooks::Misc::AssistedAimShouldReleaseEntity>("AssistedAimShouldReleaseEntity", Pointers.AssistedAimShouldReleaseEntity, reinterpret_cast<void*>(Hooks::Misc::AssistedAimShouldReleaseEntity));

	}

	Hooking::~Hooking()
	{
		DestroyImpl();

		MH_Uninitialize();
	}

	bool Hooking::Init()
	{
		return GetInstance().InitImpl();
	}

	void Hooking::Destroy()
	{
		GetInstance().DestroyImpl();
		BytePatches::RestoreAll();
	}

	bool Hooking::InitImpl()
	{
		const auto status = MH_Initialize();

		if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
		{
			return false;
		}

		LOGF(INFO, "[Hooking] Starting hook initialization.");
		auto minimalHooks = GetMinimalHooks();
		auto passiveHooks = GetPassiveHooks();
		auto mainHooks = GetMainHooks();

		// Create hooks in stages.
		BatchCreate(minimalHooks);
		BatchCreate(passiveHooks);
		BatchCreate(mainHooks);

		// Enable hooks in stages.
		BatchEnable(minimalHooks);
		BatchEnable(passiveHooks);
		BatchEnable(mainHooks);

		for (auto* hook : minimalHooks)
		{
			if (hook)
			{
				LOGF(INFO, "Minimal hook '{}' enabled = {}", hook->Name(), hook->IsEnabled());
			}
		}

		return true;
	}

	void Hooking::DestroyImpl()
	{
		auto hooks = GetAllHooks();

		BatchDisable(hooks);
		BatchRemove(hooks);

		m_Hooks.clear();
	}

	void Hooking::BatchCreate(const std::vector<DetourHook*>& hooks)
	{
		for (auto* hook : hooks)
		{
			if (!hook)
				continue;

			if (!hook->CreateHook())
			{
				throw std::runtime_error(
				    "Failed to create hook: " + std::string(hook->Name()));
			}
		}
	}

	void Hooking::BatchEnable(const std::vector<DetourHook*>& hooks)
	{
		for (auto* hook : hooks)
		{
			if (!hook)
				continue;

			if (!hook->EnableHookQueued())
			{
				throw std::runtime_error(
				    "Failed to enable hook: " + std::string(hook->Name()));
			}
		}

		if (MH_ApplyQueued() != MH_OK)
		{
			throw std::runtime_error(
			    "Failed to apply queued hook enables.");
		}
	}

	void Hooking::BatchDisable(const std::vector<DetourHook*>& hooks)
	{
		for (auto* hook : hooks)
		{
			if (!hook)
				continue;

			if (!hook->DisableHookQueued())
			{
				throw std::runtime_error(
				    "Failed to disable hook: " + std::string(hook->Name()));
			}
		}

		if (MH_ApplyQueued() != MH_OK)
		{
			throw std::runtime_error(
			    "Failed to apply queued hook enables.");
		}
	}

	void Hooking::BatchRemove(const std::vector<DetourHook*>& hooks)
	{
		for (auto* hook : hooks)
		{
			if (!hook)
				continue;

			hook->RemoveHook();
		}
	}

		std::vector<DetourHook*> Hooking::GetMinimalHooks()
	{
		std::vector<DetourHook*> hooks;

		for (auto& hook : m_Hooks)
		{
			if (hook && hook->GetGroup() == HookGroup::Minimal)
				hooks.push_back(hook.get());
		}

		return hooks;
	}

	std::vector<DetourHook*> Hooking::GetPassiveHooks()
	{
		std::vector<DetourHook*> hooks;

		for (auto& hook : m_Hooks)
		{
			if (hook && hook->GetGroup() == HookGroup::Passive)
				hooks.push_back(hook.get());
		}

		return hooks;
	}

	std::vector<DetourHook*> Hooking::GetMainHooks()
	{
		std::vector<DetourHook*> hooks;

		for (auto& hook : m_Hooks)
		{
			if (hook && hook->GetGroup() == HookGroup::Main)
				hooks.push_back(hook.get());
		}

		return hooks;
	}

	std::vector<DetourHook*> Hooking::GetAllHooks()
	{
		std::vector<DetourHook*> hooks;
		hooks.reserve(m_Hooks.size());

		for (auto& hook : m_Hooks)
		{
			if (hook)
				hooks.push_back(hook.get());
		}

		return hooks;
	}

	template<auto HookFunc>
	DetourHook* Hooking::AddHook(
	    std::string_view name,
	    void* target,
	    void* detour,
	    HookGroup group)
	{
		auto hook = std::make_unique<DetourHook>(
		    name,
		    target,
		    detour);

		hook->SetGroup(group);

		auto* ptr = hook.get();

		m_Hooks.emplace_back(std::move(hook));

		RegisterHook<HookFunc>(ptr);

		return ptr;
	}

	DetourHook* Hooking::FindHook(std::string_view name)
	{
		for (auto& hook : m_Hooks)
		{
			if (hook && hook->Name() == name)
				return hook.get();
		}

		return nullptr;
	}

	DetourHook* Hooking::GetHook(std::string_view name)
	{
		return GetInstance().FindHook(name);
	}
}