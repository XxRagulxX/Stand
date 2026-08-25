#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListWithOnTickAsActiveList.hpp"
#include "Commands/Widgets/CommandWithOnTickFocused.hpp"

#include "Core/AbstractEntity.hpp"
#include "Util/Box.hpp"
#include "Commands/Extra/CommandLambdaActionScript.hpp"
#include "Commands/Weapons/CommandLambdaListSelectWeapons.hpp"
#include "Rendering/DrawUtil3d.hpp"
#include "Game/natives.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandBodyguardMember final : public CommandWithOnTickFocused<CommandListWithOnTickAsActiveList<CommandList>>
	{
	public:
		AbstractEntity entity;
		CommandListSelect* primary;
		CommandListSelect* secondary;

		explicit CommandBodyguardMember(CommandList* const parent, AbstractEntity& entity)
			: CommandWithOnTickFocused(parent, entity.getModel().getLabel(), {}, NOLABEL, CMDFLAGS_LIST | CMDFLAG_TEMPORARY, COMMAND_LIST_CUSTOM_SPECIAL_MEANING), entity(entity)
		{
			this->createChild<CommandLambdaActionScript>(LOC("DEL"), {}, NOLABEL, [this](const Click& click)
			{
				this->yeet();
			});

			this->createChild<CommandLambdaActionScript>(LOC("PLYSMMN"), {}, NOLABEL, [this](const Click& click)
			{
				this->entity.requestControl(ATSTRINGHASH("call"), [this](AbstractEntity& entity)
				{
					entity.setPos(g_player_ped.getPos());
				});
			});

			primary = this->createChild<CommandLambdaListSelectWeapons>(LOC("PRIM_WEAP"), {}, NOLABEL, BodyguardMgr::primary_weapon->value, [this](Hash value, Hash last_value, Click& click)
			{
				click.ensureScriptThread([this, value, last_value]
				{
					this->entity.removeWeapons({ last_value });
					this->entity.giveWeapons({ value }, true);
				});
			});

			secondary = this->createChild<CommandLambdaListSelectWeapons>(LOC("SCND_WEAP"), {}, NOLABEL, BodyguardMgr::secondary_weapon->value, [this](Hash value, Hash last_value, Click& click)
			{
				click.ensureScriptThread([this, value, last_value]
				{
					this->entity.removeWeapons({ last_value });
					this->entity.giveWeapons({ value }, true);
				});
			});
		}

		~CommandBodyguardMember()
		{
			entity.removeFromPlaneOfExistence();
		}

		void onTickInGameViewport() final
		{
			SOUP_IF_UNLIKELY (!entity.exists())
			{
				yeet();
				return;
			}
			return CommandWithOnTickFocused::onTickInGameViewport();
		}

		void onTickInWebViewport() final
		{
			return onTickInWebViewportImplRedirect();
		}

		void onTickFocused() final
		{
			drawBox();
		}

		void onTickAsActiveList() final
		{
			drawBox();
		}

	private:
		void drawBox()
		{
			this->entity.getBox().drawLines(int(g_renderer.arColour.x * 255.0f), int(g_renderer.arColour.y * 255.0f), int(g_renderer.arColour.z * 255.0f), int(g_renderer.arColour.w * 255.0f));
		}
		
		void yeet()
		{
			beGone();
			BodyguardMgr::postDeleteMember();
		}
	};
}