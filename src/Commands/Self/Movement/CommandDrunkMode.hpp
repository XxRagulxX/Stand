#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandDrunkMode : public CommandToggle
	{
	public:
		explicit CommandDrunkMode(CommandList* const parent)
			: CommandToggle(parent, LIT("Drunk Mode"), CMDNAMES("drunkmode"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			CAMERA::STOP_GAMEPLAY_CAM_SHAKING(true);
		}

		void onTick() override
		{
			CAMERA::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 0.3f);
		}

		~CommandDrunkMode() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
