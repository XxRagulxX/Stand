#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Commands/Widgets/CommandLambdaToggle.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand::Features
{
	static CommandSliderFloatLegacy _SuperFlightSpeed{"superflight", "Super Flight",
	    "", -10000.0f, 10000.0f, 1.0f};

	static StandWidgets::CommandLambdaToggle _SuperFlightJumpToFlight{
	    "superflightjumptoflight",
	    "Button Instructions: Super Jump To Flight",
	    "",
	    [] {},
	    [] {}};

	static StandWidgets::CommandLambdaToggle _SuperFlightControls{
	    "superflightcontrols",
	    "Button Instructions: Controls",
	    "",
	    [] {},
	    [] {}};

	class SuperFlight : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		static constexpr float kPi = 3.14159265358979323846f;

		void OnTick() override
		{
			auto ped = Self::GetPed();
			if (!ped)
				return;

			if (!ENTITY::IS_ENTITY_IN_AIR(ped.GetHandle()))
				return;

			const float speed = _SuperFlightSpeed.GetState();
			const auto rot = CAMERA::GET_GAMEPLAY_CAM_ROT(2);
			const float pitch = rot.x * (kPi / 180.0f);
			const float yaw = rot.z * (kPi / 180.0f);

			ENTITY::SET_ENTITY_VELOCITY(
			    ped.GetHandle(),
			    std::cos(pitch) * std::sin(-yaw) * speed,
			    std::cos(pitch) * std::cos(-yaw) * speed,
			    std::sin(pitch) * speed);
		}
	};

	static SuperFlight _SuperFlight{"superflightmode", "Super Flight", ""};
}
