#include "World/Self.hpp"

#include "Scripting/ScriptMgr.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/Natives.hpp"
#include "Ped/CPedFactory.hpp"

namespace Stand
{
	Self::Self() :
	    m_SelfPed(nullptr),
	    m_SelfPlayer(nullptr),
	    m_Vehicle(nullptr)
	{
	}

	void Self::RunScriptImpl()
	{
		while (g_Running)
		{
			UpdateImpl();
			Script::current()->yield();
		}
	}

	void Self::UpdateImpl()
	{
		CPed* ped_ptr = nullptr;
		if (auto factory = *Pointers.PedFactory)
			ped_ptr = factory->m_LocalPed;
		m_SelfPed = Ped(ped_ptr, PLAYER::PLAYER_PED_ID());
		m_SelfPlayer = Player(PLAYER::PLAYER_ID());
		if (m_SelfPed.IsValid())
		{
			m_Vehicle = m_SelfPed.GetVehicle();
		}
	}
}