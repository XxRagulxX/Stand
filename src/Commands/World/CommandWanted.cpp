#include "Commands/CommandLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	static CommandSliderLegacy _WantedSlider{"wantedslider", "Wanted Slider", "Wanted level to set/freeze", 0, 5, 0};

	class ClearWanted : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Self::GetPlayer().SetWantedLevel(0);
		}
	};

	class SetWanted : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Self::GetPlayer().SetWantedLevel(_WantedSlider.GetState());
		}
	};

	class NeverWanted : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			Self::GetPlayer().SetWantedLevel(0);
			PLAYER::SET_MAX_WANTED_LEVEL(0);
		}

		virtual void OnDisable() override
		{
			PLAYER::SET_MAX_WANTED_LEVEL(6);
		}
	};

	class FreezeWanted : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (Self::GetPlayer().GetWantedLevel() != _WantedSlider.GetState())
				Self::GetPlayer().SetWantedLevel(_WantedSlider.GetState());
			PLAYER::SET_MAX_WANTED_LEVEL(_WantedSlider.GetState());
		}
	};

	static ClearWanted _ClearWanted{"clearwanted", "Clear Wanted", "Clears your wanted level"};
	static SetWanted _SetWanted{"setwanted", "Set Wanted", "Sets your wanted level to the desired level"};
	static NeverWanted _NeverWanted{"neverwanted", "Never Wanted", "Never gain a wanted level"};
	static FreezeWanted _FreezeWanted{"freezewanted", "Freeze Wanted", "Freeze your wanted level to the desired level"};
}