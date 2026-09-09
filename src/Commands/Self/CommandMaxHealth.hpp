#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandMaxHealth : public CommandSlider
	{
	public:
		explicit CommandMaxHealth(CommandList* const parent)
			: CommandSlider(parent, LIT("Maximum Health"), CMDNAMES("maxhealth", "maximumhealth"), NOLABEL, 200, 15000, 200, 25)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			return value == min_value ? "Don't Override" : std::to_string(value);
		}

		void onChange(Click& click, int prev_value) final
		{
			if (value == min_value)
			{
				CommandTickDispatch::RemoveCommand(this);
				auto ped = Stand::Self::GetPed();
				if (ped)
				{
					PED::SET_PED_MAX_HEALTH(ped.GetHandle(), 200);
					ENTITY::SET_ENTITY_MAX_HEALTH(ped.GetHandle(), 200);
					HUD::SET_MAX_HEALTH_HUD_DISPLAY(200);
				}
			}
			else
			{
				CommandTickDispatch::AddCommand(this);
			}
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped || ped.IsDead())
				return;
			PED::SET_PED_MAX_HEALTH(ped.GetHandle(), value);
			ENTITY::SET_ENTITY_MAX_HEALTH(ped.GetHandle(), value);
			HUD::SET_MAX_HEALTH_HUD_DISPLAY(value);
			if (ENTITY::GET_ENTITY_HEALTH(ped.GetHandle()) < value)
				ENTITY::SET_ENTITY_HEALTH(ped.GetHandle(), value, 0, 0);
		}

		~CommandMaxHealth() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
