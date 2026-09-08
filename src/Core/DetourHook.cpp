#include "Core/DetourHook.hpp"

#include <stdexcept>

#include <MinHook.h>



namespace Stand
{
	DetourHook::DetourHook(
	    std::string_view name,
	    void* target,
	    void* detour) :
	    m_Name(name),
	    m_TargetFunc(target),
	    m_DetourFunc(detour)
	{
	}

	DetourHook::~DetourHook()
	{
		RemoveHook();
	}

	bool DetourHook::CreateHook()
	{
		if (!isValid())
		{
			if (isMandatory())
			{
				throw std::runtime_error("Failed to create mandatory hook: invalid target.");
			}

			return false;
		}

		if (isHooked())
			return true;

		if (isLongjump())
		{
			throw std::runtime_error("DH_LONGJUMP is not supported yet.");
		}

		OptimizeHook();

		if (!m_EffectiveTarget)
			return false;

		const auto result = MH_CreateHook(m_EffectiveTarget, m_DetourFunc, &m_OriginalFunc);

		if (result != MH_OK)
		{
			m_EffectiveTarget = nullptr;
			m_OriginalFunc = nullptr;

			if (isMandatory())
			{
				throw std::runtime_error("Failed to create mandatory hook.");
			}

			return false;
		}

		return true;
	}

	bool DetourHook::Enable()
	{
		if (m_Enabled)
			return false;

		if (!isHooked())
			return false;

		const auto result = MH_QueueEnableHook(m_EffectiveTarget);

		if (result != MH_OK)
		{
			throw std::runtime_error("Failed to queue hook to be enabled.");
		}

		m_Enabled = true;

		return true;
	}

	bool DetourHook::Disable()
	{
		if (!m_Enabled)
			return false;

		if (!isHooked())
			return false;

		const auto result = MH_QueueDisableHook(m_EffectiveTarget);

		if (result != MH_OK)
		{
			throw std::runtime_error("Failed to queue hook to be disabled.");
		}

		m_Enabled = false;

		return true;
	}

	bool DetourHook::EnableNow()
	{
		if (m_Enabled)
			return false;

		if (!isHooked())
			return false;

		const auto result = MH_EnableHook(m_EffectiveTarget);

		if (result != MH_OK)
		{
			throw std::runtime_error("Failed to enable hook right now.");
		}

		m_Enabled = true;

		return true;
	}

	bool DetourHook::DisableNow()
	{
		if (!m_Enabled)
			return false;

		if (!isHooked())
			return false;

		const auto result = MH_DisableHook(m_EffectiveTarget);

		if (result != MH_OK)
		{
			throw std::runtime_error("Failed to disable hook right now.");
		}

		m_Enabled = false;

		return true;
	}

	bool DetourHook::EnableHook()
	{
		return EnableNow();
	}

	bool DetourHook::EnableHookQueued()
	{
		return Enable();
	}

	bool DetourHook::DisableHook()
	{
		return DisableNow();
	}

	bool DetourHook::DisableHookQueued()
	{
		return Disable();
	}

	bool DetourHook::RemoveHook()
	{
		if (!isHooked())
			return false;

		const auto result = MH_RemoveHook(m_EffectiveTarget);

		if (result != MH_OK)
		{
			throw std::runtime_error("Failed to remove hook.");
		}

		m_OriginalFunc = nullptr;
		m_EffectiveTarget = nullptr;
		m_Enabled = false;

		return true;
	}

	void DetourHook::OptimizeHook()
	{
		if (!m_TargetFunc)
			return;

		// Respect DH_NOFOLLOWJUMPS.
		if (!canFollowJumps())
		{
			m_EffectiveTarget = m_TargetFunc;
			return;
		}

		auto ptr = PointerCalculator(m_TargetFunc);

		while (ptr.As<std::uint8_t&>() == 0xE9)
		{
			ptr = ptr.Add(1).Rip();
		}

		m_EffectiveTarget = ptr.As<void*>();
	}
}