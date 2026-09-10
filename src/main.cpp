#include "Core/common.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Widgets/CommandHotkeyDispatch.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Config/HotkeySystem.hpp"
#include "Config/Settings.hpp"
#include "Core/FileMgr.hpp"
#include "Rendering/Notifications.hpp"
#include "Core/Hooking.hpp"
#include "Core/ModuleMgr.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Core/Wine.hpp"
#include "Scripting/LuaManager.hpp"
#include "AntiCheat/AnticheatBypass.hpp"
#include "Network/Players.hpp"
#include "Commands/Extra/CommandSavedLocations.hpp"
#include "Commands/Extra/CommandSavedPlayers.hpp"
#include "World/Self.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Network/Tunables.hpp"
#include "Scripting/ScriptPointers.hpp"
#include "Menu/GUI.hpp"
#include "Core/Pointers.hpp"
#include "Commands/Vehicle/CommandSavePersonalVehicle.hpp"
#include "Commands/Weapons/CommandOpenGunLocker.hpp"
#include "Commands/World/CommandDailyActivities.hpp"
#include "Core/LogHelper.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Commands/Self/CommandTabLevitation.hpp"
#include "Commands/Self/CommandTabSelf.hpp"
#include "Commands/Settings/CommandTabAddressBar.hpp"
#include "Commands/Settings/CommandTabAppearanceDirect.hpp"
#include "Commands/Settings/CommandTabBorder.hpp"
#include "Commands/Settings/CommandTabEntityPreviews.hpp"
#include "Commands/Settings/CommandTabFontText.hpp"
#include "Commands/Settings/CommandTabInput.hpp"
#include "Commands/Settings/CommandTabTextures.hpp"
#include "Commands/Settings/CommandTabColours.hpp"
#include "Commands/Settings/CommandTabCommandInfoText.hpp"
#include "Commands/Settings/CommandTabCommands.hpp"
#include "Commands/Settings/CommandTabCursor.hpp"
#include "Commands/Settings/CommandTabPosition.hpp"
#include "Commands/Settings/CommandTabScrollbar.hpp"
#include "Commands/Settings/CommandTabTabs.hpp"

namespace Stand
{
	DWORD Main(void*)
	{
		const auto documents = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced";
		FileMgr::Init(documents);

		HMODULE module = GetModuleHandle(nullptr);

		LogHelper::Init("StandEnhanced", FileMgr::GetProjectFile("./cout.log"));

		LOGF(INFO, "Welcome to StandEnhanced! Build date: {} at {}", __DATE__, __TIME__);

		g_HotkeySystem.RegisterCommands();
		SavedLocations::FetchSavedLocations();
		(void)Features::GetCommandTabSelf();
		(void)Features::GetCommandTabLevitation();
		(void)Features::GetCommandTabAddressBar();
		(void)Features::GetCommandTabAppearanceDirect();
		(void)Features::GetCommandTabBorder();
		(void)Features::GetCommandTabEntityPreviews();
		(void)Features::GetCommandTabFontText();
		(void)Features::GetCommandTabInput();
		(void)Features::GetCommandTabTextures();
		(void)Features::GetCommandTabColours();
		(void)Features::GetCommandTabCommandInfoText();
		(void)Features::GetCommandTabCommands();
		(void)Features::GetCommandTabCursor();
		(void)Features::GetCommandTabPosition();
		(void)Features::GetCommandTabScrollbar();
		(void)Features::GetCommandTabTabs();
		Settings::Initialize(FileMgr::GetProjectFile("./settings.json"));

		bool rendererInitialized = false;
		bool hookingInitialized = false;
		bool exceptionalInitialized = false;

		if (!g_ModuleMgr.LoadModules())
			goto EARLY_UNLOAD;

		if (g_ModuleMgr.IsManualMapped())
			LOGF(WARNING, "Manual mapping detected, switch to normal injection if you're having issues");

		if (!Pointers.Init())
			goto EARLY_UNLOAD;

		AnticheatBypass::RunOnStartup();

		if (!Renderer::Init())
			goto EARLY_UNLOAD;

		rendererInitialized = true;

		Rendering::GridRenderer::Init();

		Players::Init();

		Hooking::Init();
		hookingInitialized = true;

		exceptional_init(&Exceptional::handleCaughtException, &Exceptional::handleUncaughtException);

		(void)Exceptional::setUnhandledExceptionHandler();

		Exceptional::createManagedThread(
		    []() {
			    Exceptional::thread_func();
		    });

		exceptionalInitialized = true;

		LOG(INFO) << "Exceptional initialized";

	
		LOG(INFO) << "ScriptMgr initialized";

		ScriptPointers::Init();

		GUI::Init();

		g_script_mgr.addScript(module, std::make_unique<Script>(&NativeHooks::RunScript)); // runs once
		g_script_mgr.addScript(module, std::make_unique<Script>(&Tunables::RunScript));    // runs once
		g_script_mgr.addScript(module, std::make_unique<Script>(&AnticheatBypass::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Self::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&GUI::RunScript));

		g_script_mgr.addScript(module, std::make_unique<Script>(&LuaManager::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&HotkeySystem::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Commands::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Stand::CommandHotkeyDispatch::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Stand::CommandTickDispatch::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Features::SavePersonalVehicle::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Features::OpenGunLocker::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&Features::OpenStreetDealerMenu::RunScript));
		g_script_mgr.addScript(module, std::make_unique<Script>(&SavedPlayers::RunScript));

		if (!Pointers.LateInit())
			LOG(WARNING) << "Socialclub patterns failed to load";

		Notifications::Show("StandEnhanced", "Loaded succesfully", NotificationType::Success);

		if (InWine().value_or(false))
		    LOG(INFO) << "Running in Wine!";

		while (g_Running)
		{
			Settings::Tick();
			std::this_thread::yield();
		}
	EARLY_UNLOAD:

		LOG(INFO) << "=== UNLOAD BEGIN ===";

		g_Running = false;

		LOG(INFO) << "Stopping ScriptMgr";
		g_script_mgr.deinit();
		LOG(INFO) << "ScriptMgr stopped";

		if (exceptionalInitialized)
		{
			LOG(INFO) << "Stopping Exceptional";
			Exceptional::disableExceptionHandling();
			LOG(INFO) << "Exceptional stopped";
		}

		if (hookingInitialized)
		{
			LOG(INFO) << "Destroying NativeHooks";
			NativeHooks::Destroy();
			LOG(INFO) << "NativeHooks destroyed";

			LOG(INFO) << "Destroying Hooking";
			Hooking::Destroy();
			LOG(INFO) << "Hooking destroyed";
		}

		if (rendererInitialized)
		{
			LOG(INFO) << "Destroying Renderer";
			Renderer::Destroy();
			LOG(INFO) << "Renderer destroyed";
		}

		LOG(INFO) << "=== StandEnhanced shutdown complete ===";

		LogHelper::Destroy();

		if (g_MainThread)
		{
			CloseHandle(g_MainThread);
			g_MainThread = nullptr;
		}

		FreeLibraryAndExitThread(g_DllInstance, EXIT_SUCCESS);
	}
}

BOOL WINAPI DllMain(HINSTANCE dllInstance, DWORD reason, void*)
{
	using namespace Stand;

	if (dllInstance)
		DisableThreadLibraryCalls(dllInstance);

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_DllInstance = dllInstance;

		g_MainThread = CreateThread(nullptr, 0, Main, nullptr, 0, &g_MainThreadId);
	}
	return true;
}