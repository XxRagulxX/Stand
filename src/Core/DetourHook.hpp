#pragma once

#include "Core/PointerCalculator.hpp"

#include <cstdint>
#include <string_view>

namespace Stand
{
	enum DetourHookFlags : std::uint8_t
	{
		DH_NONE = 0,
		DH_MANDATORY = 1 << 0,
		DH_NOFOLLOWJUMPS = 1 << 1,
		DH_LONGJUMP = 1 << 2,

#ifdef STAND_DEBUG
		DH_FORCEFAIL = 1 << 7,
#endif
	};

	enum class HookGroup : std::uint8_t
	{
		Minimal,
		Passive,
		Main
	};

	class DetourHook
	{
	private:
		// Basic hook information
		std::string_view m_Name;
		bool m_Enabled = false;

		// Hook functions
		void* m_TargetFunc = nullptr;
		void* m_DetourFunc = nullptr;
		void* m_OriginalFunc = nullptr;

		// Resolved/effective target
		void* m_EffectiveTarget = nullptr;

		// Stand-compatible flags
		std::uint8_t m_Flags = DH_NONE;

		// Hook lifecycle group
		HookGroup m_Group = HookGroup::Main;

	public:
		DetourHook(
		    std::string_view name,
		    void* target,
		    void* detour);

		virtual ~DetourHook();

		DetourHook(const DetourHook&) = delete;
		DetourHook& operator=(const DetourHook&) = delete;

		DetourHook(DetourHook&&) = delete;
		DetourHook& operator=(DetourHook&&) = delete;

		[[nodiscard]]
		std::string_view Name() const noexcept
		{
			return m_Name;
		}

		[[nodiscard]]
		bool IsEnabled() const noexcept
		{
			return m_Enabled;
		}

		bool Enable();
		bool Disable();

		bool EnableNow();
		bool DisableNow();

		bool CreateHook();

		bool EnableHook();
		bool EnableHookQueued();

		bool DisableHook();
		bool DisableHookQueued();

		bool RemoveHook();

		void init(void* detour, void* target, std::uint8_t flags = DH_NONE) noexcept
		{
			m_DetourFunc = detour;
			m_TargetFunc = target;
			m_Flags = flags;

			m_EffectiveTarget = nullptr;
			m_OriginalFunc = nullptr;
			m_Enabled = false;
		}

		void setTarget(void* target) noexcept
		{
			m_TargetFunc = target;
			m_EffectiveTarget = nullptr;
			m_OriginalFunc = nullptr;
			m_Enabled = false;
		}

		void SetGroup(HookGroup group) noexcept
		{
			m_Group = group;
		}

		[[nodiscard]]
		HookGroup GetGroup() const noexcept
		{
			return m_Group;
		}

		template<typename T>
		[[nodiscard]]
		T Original() const noexcept
		{
			return reinterpret_cast<T>(m_OriginalFunc);
		}

		template<typename T>
		[[nodiscard]]
		T getOriginal() const noexcept
		{
			return Original<T>();
		}


		[[nodiscard]]
		bool isValid() const noexcept
		{
			return m_TargetFunc != nullptr
			    && m_DetourFunc != nullptr;
		}

		[[nodiscard]]
		bool isHooked() const noexcept
		{
			return m_OriginalFunc != nullptr;
		}

		[[nodiscard]]
		bool isMandatory() const noexcept
		{
			return (m_Flags & DH_MANDATORY) != 0;
		}

		[[nodiscard]]
		bool canFollowJumps() const noexcept
		{
			return (m_Flags & DH_NOFOLLOWJUMPS) == 0;
		}

		[[nodiscard]]
		bool isLongjump() const noexcept
		{
			return (m_Flags & DH_LONGJUMP) != 0;
		}

	private:
		void OptimizeHook();
	};
}