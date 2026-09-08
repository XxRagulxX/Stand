#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	void CommandSliderFloatLegacy::OnCall()
	{
	}

	void CommandSliderFloatLegacy::SaveState(nlohmann::json& value)
	{
		value = m_State;
	}

	void CommandSliderFloatLegacy::LoadState(nlohmann::json& value)
	{
		m_State = value;
	}

	CommandSliderFloatLegacy::CommandSliderFloatLegacy(std::string name, std::string label, std::string description, std::optional<float> min, std::optional<float> max, float def_val) :
	    CommandLegacy(name, label, description, 0),
	    m_Min(min),
	    m_Max(max),
	    m_State(def_val)
	{
	}

	float CommandSliderFloatLegacy::GetState()
	{
		return m_State;
	}

	void CommandSliderFloatLegacy::SetState(float state)
	{
		FiberPool::queueJob([this] {
			OnChange();
		});
		m_State = state;
		MarkDirty();
	}

	std::optional<float> CommandSliderFloatLegacy::GetMinimum()
	{
		if (m_DynamicRange)
			return m_DynamicRange().first;

		return m_Min;
	}

	std::optional<float> CommandSliderFloatLegacy::GetMaximum()
	{
		if (m_DynamicRange)
			return m_DynamicRange().second;

		return m_Max;
	}

	void CommandSliderFloatLegacy::SetDynamicRange(std::function<std::pair<float, float>()> dynamicRange)
	{
		m_DynamicRange = std::move(dynamicRange);
	}
}