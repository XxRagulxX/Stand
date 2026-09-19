#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandNoRecoil : public CommandToggle
	{
	public:
		explicit CommandNoRecoil(CommandList* const parent)
			: CommandToggle(parent, LIT("No Recoil"), CMDNAMES("norecoil"), NOLABEL)
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
			CAMERA::STOP_GAMEPLAY_CAM_SHAKING(TRUE);
		}

		~CommandNoRecoil() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
