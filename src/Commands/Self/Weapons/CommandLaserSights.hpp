#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandLaserSights : public CommandToggle
	{
	public:
		explicit CommandLaserSights(CommandList* const parent)
			: CommandToggle(parent, LIT("Laser Sights"), CMDNAMES("lasersights"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			WEAPON::ENABLE_LASER_SIGHT_RENDERING(FALSE);
		}

		void onTick() override
		{
			WEAPON::ENABLE_LASER_SIGHT_RENDERING(TRUE);
		}

		~CommandLaserSights() override
		{
			CommandTickDispatch::RemoveCommand(this);
			WEAPON::ENABLE_LASER_SIGHT_RENDERING(FALSE);
		}
	};
}
