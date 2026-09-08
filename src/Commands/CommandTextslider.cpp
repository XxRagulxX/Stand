#include "Commands/CommandTextslider.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandTextslider::OnCall()
	{
	}

	void CommandTextslider::SaveState(nlohmann::json& value)
	{
		value = m_Index;
	}

	void CommandTextslider::LoadState(nlohmann::json& value)
	{
		m_Index = value;
	}

	CommandTextslider::CommandTextslider(std::string name, std::string label, std::string description, std::vector<std::string> options, int def_index) :
	    CommandLegacy(name, label, description, 0),
	    m_Options(std::move(options)),
	    m_Index(def_index)
	{
	}

	int CommandTextslider::GetIndex()
	{
		return m_Index;
	}

	void CommandTextslider::SetIndex(int index)
	{
		if (m_Options.empty())
			return;

		FiberPool::queueJob([this] {
			OnChange();
		});
		m_Index = index;
		MarkDirty();
	}

	void CommandTextslider::Cycle(int direction)
	{
		if (m_Options.empty())
			return;

		const auto count = static_cast<int>(m_Options.size());
		auto index = (m_Index + count + direction) % count;
		SetIndex(index);
	}

	const std::string& CommandTextslider::GetCurrentOption() const
	{
		static const std::string empty;
		if (m_Options.empty() || m_Index < 0 || m_Index >= static_cast<int>(m_Options.size()))
			return empty;

		return m_Options[m_Index];
	}

	const std::vector<std::string>& CommandTextslider::GetOptions() const
	{
		return m_Options;
	}
}
