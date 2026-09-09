#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandInfiniteStamina : public CommandToggle
	{
	public:
		explicit CommandInfiniteStamina(CommandList* const parent)
			: CommandToggle(parent, LIT("Infinite Stamina"), { CMDNAME("infinitestamina") }, LIT("Never run out of breath while sprinting or swimming."))
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
			auto player = Stand::Self::GetPlayer();
			if (!player)
				return;
			PLAYER::RESTORE_PLAYER_STAMINA(player.GetId(), 1.0f);
		}

		~CommandInfiniteStamina() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
