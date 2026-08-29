#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "Core/AbstractEntity.hpp"
#include "Commands/Widgets/CommandWithOnTickFocused.hpp"
#include "Commands/Widgets/CommandListWithOnTickAsActiveList.hpp"

namespace Stand
{
	class CommandSpawnedVehicleEntry : public CommandWithOnTickFocused<CommandListWithOnTickAsActiveList<CommandList>>
	{
	private:
		AbstractEntity entity;

	public:
		explicit CommandSpawnedVehicleEntry(CommandList* const parent, Label&& menu_name, AbstractEntity entity);

		void onTickFocused() final;
		void onTickAsActiveList() final;

		void onTickInWebViewport() final;
		void onTickInGameViewport() final;

	private:
		void drawBox();
	};
}