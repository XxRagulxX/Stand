#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "Core/DetourHook.hpp"

namespace Stand
{
	class Hooking
	{
	private:
		Hooking();

		// Central ownership of all hooks.
		std::vector<std::unique_ptr<DetourHook>> m_Hooks;

		// Creates and owns a hook.
		template<auto HookFunc>
		DetourHook* AddHook(
		    std::string_view name,
		    void* target,
		    void* detour,
		    HookGroup group = HookGroup::Main);

		// Internal lookup.
		DetourHook* FindHook(std::string_view name);

	public:

		template<auto HookFunc>
		struct HookHelper
		{
			inline static DetourHook* m_Hook = nullptr;
		};

		template<auto HookFunc>
		static void RegisterHook(DetourHook* hook)
		{
			HookHelper<HookFunc>::m_Hook = hook;
		}

		template<auto HookFunc>
		[[nodiscard]]
		static DetourHook* Get()
		{
			return HookHelper<HookFunc>::m_Hook;
		}

		virtual ~Hooking();

		Hooking(const Hooking&) = delete;
		Hooking(Hooking&&) = delete;
		Hooking& operator=(const Hooking&) = delete;
		Hooking& operator=(Hooking&&) = delete;

		static bool Init();
		static void Destroy();

		// Public hook lookup.
		static DetourHook* GetHook(std::string_view name);

	private:
		bool InitImpl();
		void DestroyImpl();

		std::vector<DetourHook*> GetMinimalHooks();
		std::vector<DetourHook*> GetPassiveHooks();
		std::vector<DetourHook*> GetMainHooks();
		std::vector<DetourHook*> GetAllHooks();

		void BatchCreate(const std::vector<DetourHook*>& hooks);
		void BatchEnable(const std::vector<DetourHook*>& hooks);
		void BatchDisable(const std::vector<DetourHook*>& hooks);
		void BatchRemove(const std::vector<DetourHook*>& hooks);

	private:
		static Hooking& GetInstance()
		{
			static Hooking instance;
			return instance;
		}
	};
}