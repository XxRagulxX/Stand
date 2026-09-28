#include "Commands/CommandListSelect.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandLegacyListSelect::OnCall()
	{
	}

	void CommandLegacyListSelect::SaveState(nlohmann::json& value)
	{
		value = m_State;
	}

	void CommandLegacyListSelect::LoadState(nlohmann::json& value)
	{
		m_State = value;
	}

	CommandLegacyListSelect::CommandLegacyListSelect(std::string name, std::string label, std::string description, std::vector<std::pair<int, const char*>> list, int def_val) :
	    CommandLegacy(name, label, description, 0),
	    m_List(list),
	    m_State(def_val)
	{
	}

	int CommandLegacyListSelect::GetState()
	{
		return m_State;
	}

	void CommandLegacyListSelect::SetState(int state)
	{
		FiberPool::queueJob([this] {
			OnChange();
		});
		m_State = state;
		MarkDirty();
	}

	void CommandLegacyListSelect::SetList(std::vector<std::pair<int, const char*>> list)
	{
		m_List = std::move(list);
		MarkDirty();
	}

	std::vector<std::pair<int, const char*>>& CommandLegacyListSelect::GetList()
	{
		return m_List;
	}
}