#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRegenerationRate : public CommandSlider
	{
	public:
		explicit CommandRegenerationRate(CommandList* const parent)
			: CommandSlider(parent, LIT("Regeneration Rate"), { CMDNAME("regenrate"), CMDNAME("regenerationrate") }, LIT("Controls how fast your health regenerates. 100 = normal rate."), 0, 1000, 100, 10)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			if (value == default_value)
				return "Don't Override";
			if (value == 0)
				return "Disabled";
			return std::to_string(value) + "%";
		}

		void onChange(Click& click, int prev_value) final
		{
			if (value == default_value)
			{
				CommandTickDispatch::RemoveCommand(this);
				auto player = Stand::Self::GetPlayer();
				PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player.GetId(), 1.0f);
				PLAYER::SET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(player.GetId(), 1.0f);
			}
			else
			{
				CommandTickDispatch::AddCommand(this);
			}
		}

		void onTick() override
		{
			auto player = Stand::Self::GetPlayer();
			if (value == 0)
			{
				PLAYER::DISABLE_PLAYER_HEALTH_RECHARGE(player.GetId());
			}
			else
			{
				const float multiplier = static_cast<float>(value) / 100.0f;
				PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player.GetId(), multiplier);
				PLAYER::SET_PLAYER_HEALTH_RECHARGE_MAX_PERCENT(player.GetId(), 1.0f);
			}
		}

		~CommandRegenerationRate() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
