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
	class CommandNoCollision : public CommandToggle
	{
	public:
		explicit CommandNoCollision(CommandList* const parent)
			: CommandToggle(parent, LIT("No Collision"), CMDNAMES("nocollision"), LIT("Pass through everything."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				ENTITY::SET_ENTITY_COLLISION(ped.GetHandle(), true, false);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(ped.GetHandle(), false, false);
		}

		~CommandNoCollision() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
