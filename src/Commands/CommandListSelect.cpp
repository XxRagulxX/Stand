#include "Commands/CommandListSelect.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandListSelect::OnCall()
	{
	}

	void CommandListSelect::SaveState(nlohmann::json& value)
	{
		value = m_State;
	}

	void CommandListSelect::LoadState(nlohmann::json& value)
	{
		m_State = value;
	}

	CommandListSelect::CommandListSelect(std::string name, std::string label, std::string description, std::vector<std::pair<int, const char*>> list, int def_val) :
	    CommandLegacy(name, label, description, 0),
	    m_List(list),
	    m_State(def_val)
	{
	}

	int CommandListSelect::GetState()
	{
		return m_State;
	}

	void CommandListSelect::SetState(int state)
	{
		FiberPool::queueJob([this] {
			OnChange();
		});
		m_State = state;
		MarkDirty();
	}

	void CommandListSelect::SetList(std::vector<std::pair<int, const char*>> list)
	{
		m_List = std::move(list);
		MarkDirty();
	}

	std::vector<std::pair<int, const char*>>& CommandListSelect::GetList()
	{
		return m_List;
	}
}