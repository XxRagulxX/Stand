#include "Commands/CommandColourCustom.hpp"
#include "Scripting/FiberPool.hpp"

namespace
{
	bool AreColorsEqual(ImVec4 col1, ImVec4 col2)
	{
		return col1.x == col2.x && col1.y == col2.y && col1.z == col2.z && col1.w == col2.w;
	}
}

namespace Stand
{
	void CommandColourCustom::OnCall()
	{
	}

	void CommandColourCustom::SaveState(nlohmann::json& value)
	{
		value = nlohmann::json::object();
		value["r"] = m_State.x;
		value["g"] = m_State.y;
		value["b"] = m_State.z;
		value["a"] = m_State.w;
	}

	void CommandColourCustom::LoadState(nlohmann::json& value)
	{
		if (value.is_object())
		{
			m_State.x = value["r"];
			m_State.y = value["g"];
			m_State.z = value["b"];
			m_State.w = value["a"];
		}
		else if (value.is_array())
		{
			auto arr = value.get<std::array<float, 4>>();
			m_State.x = arr[0];
			m_State.y = arr[1];
			m_State.z = arr[2];
			m_State.w = arr[3];
		}
	}

	CommandColourCustom::CommandColourCustom(std::string name, std::string label, std::string description, ImVec4 color) :
	    CommandLegacy(name, label, description, 0),
	    m_State(color)
	{
	}

	ImVec4 CommandColourCustom::GetState()
	{
		return m_State;
	}

	void CommandColourCustom::SetState(ImVec4 state)
	{
		if (!AreColorsEqual(m_State, state))
		{
			FiberPool::queueJob([this] {
				OnChange();
			});
			m_State = state;
			MarkDirty();
		}
	}
}