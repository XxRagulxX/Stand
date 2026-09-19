#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandNoSpread : public CommandToggle
	{
		int m_savedAccuracy{100};

	public:
		explicit CommandNoSpread(CommandList* const parent)
			: CommandToggle(parent, LIT("No Spread"), CMDNAMES("nospread"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
				m_savedAccuracy = PED::GET_PED_ACCURACY(ped.GetHandle());
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_ACCURACY(ped.GetHandle(), m_savedAccuracy);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_ACCURACY(ped.GetHandle(), 100);
		}

		~CommandNoSpread() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
