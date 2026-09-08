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
					ENTITY::SET_ENTITY_MAX_HEALTH(ped.GetHandle(), 200);
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
			ENTITY::SET_ENTITY_MAX_HEALTH(ped.GetHandle(), value);
		}

		~CommandMaxHealth() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
