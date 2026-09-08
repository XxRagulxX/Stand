#pragma once
#include "Commands/CommandSliderLegacy.hpp"

#include <functional>
#include <optional>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandLambdaSlider (a template over
	// which slider base it wraps there - this project only has one
	// integer slider base, CommandSlider, so no template needed here) - see
	// CommandLambdaToggle.hpp's own doc comment for why this whole file exists.
	// Callback receives the new value directly (CommandSlider::OnChange()
	// already runs after m_State is updated - see CommandSlider::SetState())
	// rather than needing a separate GetState() call.
	class CommandLambdaSlider : public CommandSliderLegacy
	{
	public:
		CommandLambdaSlider(std::string name,
		    std::string label,
		    std::string description,
		    std::optional<int> min,
		    std::optional<int> max,
		    int def_val,
		    std::function<void(int)> onChange) :
		    CommandSliderLegacy(std::move(name), std::move(label), std::move(description), min, max, def_val),
		    m_OnChange(std::move(onChange))
		{
		}

	protected:
		void OnChange() override
		{
			if (m_OnChange)
				m_OnChange(GetState());
		}

	private:
		std::function<void(int)> m_OnChange;
	};
}
