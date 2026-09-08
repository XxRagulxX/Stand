#include "Commands/Weapons/CommandOpenGunLocker.hpp"
#include "Commands/CommandLegacy.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Rendering/Notifications.hpp"
#include "World/Self.hpp"
#include "Scripting/ScriptPatches.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptFunction.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/scrThread.hpp"
#include "Network/GlobalPlayerBD.hpp"

namespace Stand::Features
{
	static ScriptPatch skipGunLockerMenuChecksPatch{};

	void OpenGunLocker::RunScriptImpl()
	{
		while (g_Running)
		{
			Script::current()->yield();

			if (!m_ShouldRunScript)
				continue;

			if (!m_Thread)
			{
				int id = Scripts::StartScript("AM_MP_AUTO_SHOP"_J, eStackSizes::WAREHOUSE);
				m_Thread = Scripts::FindScriptThreadByID(id);
				if (m_Thread)
				{
					m_Thread->m_Context.m_State = rage::scrThread::State::PAUSED;
					if (!skipGunLockerMenuChecksPatch)
					{
						skipGunLockerMenuChecksPatch = ScriptPatches::AddPatch("AM_MP_AUTO_SHOP"_J, ScriptPointer("SkipGunLockerMenuChecksPatch", "72 5D ? ? ? 56 ? ? 2C ? ? ? 2C ? ? ? 06").Add(5), {0x55, 0xB1, 0x00});
					}
					skipGunLockerMenuChecksPatch->Enable();
				}
				else
				{
					m_ShouldRunScript = false;
					continue;
				}
			}

			static ScriptFunction runGunLockerMenu("AM_MP_AUTO_SHOP"_J, ScriptPointer("RunGunLockerMenu", "2D 06 08 00 00 38 03 5D ? ? ? 57 03 00"));
			auto gunLockerData = ScriptLocal(m_Thread, 301).At(568);

			int unused;
			runGunLockerMenu.Call<void>(gunLockerData.As<void*>(), &unused, eSimpleInteriorIndex::SIMPLE_INTERIOR_AUTO_SHOP_MISSION_ROW, Self::GetPlayer().GetId(), false, true);
			if (*gunLockerData.At(20).As<int*>() == 0)
			{
				skipGunLockerMenuChecksPatch->Disable();
				m_Thread->Kill();
				m_Thread->m_Context.m_State = rage::scrThread::State::KILLED;
				m_Thread = nullptr;
				m_ShouldRunScript = false;
			}
		}
	}

	class _OpenGunLocker : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
			{
				Notifications::Show("Open Gun Locker", "Please join GTA Online.", NotificationType::Error);
				return;
			}

			if (GlobalPlayerBD::Get()->Entries[Self::GetPlayer().GetId()].SimpleInteriorData.Index != eSimpleInteriorIndex::SIMPLE_INTERIOR_INVALID)
			{
				Notifications::Show("Open Gun Locker", "Cannot open gun locker while in an interior.", NotificationType::Error);
				return;
			}

			OpenGunLocker::SetShouldRunScript(true);
		}
	};

	static _OpenGunLocker __OpenGunLocker{"opengunlocker", "Open Gun Locker", "Allows you to access gun locker remotely."};
}