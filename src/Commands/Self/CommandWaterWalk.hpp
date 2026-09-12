#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWaterWalk : public CommandToggle
	{
	public:
		explicit CommandWaterWalk(CommandList* const parent)
			: CommandToggle(parent, LIT("Water Walk"), CMDNAMES("waterwalk"), LIT("Walk on the surface of water."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!Stand::Self::GetPed())
				return;

			if (!ENTITY::IS_ENTITY_IN_WATER(Stand::Self::GetPed().GetHandle()))
				return;

			const auto pos = Stand::Self::GetPed().GetPosition();
			float waterHeight = 0.0f;
			if (!WATER::GET_WATER_HEIGHT(pos.x, pos.y, pos.z, &waterHeight))
				return;

			if (pos.z < waterHeight)
				Stand::Self::GetPed().SetPosition({pos.x, pos.y, waterHeight});
		}

		~CommandWaterWalk() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
