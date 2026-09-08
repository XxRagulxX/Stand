#include "Commands/CommandToggleLegacy.hpp"
#include "Scripting/FiberPool.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	CommandToggleLegacy::CommandToggleLegacy(std::string name, std::string label, std::string description, bool def_value) :
	    CommandLegacy(name, label, description, 0),
	    m_State(def_value)
	{
		Commands::AddBoolCommand(this);
	}

	void CommandToggleLegacy::OnCall()
	{
		SetState(!m_State);
	}

	void CommandToggleLegacy::SaveState(nlohmann::json& value)
	{
		value = m_State;
	}

	void CommandToggleLegacy::LoadState(nlohmann::json& value)
	{
		m_State = value;
	}

	bool CommandToggleLegacy::GetState()
	{
		return m_State;
	}

	void CommandToggleLegacy::SetState(bool state)
	{
		if (state && !m_State)
		{
			FiberPool::queueJob([this] {
				OnEnable();
				m_Ready = true;
			});
		}
		else if (!state && m_State)
		{
			FiberPool::queueJob([this] {
				OnDisable();
				m_Ready = true;
			});
		}

		m_Ready = false;
		m_State = state;
		MarkDirty();
	}

	void CommandToggleLegacy::Initialize()
	{
		OnEnable();
		m_Ready = true;
	}

	void CommandToggleLegacy::Shutdown()
	{
		m_Ready = false;
		OnDisable();
	}
}