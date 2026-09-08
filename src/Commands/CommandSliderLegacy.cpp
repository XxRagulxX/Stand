#include "Commands/CommandSliderLegacy.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandSliderLegacy::OnCall()
	{
	}

	void CommandSliderLegacy::SaveState(nlohmann::json& value)
	{
		value = m_State;
	}

	void CommandSliderLegacy::LoadState(nlohmann::json& value)
	{
		m_State = value;
	}

	CommandSliderLegacy::CommandSliderLegacy(std::string name, std::string label, std::string description, std::optional<int> min, std::optional<int> max, int def_val) :
	    CommandLegacy(name, label, description, 0),
	    m_Min(min),
	    m_Max(max),
	    m_State(def_val)
	{
	}

	int CommandSliderLegacy::GetState()
	{
		return m_State;
	}

	void CommandSliderLegacy::SetState(int state)
	{
		FiberPool::queueJob([this] {
			OnChange();
		});
		m_State = state;
		MarkDirty();
	}

	std::optional<int> CommandSliderLegacy::GetMinimum()
	{
		if (m_DynamicRange)
			return m_DynamicRange().first;

		return m_Min;
	}

	std::optional<int> CommandSliderLegacy::GetMaximum()
	{
		if (m_DynamicRange)
			return m_DynamicRange().second;

		return m_Max;
	}

	void CommandSliderLegacy::SetDynamicRange(std::function<std::pair<int, int>()> dynamicRange)
	{
		m_DynamicRange = std::move(dynamicRange);
	}
}