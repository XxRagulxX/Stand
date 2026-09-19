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
	class CommandTennisMode : public CommandToggle
	{
	public:
		explicit CommandTennisMode(CommandList* const parent)
			: CommandToggle(parent, LIT("Tennis Mode"), CMDNAMES("tennismode"), LIT("Sprint like you're playing tennis."))
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
				MISC::ENABLE_TENNIS_MODE(ped.GetHandle(), false, false);
		}

		void onTick() override
		{
			if (!Stand::Self::GetPed())
				return;
			const auto handle = Stand::Self::GetPed().GetHandle();
			if (static_cast<bool>(MISC::IS_TENNIS_MODE(handle)) != m_on)
				MISC::ENABLE_TENNIS_MODE(handle, m_on, m_on);
		}

		~CommandTennisMode() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
