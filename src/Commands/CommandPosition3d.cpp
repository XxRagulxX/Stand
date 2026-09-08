#include "Commands/CommandPosition3d.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandPosition3d::OnCall()
	{
	}

	void CommandPosition3d::SaveState(nlohmann::json& value)
	{
		value = nlohmann::json::object();
		value["x"] = m_State.x;
		value["y"] = m_State.y;
		value["z"] = m_State.z;
	}

	void CommandPosition3d::LoadState(nlohmann::json& value)
	{
		if (value.is_object())
		{
			m_State.x = value["x"];
			m_State.y = value["y"];
			m_State.z = value["z"];
		}
	}

	CommandPosition3d::CommandPosition3d(std::string name, std::string label, std::string description, rage::fvector3 def_val) :
	    CommandLegacy(name, label, description, 0),
	    m_State(def_val)
	{
	}

	rage::fvector3 CommandPosition3d::GetState()
	{
		return m_State;
	}

	void CommandPosition3d::SetState(const rage::fvector3& state)
	{
		FiberPool::queueJob([this] {
			OnChange();
		});
		m_State = state;
		MarkDirty();
	}
}