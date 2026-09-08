#include "Core/Pointers.hpp"

#include "Core/ModuleMgr.hpp"
#include "Util/Joaat.hpp"
#include "Game/atArray.hpp"
#include "Core/PatternBatch.hpp"
#include "Core/PatternCache.hpp"
#include "Core/LogHelper.hpp"

namespace Stand
{
	// on some cracked game builds
	static bool IsSocialClubNeverGoingToLoad()
	{
		return Pointers.ScriptThreads && Pointers.ScriptThreads->size() != 0;
	}

	bool Pointers::Init()
	{
		PatternCache::Init();


		const auto gta5 = g_ModuleMgr.Get("GTA5_Enhanced.exe"_J);

		if (!gta5)
		{
			LOG(FATAL) << "Could not find GTA5_Enhanced.exe, is this GTA 5 Enhanced?";
			return false;
		}

		PatternBatch batch;


		soup::Range versionRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("Version", std::move(versionRange), "4C 8D 0D ? ? ? ? 48 8D 5C 24 ? 48 89 D9 48 89 FA", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GameVersion = ptr.Add(3).Rip().As<const char*>();
			OnlineVersion = ptr.Add(0x47).Add(3).Rip().As<const char*>();
		});

		soup::Range hwndRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("HWND", std::move(hwndRange), "E8 ? ? ? ? 84 C0 74 25 48 8B 0D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			Hwnd = ptr.Add(9).Add(3).Rip().As<HWND*>();
		});

		soup::Range screenResRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScreenRes", std::move(screenResRange), "75 39 0F 57 C0 F3 0F 2A 05", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ScreenResX = ptr.Add(0x9).Rip().As<std::uint32_t*>();
			ScreenResY = ptr.Add(0x22).Rip().As<std::uint32_t*>();
		});

		soup::Range scriptThreadsRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScriptThreads", std::move(scriptThreadsRange), "48 8B 05 ? ? ? ? 48 89 34 F8 48 FF C7 48 39 FB 75 97", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ScriptThreads = ptr.Add(3).Rip().As<rage::atArray<rage::scrThread*>*>();
		});


		soup::Range initNativeTablesRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("InitNativeTables", std::move(initNativeTablesRange), "EB 2A 0F 1F 40 00 48 8B 54 17 10", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			InitNativeTables = ptr.Sub(0x2A).As<PVOID>();
		});

		soup::Range gameLogosRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GameLogos", std::move(gameLogosRange), "70 6C 61 74 66 6F 72 6D 3A 2F 6D 6F 76 69 65 73 2F 72 6F 63", [this](soup::Pointer p) {
			GameLogos = p.as<PVOID>();
		});

		soup::Range scriptProgramsRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScriptPrograms", std::move(scriptProgramsRange), "48 C7 84 C8 D8 00 00 00 00 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ScriptPrograms = ptr.Add(0x13).Add(3).Rip().Add(0xD8).As<rage::scrProgram**>();
		});

		soup::Range swapchainRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("IDXGISwapChain", std::move(swapchainRange), "72 C7 EB 02 31 C0 8B 0D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			CommandQueue = ptr.Add(0x1A).Add(3).Rip().As<ID3D12CommandQueue**>();
			SwapChain = ptr.Add(0x21).Add(3).Rip().As<IDXGISwapChain1**>();
		});

		soup::Range wndProcRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("WndProc", std::move(wndProcRange), "3D 85 00 00 00 0F 87 2D 02 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			WndProc = ptr.Sub(0x4F).As<PVOID>();
		});

		soup::Range runScriptThreadsRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("RunScriptThreads", std::move(runScriptThreadsRange), "BE 40 5D C6 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			RunScriptThreads = ptr.Sub(0xA).As<PVOID>();
		});

		soup::Range handlesAndPtrsRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("HandlesAndPtrs", std::move(handlesAndPtrsRange), "0F 1F 84 00 00 00 00 00 89 F8 0F 28 FE 41", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			HandleToPtr = ptr.Add(0x21).Add(1).Rip().As<Functions::HandleToPtr>();
			PtrToHandle = ptr.Sub(0xB).Add(1).Rip().As<Functions::PtrToHandle>();
		});

		soup::Range pedFactoryRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("PedFactory", std::move(pedFactoryRange), "C7 40 30 03 00 00 00 48 8B 0D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			PedFactory = ptr.Add(7).Add(3).Rip().As<CPedFactory**>();
		});

		soup::Range getNetPlayerFromPIDRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetNetPlayerFromPID", std::move(getNetPlayerFromPIDRange), "83 FB 20 74 2A 89 D9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetNetPlayerFromPid = ptr.Add(8).Rip().As<Functions::GetNetPlayerFromPid>();
		});

		soup::Range isSessionStartedRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("IsSessionStarted", std::move(isSessionStartedRange), "0F B6 05 ? ? ? ? 0A 05 ? ? ? ? 75 2A", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			IsSessionStarted = ptr.Add(3).Rip().As<bool*>();
		});

		soup::Range assignPhysicalIndexRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AssignPhysicalIndex", std::move(assignPhysicalIndexRange), "41 8D 47 01 3C 20 0F 87 68 01 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AssignPhysicalIndex = ptr.Sub(0x13).As<PVOID>();
		});

		soup::Range networkPlayerMgrShutdownRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetworkPlayerMgrShutdown", std::move(networkPlayerMgrShutdownRange), "84 C0 0F 84 88 00 00 00 4C 89 F9 E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetworkPlayerMgrShutdown = ptr.Sub(0x1B).As<PVOID>();
		});

		soup::Range scriptGlobalsRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScriptGlobals", std::move(scriptGlobalsRange), "48 8B 8E B8 00 00 00 48 8D 15 ? ? ? ? 49 89 D8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ScriptGlobals = ptr.Add(7).Add(3).Rip().As<std::int64_t**>();
		});

		soup::Range triggerWeaponDamageEventRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("TriggerWeaponDamageEvent", std::move(triggerWeaponDamageEventRange), "E8 ? ? ? ? 66 90 FF C5", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			TriggerWeaponDamageEvent = ptr.Add(1).Rip().As<Functions::TriggerWeaponDamageEvent>();
		});

		soup::Range regionCodeRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("RegionCode", std::move(regionCodeRange), "4C 8D 05 ? ? ? ? 48 89 F1 48 89 FA E8 ? ? ? ? 84 C0 74 3D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			RegionCode = ptr.Add(3).Rip().As<int*>();
		});

		soup::Range networkObjectMgrRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetworkObjectMgr&GetSyncTreeForType", std::move(networkObjectMgrRange), "41 83 7E FA 02 40 0F 9C C5 C1 E5 02", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetworkObjectMgr = ptr.Add(0xC).Add(3).Rip().As<CNetworkObjectMgr**>();
			GetSyncTreeForType = ptr.Add(0x13).Add(1).Rip().As<Functions::GetSyncTreeForType>();
		});

		soup::Range writeNodeDataRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("WriteNodeData", std::move(writeNodeDataRange), "48 8B 89 A8 00 00 00 48 8B 01 48 8B 40 10 48 FF E0", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			WriteNodeData = ptr.As<PVOID>();
		});

		soup::Range shouldUseNodeCacheRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ShouldUseNodeCache&Nullsub", std::move(shouldUseNodeCacheRange), "83 FA 20 74 1D 48 89 CE", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ShouldUseNodeCache = ptr.Sub(5).As<PVOID>();
			Nullsub = ptr.Add(0x29).As<PVOID>();
		});

		soup::Range isNodeInScopeRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("IsNodeInScope", std::move(isNodeInScopeRange), "41 83 F9 02 74 22 48 8B 06", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			IsNodeInScope = ptr.Sub(0x26).As<PVOID>();
		});

		soup::Range writeSyncTreeRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("WriteSyncTree", std::move(writeSyncTreeRange), "4D 89 CD 45 89 C6 41 89 D7 48 89 CF 8B 05 ? ? ? ? 65 48 8B 0C 25 58 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			WriteSyncTree = ptr.Sub(0x10).As<PVOID>();
		});

		soup::Range migrateObjectRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("MigrateObject", std::move(migrateObjectRange), "48 8B 96 D0 00 00 00 4C 89 F9 41 B8 03 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			MigrateObject = ptr.Add(0x10).Add(1).Rip().As<Functions::MigrateObject>();
		});

		soup::Range networkPlayerMgrRange2(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetworkPlayerMgr", std::move(networkPlayerMgrRange2), "75 0E 48 8B 05 ? ? ? ? 48 8B 88 F0 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetworkPlayerMgr = ptr.Add(2).Add(3).Rip().As<CNetworkPlayerMgr**>();
		});

		soup::Range queuePacketRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("QueuePacket", std::move(queuePacketRange), "45 89 F1 E8 ? ? ? ? 84 C0 74 25", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			QueuePacket = ptr.Add(3).Add(1).Rip().As<Functions::QueuePacket>();
		});

		soup::Range getNetObjectByIdRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetNetObjectById", std::move(getNetObjectByIdRange), "0F B7 4E 60 E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetNetObjectById = ptr.Add(4).Add(1).Rip().As<Functions::GetNetObjectById>();
		});

		soup::Range requestControlRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("RequestControl", std::move(requestControlRange), "74 0A 48 89 F9 E8 ? ? ? ? 31 F6 89 F0 48 83 C4 20", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			RequestControl = ptr.Add(5).Add(1).Rip().As<Functions::RequestControl>();
		});

		soup::Range spectatePatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("SpectatePatch", std::move(spectatePatchRange), "74 ? 66 83 FF 0D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			SpectatePatch = BytePatches::Add(p.as<std::uint8_t*>(), 0xEB);
		});

		// Checked working

		soup::Range writeNetArrayDataRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("WriteNetArrayData", std::move(writeNetArrayDataRange), "0F 84 06 03 00 00 0F B6 83", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			WriteNetArrayData = ptr.Sub(0x4E).As<PVOID>();
			NetArrayMgr = ptr.Sub(0x32).Add(3).As<rage::netArrayMgr**>();
		});

		soup::Range netArrayCachedDataPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetArrayCachedDataPatch", std::move(netArrayCachedDataPatchRange), "0F 84 64 FE FF FF FF", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetArrayCachedDataPatch = BytePatches::Add(ptr.As<void*>(), std::to_array<std::uint8_t>({0xE9, 0x65, 0xFE, 0xFF, 0xFF, 0x90}));
		});

		soup::Range statsMgrRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("CStatsMgr", std::move(statsMgrRange), "89 6C 24 28 48 8D 0D ? ? ? ? 48 8D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			StatsMgr = ptr.Add(4).Add(3).Rip().As<CStatsMgr*>();
		});

		soup::Range getPackedStatDataRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetPackedStatData", std::move(getPackedStatDataRange), "8D 81 37 FE FF FF", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetPackedStatData = ptr.Sub(0xE).As<Functions::GetPackedStatData>();
		});

		soup::Range netCatalogRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetCatalog&GetCatalogItem", std::move(netCatalogRange), "0F 82 55 FF FF FF 44 89 7C 24 30", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetCatalog = ptr.Add(0xB).Add(3).Rip().As<rage::netCatalog*>();
			GetCatalogItem = ptr.Add(0x17).Add(1).Rip().As<Functions::GetCatalogItem>();
		});

		soup::Range transactionMgrRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("TransactionMgr", std::move(transactionMgrRange), "48 8B 05 ? ? ? ? 80 78 39 00 74 2D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			TransactionMgr = ptr.Add(3).Rip().As<void**>();
		});

		soup::Range getActiveBasketRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetActiveBasket", std::move(getActiveBasketRange), "48 8B 40 10 81 7B 0C AE A0 A9 04", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetActiveBasket = ptr.Sub(0x39).As<Functions::GetActiveBasket>();
		});

		soup::Range pedPoolRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("PedPool", std::move(pedPoolRange), "80 79 4B 00 0F 84 F5 00 00 00 48 89 F1", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			PedPool = ptr.Add(0x18).Add(3).Rip().As<PoolEncryption*>();
		});

		soup::Range vehiclePoolRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("VehiclePool", std::move(vehiclePoolRange), "48 83 78 18 0D", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			VehiclePool = ptr.Sub(0xA).Add(3).Rip().As<rage::fwVehiclePool***>();
		});

		soup::Range objectPoolRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ObjectPool", std::move(objectPoolRange), "48 8B 04 0A C3 0F B6 05", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ObjectPool = ptr.Add(5).Add(3).Rip().As<PoolEncryption*>();
		});

		soup::Range httpStartRequestRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("HttpStartRequest", std::move(httpStartRequestRange), "56 57 48 83 EC 28 48 89 CE 8B 81 ? ? ? ? FF C8 83 F8 04 0F 87", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			HttpStartRequest = ptr.As<PVOID>();
		});

		soup::Range networkSessionRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetworkSession", std::move(networkSessionRange), "49 C7 86 F8 00 00 00 00 00 00 00 48 8B 05", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetworkSession = ptr.Add(0x17).Add(3).Rip().As<CNetworkSession**>();
		});

		soup::Range joinSessionByInfoRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("JoinSessionByInfo", std::move(joinSessionByInfoRange), "E8 ? ? ? ? 0F 10 87 ? ? ? ? 0F 11 86 ? ? ? ? 88 86 ? ? ? ? 84 C0", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			JoinSessionByInfo = ptr.Add(1).Rip().As<Functions::JoinSessionByInfo>();
		});

		soup::Range getSessionByGamerHandleRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetSessionByGamerHandle", std::move(getSessionByGamerHandleRange), "48 C7 84 24 80 00 00 00 10 00 00 08", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetSessionByGamerHandle = ptr.Sub(0x4A).Add(1).Rip().As<Functions::GetSessionByGamerHandle>();
		});

		soup::Range networkTimeRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetworkTime", std::move(networkTimeRange), "89 05 ? ? ? ? 80 3D ? ? ? ? ? 0F 84 ? ? ? ? E9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetworkTime = ptr.Add(2).Rip().As<std::uint32_t*>();
		});

		soup::Range gameTimerRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GameTimer", std::move(gameTimerRange), "3B 2D ? ? ? ? 76 ? 89 D9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GameTimer = ptr.Add(2).Rip().As<std::uint32_t*>();
		});

		soup::Range queueDependencyRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("QueueDependency&SigScanMemory", std::move(queueDependencyRange), "0F 29 46 50 48 8D 05", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			QueueDependency = ptr.Add(0x71).Add(1).Rip().As<PVOID>();
			SigScanMemory = ptr.Add(4).Add(3).Rip().As<PVOID>();
		});

		soup::Range scriptVMRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScriptVM", std::move(scriptVMRange), "49 63 41 1C", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ScriptVM = ptr.Sub(0x24).As<Functions::ScriptVM>();
		});

		soup::Range prepareMetricForSendingRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("PrepareMetricForSending", std::move(prepareMetricForSendingRange), "41 56 56 57 55 53 48 83 EC ? 4C 89 CB 4C 89 C6", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			PrepareMetricForSending = ptr.As<PVOID>();
		});

		soup::Range beDataRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("BEData", std::move(beDataRange), "48 C7 05 ? ? ? ? 00 00 00 00 E8 ? ? ? ? 48 89 C1 E8 ? ? ? ? E8 ? ? ? ? BD 0A 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BERestartStatus = ptr.Add(3).Rip().Add(8).Add(4).As<int*>();
			NeedsBERestart = ptr.Add(3).Rip().Add(8).Add(4).Add(8).As<bool*>();
			IsBEBanned = ptr.Add(3).Rip().Add(8).Add(4).Add(8).Add(4).As<bool*>();
		});

		soup::Range battlEyeStatusUpdatePatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("BattlEyeStatusUpdatePatch", std::move(battlEyeStatusUpdatePatchRange), "C6 05 ? ? ? ? 00 84 C0 0F 84 ? ? ? ? E9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BattlEyeStatusUpdatePatch = BytePatches::Add(ptr.Add(11).Rip().Add(1).Rip().As<void*>(), std::to_array<std::uint8_t>({0xC3}));
		});

		soup::Range statsMpCharacterMappingDataRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("CStatsMpCharacterMappingData", std::move(statsMpCharacterMappingDataRange), "48 8D 0D ? ? ? ? 89 F2 0F 28 74 24 ? 48 83 C4 38", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			StatsMpCharacterMappingData = ptr.Add(3).Rip().As<CStatsMpCharacterMappingData*>();
		});

		soup::Range scMembershipStuffRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ScMembershipStuff", std::move(scMembershipStuffRange), "48 8D 15 ? ? ? ? 41 B8 18 02 00 00 E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			HasGTAPlus = ptr.Add(3).Rip().As<int*>();
		});

		soup::Range battlEyeServerProcessPlayerJoinRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("BattlEyeServerProcessPlayerJoin", std::move(battlEyeServerProcessPlayerJoinRange), "48 89 10 48 89 48 10 89 F9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BattlEyeServerProcessPlayerJoin = ptr.Sub(4).Rip().As<PVOID*>()[1];
		});

		soup::Range getDLCHashRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetDLCHash&DLCManager", std::move(getDLCHashRange), "31 D2 E8 ? ? ? ? 3B 84", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			DLCManager = ptr.Sub(4).Rip().As<void**>();
			GetDLCHash = ptr.Add(3).Rip().As<PVOID>();
		});

		soup::Range gameDataHashRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GameDataHash", std::move(gameDataHashRange), "48 8D 3D ? ? ? ? 69 C9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GameDataHash = ptr.Add(3).Rip().As<CGameDataHash*>();
		});

		soup::Range assistedAimShouldReleaseEntityRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AssistedAimShouldReleaseEntity", std::move(assistedAimShouldReleaseEntityRange), "80 7F 28 04 75 6A", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AssistedAimShouldReleaseEntity = ptr.Sub(0xF).As<PVOID>();
		});

		soup::Range assistedAimFindNewTargetRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AssistedAimFindNewTarget", std::move(assistedAimFindNewTargetRange), "0F 84 C9 00 00 00 48 89 CE 48 89 F9", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AssistedAimFindNewTarget = ptr.Sub(0x33).As<Functions::AssistedAimFindNewTarget>();
		});

		soup::Range anticheatInitializedHashRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("InitializedHash-Anticheat&GetAnticheat", std::move(anticheatInitializedHashRange), "89 9E C8 00 00 00 48 8B 0D ? ? ? ? 48 85 C9 74 46", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AnticheatInitializedHash = ptr.Add(9).Rip().As<rage::Obf32**>();
			GetAnticheatInitializedHash = ptr.Add(0x13).Rip().As<PVOID>();
		});

		soup::Range anticheatContextRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AnticheatContext", std::move(anticheatContextRange), "48 8D BB 70 0A 00 00 4C 8D 35 ? ? ? ? 66 90", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AnticheatContext = ptr.Sub(0x12).Add(3).Rip().As<CAnticheatContext**>();
		});

		soup::Range getAnticheatInitializedHash2Range(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetAnticheatInitializedHash2", std::move(getAnticheatInitializedHash2Range), "89 9E E8 00 00 00 89 C2 E8 ? ? ? ? 69", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetAnticheatInitializedHash2 = ptr.Add(0x9).Rip().As<PVOID>();
		});

		soup::Range abilityBarPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AbilityBarPatch", std::move(abilityBarPatchRange), "75 39 48 85 F6 74 1A 48 89 F1 E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AbilityBarPatch = BytePatches::Add(ptr.As<std::uint16_t*>(), 0x9090);
		});

		soup::Range matchmakingAdvertiseRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("MatchmakingAdvertise", std::move(matchmakingAdvertiseRange), "C7 47 30 01 00 00 00 E9 92", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			MatchmakingAdvertise = ptr.Sub(0xC).Rip().As<PVOID>();
		});

		soup::Range matchmakingUpdateRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("MatchmakingUpdate", std::move(matchmakingUpdateRange), "C7 47 30 02 00 00 00 EB 7A", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			MatchmakingUpdate = ptr.Sub(0x8).Rip().As<PVOID>();
		});

		soup::Range matchmakingUnadvertiseRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("MatchmakingUnadvertise", std::move(matchmakingUnadvertiseRange), "C7 86 C8 01 00 00 04 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			MatchmakingUnadvertise = ptr.Sub(0xC).Rip().As<PVOID>();
		});

		soup::Range matchmakingSessionDetailSendResponseRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("SessionDetailSendResponse", std::move(matchmakingSessionDetailSendResponseRange), "48 B8 01 00 00 00 0D 00 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			MatchmakingSessionDetailSendResponse = ptr.Add(0x2F).Rip().As<PVOID>();
		});

		soup::Range gameSkeletonUpdateRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GameSkeletonUpdate", std::move(gameSkeletonUpdateRange), "56 48 83 EC 20 48 8B 81 40 01 00 00 48 85 C0", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GameSkeletonUpdate = ptr.As<PVOID>();
		});

		soup::Range modelSpawnBypassRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ModelSpawnBypass", std::move(modelSpawnBypassRange), "E8 ? ? ? ? 48 8B 78 48", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ModelSpawnBypass = BytePatches::Add(ptr.Add(1).Rip().Add(0x2B).As<std::uint8_t*>(), 0xEB);
		});

		soup::Range worldModelSpawnBypassRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("WorldModelSpawnBypass", std::move(worldModelSpawnBypassRange), "4C 8B 2C 01 4D 85 ED 0F 84 ? ? ? ?", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			WorldModelSpawnBypass = BytePatches::Add(ptr.Add(4).As<void*>(), std::to_array<std::uint8_t>({0xEB, 0x12, 0x90}));
		});

		soup::Range receiveNetMessageRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ReceiveNetMessage", std::move(receiveNetMessageRange), "48 81 C1 00 03 00 00 4C 89 E2", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ReceiveNetMessage = ptr.Add(0xD).Add(1).Rip().As<PVOID>();
		});

		soup::Range netEventMgrRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("NetEventMgr", std::move(netEventMgrRange), "4C 8B 05 ? ? ? ? 44 0F B7 CA", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			NetEventMgr = ptr.Add(3).Rip().As<rage::netEventMgr**>();
		});

		soup::Range sendEventAckRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("SendEventAck", std::move(sendEventAckRange), "E8 ? ? ? ? 84 C0 75 ? 44 89 F5", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			EventAck = ptr.Add(1).Rip().As<Functions::EventAck>();
			SendEventAck = ptr.Add(0x1A).Add(1).Rip().As<Functions::SendEventAck>();
		});

		soup::Range formatIntCaller1Range(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("FormatIntCaller1", std::move(formatIntCaller1Range), "48 89 35 ? ? ? ? 48 8B 74 24", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			FormatIntCaller1 = ptr.Add(0x5D).As<PVOID>();
		});

		soup::Range formatIntCaller2Range(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("FormatIntCaller2", std::move(formatIntCaller2Range), "48 B8 20 73 69 7A 65 3D 27 32 48 89 84 24", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			FormatIntCaller2 = ptr.Sub(0x11).As<PVOID>();
		});

		soup::Range shouldTargetEntityPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ShouldNotTargetEntityPatch", std::move(shouldTargetEntityPatchRange), "F6 80 A9 14 00 00 01", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ShouldNotTargetEntityPatch = BytePatches::Add(ptr.Sub(0x53).As<void*>(), std::to_array<std::uint8_t>({0xB0, 0x00, 0xC3}));
		});

		soup::Range getAssistedAimTypePatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetAssistedAimTypePatch", std::move(getAssistedAimTypePatchRange), "FF E0 48 8D 86", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetAssistedAimTypePatch = BytePatches::Add(ptr.Sub(0x15).As<void*>(), std::to_array<std::uint8_t>({0xBD, 0x01, 0x00, 0x00, 0x00}));
		});

		soup::Range getLockOnPosPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetLockOnPosPatch", std::move(getLockOnPosPatchRange), "0F 29 74 24 ? 48 89 D6 48 89 CF 48 8B 05", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetLockOnPosPatch = BytePatches::Add(ptr.Add(0x22).As<std::uint8_t*>(), 0xEB);
		});

		soup::Range shouldAllowDriverLockOnPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("ShouldAllowDriverLockOnPatch", std::move(shouldAllowDriverLockOnPatchRange), "75 ? 45 89 C7 49 89 CE", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			ShouldAllowDriverLockOnPatch = BytePatches::Add(ptr.Sub(0x2C).As<std::uint8_t*>(), std::to_array<std::uint8_t>({0xB0, 0x01, 0xC3}));
		});

		soup::Range allowPausingInSessionPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("AllowPausingInSessionPatch", std::move(allowPausingInSessionPatchRange), "80 88 ? ? ? ? ? EB ? E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			AllowPausingInSessionPatch = BytePatches::Add(ptr.Sub(0x1E).As<std::uint8_t*>(), 0xEB);
		});

		soup::Range getPoolTypeRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("GetPoolType", std::move(getPoolTypeRange), "BA CE 94 A6 ED E8", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetPoolType = ptr.Sub(19).As<PVOID>();
		});

		soup::Range setJoinRequestPoolTypePatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("SetJoinRequestPoolTypePatch", std::move(setJoinRequestPoolTypePatchRange), "89 86 ? ? ? ? E8 ? ? ? ? 89 C0", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			SetJoinRequestPoolTypePatch = BytePatches::Add(ptr.Sub(5).As<std::uint8_t*>(), std::to_array<std::uint8_t>({0xB8, 0x00, 0x00, 0x00, 0x00}));
		});

		soup::Range handleJoinRequestIgnorePoolPatchRange(reinterpret_cast<void*>(gta5->Base()), gta5->Size());
		batch.Add("HandleJoinRequestIgnorePoolPatch", std::move(handleJoinRequestIgnorePoolPatchRange), "41 83 FF 05 0F 84 ? ? ? ? 8B 84 F7", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			HandleJoinRequestIgnorePoolPatch = BytePatches::Add(ptr.As<void*>(), std::to_array<std::uint8_t>({0x39, 0xC9, 0x90, 0x90}));
		});


		if (!batch.Run())
		{
			LOG(FATAL) << "Failed to initialize pointer patterns";
			return false;
		}

		PatternCache::Update();

		return true;
	}

	bool Pointers::LateInit()
	{
		auto sc = g_ModuleMgr.Get("socialclub.dll"_J);
		while (!sc)
		{
			LOG(WARNING) << "Waiting for socialclub.dll";
			std::this_thread::sleep_for(5s);

			if (IsSocialClubNeverGoingToLoad())
			{
				LOG(WARNING) << "Timed out checking for socialclub.dll";
				return false;
			}

			g_ModuleMgr.LoadModules();
			sc = g_ModuleMgr.Get("socialclub.dll"_J);
		}

		PatternBatch batch;

		soup::Range getPresenceAttributesRange(reinterpret_cast<void*>(sc->Base()), sc->Size());
		batch.Add("GetPresenceAttributes", std::move(getPresenceAttributesRange), "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 40 33 DB 49", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetPresenceAttributes = ptr.As<Functions::GetPresenceAttributes>();
		});

		soup::Range numHandlesPatchRange(reinterpret_cast<void*>(sc->Base()), sc->Size());
		batch.Add("NumHandlesPatch", std::move(numHandlesPatchRange), "83 FD 20 0F 87 54 02 00 00", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BytePatches::Add(ptr.Add(2).As<std::uint8_t*>(), 100)->Apply();
		});

		soup::Range readAttributePatchRange(reinterpret_cast<void*>(sc->Base()), sc->Size());
		batch.Add("ReadAttributesPatch", std::move(readAttributePatchRange), "75 70 EB 23", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BytePatches::Add(ptr.As<void*>(), std::array<std::uint8_t, 2>{0x90, 0x90})->Apply();
		});

		soup::Range readAttributePatch2Range(reinterpret_cast<void*>(sc->Base()), sc->Size());
		batch.Add("ReadAttributesPatch2", std::move(readAttributePatch2Range), "32 C0 EB ? C7 83", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			BytePatches::Add(ptr.As<void*>(), std::array<std::uint8_t, 2>{0xB0, 0x01})->Apply();
		});

		soup::Range getAvatarsRange(reinterpret_cast<void*>(sc->Base()), sc->Size());
		batch.Add("GetAvatars", std::move(getAvatarsRange), "89 4B ? 48 8B CB E8 ? ? ? ? 84 C0", [this](soup::Pointer p) {
			PointerCalculator ptr(p.as<std::uintptr_t>());
			GetAvatars = ptr.Add(6).Add(1).Rip().As<Functions::GetAvatars>();
		});

		if (!batch.Run())
		{
			LOG(WARNING) << "Some Social Club patterns could not be found";
			return false;
		}

		PatternCache::Update();

		return true;
	}
}