#include "ToggleCorrelation.hpp"

#include "Menu/GUI.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	bool ToggleCorrelation::getCurrentValue(ToggleCorrelation::Type correlation)
	{
		switch (correlation)
		{
		case MENU_OPEN:
			return GUI::IsOpen();

		case ON_FOOT:
			return !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE);

		case AIMING:
			return PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()) != FALSE;

		case FREEROAM:
			return NETWORK::NETWORK_IS_SESSION_STARTED() != FALSE;

		case CHATTING:
			return false;

		case SESSION_HOST:
			return NETWORK::NETWORK_IS_HOST() != FALSE;

		default:
			return false;
		}
	}

	bool ToggleCorrelation::getCurrentValue(ToggleCorrelation::Type correlation, bool correlation_invert)
	{
		bool value = getCurrentValue(correlation);
		if (correlation_invert)
			value = !value;
		return value;
	}

	bool ToggleCorrelation::getCurrentValue() const
	{
		return getCurrentValue(type, invert);
	}

	bool ToggleCorrelation::isActive() const
	{
		return type != NONE;
	}

	std::string ToggleCorrelation::getState() const
	{
		static constexpr const char* kConditionNames[] = {
			"", "Menu Open", "On Foot", "Aiming", "Freeroam", "Chatting", "Session Host"
		};
		const char* cond = (type < _NUM_TOGGLE_CORRELATIONS) ? kConditionNames[type] : "";
		return std::string(invert ? "Off" : "On").append(" while ").append(cond);
	}
}
