#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "World/Self.hpp"

namespace Stand
{
	class CommandGod : public CommandToggle
	{
	public:
		explicit CommandGod(CommandList* parent) :
		    CommandToggle(parent, LIT("Immortality"), CMDNAMES("godmode", "immortality"), LIT("Makes your character unable to die."))
		{
		}

		void onEnable(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetInvincible(true);

			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);

			if (auto ped = Stand::Self::GetPed())
				ped.SetInvincible(false);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			ped.SetInvincible(!ped.IsDead());
		}

		~CommandGod() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

