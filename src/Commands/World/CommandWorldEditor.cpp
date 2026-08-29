#include "Commands/World/CommandWorldEditor.hpp"

#include "lib/soup/gmBox.hpp"
#include "lib/soup/Ray.hpp"

#include "Core/AbstractEntity.hpp"
#include "Network/Auth.hpp"
#include "Util/Box.hpp"
#include "Menu/ButtonInstructions.hpp"
#include "Commands/Widgets/Commandbox.hpp"
#include "Core/FiberPool.hpp"
#include "Core/tbFreecam.hpp"
#include "Game/gta_input.hpp"
#include "Rendering/Gui.hpp"
#include "Core/input.hpp"
#include "Game/natives.hpp"
#include "Core/tbPositionSelection.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/tbReticle.hpp"
#include "Core/TpUtil.hpp"
#include "Util/Util.hpp"

#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Extra/CommandLambdaActionScript.hpp"
#include "Commands/World/CommandObjectSpawner.hpp"
#include "Commands/World/CommandSpawnedObject.hpp"
#include "Commands/World/CommandWorldEditorFreecam.hpp"

namespace Stand
{
	CommandWorldEditor::CommandWorldEditor(CommandList* parent)
		: CommandList(parent, LOC("EDTR"), { CMDNAME("editor") }, NOLABEL, 0, COMMAND_LIST_CUSTOM_SPECIAL_MEANING)
	{
		this->createChild<CommandObjectSpawner>();

		{
			auto tools = this->createChild<CommandList>(LOC("TOOLS"));

			tools->createChild<CommandWorldEditorFreecam>();
			tools->createChild<CommandLambdaActionScript>(LOC("TP2LKAT"), { CMDNAME("editorteleport") }, NOLABEL, [this](const Click&)
			{
				if (active)
				{
					TpUtil::teleport(g_tb_position_selection.pos);
				}
			});
			tools->createChild<CommandLambdaActionScript>(LOC("DELSPWD"), { CMDNAME("editorclear") }, NOLABEL, [this](const Click&)
			{
				this->children.erase(this->children.begin() + spawned_offset, this->children.end());
			});
		}

		this->createChild<CommandDivider>(LOC("SPWND"));

		spawned_offset = children.size();
	}

	CommandWorldEditor::~CommandWorldEditor()
	{
		setNotActive();
	}

	void CommandWorldEditor::onClick(Click& click)
	{
		if (click.isRegularEdition())
		{
			CommandList::onClick(click);
		}
	}

	void CommandWorldEditor::onActiveListUpdate()
	{
		if (g_gui.opened && isThisOrSublistCurrentMenuList())
		{
			if (!active)
			{
				if (g_auth.license_permissions < LICPERM_REGULAR)
				{
					Util::toast(LOC("CMDNEDR"));
					do
					{
						g_gui.inputBack(TC_OTHER, false);
					} while (isThisOrSublistCurrentMenuList());
					return;
				}

				active = true;

				if (freecam)
				{
					enableFreecam();
				}
				g_tb_position_selection.enable();
				g_tb_reticle.enable();

				registerScriptTickEventHandler([this]
				{
					HANDLER_CHECK(active);
					spawned_focus = nullptr;
					if (g_gui.opened && !g_commandbox.active)
					{
						auto r = soup::Ray::withRot(CAM::GET_FINAL_RENDERED_CAM_COORD(), CAM::GET_FINAL_RENDERED_CAM_ROT(2), 10000.0f);
						float best_dist = FLT_MAX;
						soup::Vector3 best_hit;
						for (auto i = children.begin() + spawned_offset; i != children.end(); ++i)
						{
							auto ent = AbstractEntity::get((*i)->as<CommandSpawnedObject>()->handle);
							soup::gmBox bounding{ ent.getOrigin(), ent.getRot(), ent.getDimensions() };
							soup::Vector3 hit;
							if (bounding.checkRayIntersection(r, &hit))
							{
								float dist = hit.distance(r.start);
								if (dist < best_dist)
								{
									best_dist = dist;
									best_hit = hit;
									spawned_focus = (*i)->as<CommandSpawnedObject>();
								}
							}
						}
						if (spawned_focus != nullptr)
						{
							spawned_focus_hit = best_hit;

							if (!spawnable_focused)
							{
								AbstractEntity::get(spawned_focus->handle).getBox().drawLines(int(g_renderer.arColour.x * 255.0f), int(g_renderer.arColour.y * 255.0f), int(g_renderer.arColour.z * 255.0f), int(g_renderer.arColour.w * 255.0f));
								ButtonInstructions::feature_buttons |= ButtonInstructions::FOCUS_IN_MENU;
								if (Input::isContextJustPressed(INPUT_FRONTEND_RB))
								{
									spawned_focus->focus(TC_SCRIPT_NOYIELD);
								}
							}
						}
					}
					HANDLER_END;
				});
			}
		}
		else
		{
			setNotActive();
		}
	}

	void CommandWorldEditor::enableFreecam()
	{
		g_tb_freecam.ensureMovementControlsAreEnabled();
		g_tb_freecam.enable();
	}

	void CommandWorldEditor::setNotActive()
	{
		if (active)
		{
			active = false;
			FiberPool::queueJob([this]
			{
				if (freecam)
				{
					g_tb_freecam.disable();
				}
				g_tb_position_selection.disable();
				g_tb_reticle.disable();
			});
		}
	}

	void CommandWorldEditor::onSpawnableFocus() noexcept
	{
		spawnable_focused = true;
	}

	void CommandWorldEditor::onSpawnableBlur() noexcept
	{
		spawnable_focused = false;
	}
}
