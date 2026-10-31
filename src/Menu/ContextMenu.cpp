#include "Menu/ContextMenu.hpp"

#include "Commands/Context/CommandCtxAddress.hpp"
#include "Commands/Context/CommandCtxHotkeys.hpp"
#include "Commands/Context/CommandCtxStar.hpp"
#include "Commands/Context/CommandCtxToggleCorrelation.hpp"
#include "Commands/Context/CommandCtxToggleCorrelationInvert.hpp"
#include "Commands/Context/CommandQuickCtx.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/StandPort/Tutorial.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/StandPort/CommandLambdaAction.hpp"
#include "Scripting/ExecCtx.hpp"

#include "lib/soup/base.hpp"

namespace Stand
{
	Command* ContextMenu::getTarget() noexcept
	{
		return g_gui.getCurrentMenuFocus();
	}

	CommandPhysical* ContextMenu::getTargetPhysical() noexcept
	{
		return g_gui.getCurrentMenuFocusPhysical();
	}

	bool ContextMenu::isAvailable() noexcept
	{
		return Tutorial::state >= TUT_DONE;
	}

	void ContextMenu::toggleIfAvailable(ThreadContext thread_context)
	{
		if (isAvailable())
		{
			toggle(thread_context);
		}
	}

	void ContextMenu::toggle(ThreadContext thread_context)
	{
		if (isOpen())
		{
			close(thread_context);
		}
		else
		{
			open(thread_context);
		}
		g_gui.sfxOpenClose(thread_context, isOpen());
	}

	bool ContextMenu::isOpen() noexcept
	{
		return root.operator bool();
	}

	void ContextMenu::close(ThreadContext thread_context)
	{
		g_gui.updateFocus(thread_context, TELEPORT);
		root.reset();
		if (!g_gui.m_active_list.empty())
		{
			CommandList* list = g_gui.m_active_list.back();
			list->fixCursorAndOffset();
			list->onActiveListUpdate();
		}
	}

	void ContextMenu::open(ThreadContext thread_context)
	{
		if (auto* const target = getTargetPhysical())
		{
			root = std::make_unique<CommandList>(nullptr, Label(target->menu_name));
			view = root.get();

			auto stateCommand = target->getStateCommand();
			if (stateCommand != nullptr)
			{
				if (stateCommand == target && target->type == COMMAND_TOGGLE)
				{
					auto autostate = root->createChild<CommandCtxToggleCorrelation>((CommandToggle*)target);
					root->createChild<CommandCtxToggleCorrelationInvert>((CommandToggle*)target, autostate);
				}
				if (g_gui.active_profile.isInitialised()
					&& !(stateCommand->flags & CMDFLAG_TEMPORARY)
					&& stateCommand->supportsSavedState()
					)
				{
					if (!g_gui.isUsingAutosaveState())
					{
						root->createChild<CommandLambdaAction>(LOC("SSTATE"), CMDNAMES(), NOLABEL, [](Click& click)
						{
							doSaveState(click);
						}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->save_state->hotkeys : std::vector<Hotkey>{});
					}
					root->createChild<CommandLambdaAction>(LOC("LSTATE"), CMDNAMES(), NOLABEL, [](Click& click)
					{
						doLoadState(click);
					}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->load_state->hotkeys : std::vector<Hotkey>{});
				}
				root->createChild<CommandLambdaAction>(LOC("GODFLT"), CMDNAMES(), NOLABEL, [](Click& click)
				{
					doApplyDefaultState(click);
				}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->apply_default_state->hotkeys : std::vector<Hotkey>{});
			}

			if (target->isListNonAction())
			{
				root->createChild<CommandLambdaAction>(LOC("RGODFLT"), CMDNAMES(), NOLABEL, [](Click& click)
				{
					doApplyDefaultStateToChildren(click);
				}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->apply_default_state_to_children->hotkeys : std::vector<Hotkey>{});
			}

			if (target->isT<CommandSlider>())
			{
				root->createChild<CommandLambdaAction>(LOC("CTX_MIN"), CMDNAMES(), NOLABEL, [](Click& click)
				{
					doMin(click);
				}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->min->hotkeys : std::vector<Hotkey>{});
				root->createChild<CommandLambdaAction>(LOC("CTX_MAX"), CMDNAMES(), NOLABEL, [](Click& click)
				{
					doMax(click);
				}, CMDFLAGS_ACTION, COMMANDPERM_USERONLY, CommandQuickCtx::instance ? CommandQuickCtx::instance->max->hotkeys : std::vector<Hotkey>{});
			}

			if (!(target->flags & CMDFLAG_TEMPORARY)
				&& target->canBeResolved()
				)
			{
				root->createChild<CommandCtxHotkeys>();

				CommandList* stand_tab = g_gui.getStandTab();
				if (stand_tab != nullptr)
				{
					Command* stars = stand_tab->recursivelyResolveChildByMenuName(LOC("CMDSTARS"));
					if (stars != nullptr)
					{
						root->createChild<CommandCtxStar>(stars);
					}
				}
			}

			root->createChild<CommandCtxAddress>();

			SOUP_IF_UNLIKELY (Tutorial::state == TUT_CTX)
			{
				Tutorial::setState(TUT_DONE);
				g_gui.user_understands_context_menu = true;
				g_gui.saveTutorialFlags();
			}

			g_gui.updateActiveFocus(thread_context, TELEPORT, target);
			g_gui.m_active_list.push_back(view);
		}

		if (!g_gui.m_active_list.empty())
		{
			CommandList* list = g_gui.m_active_list.back();
			list->onActiveListUpdate();
		}
	}

	CommandCtxHotkeys* ContextMenu::getHotkeysList()
	{
		return (CommandCtxHotkeys*)root->resolveChildByMenuName(LOC("HOTKEYS"));
	}

	void ContextMenu::openIntoHotkeysList(ThreadContext thread_context)
	{
		open(thread_context);
		CommandCtxHotkeys* const hotkeys = getHotkeysList();
		hotkeys->open(thread_context);
		hotkeys->close_context_menu_on_back = true;
	}

	void ContextMenu::doSaveState(Click& click)
	{
		if (!g_gui.active_profile.isInitialised())
		{
			return;
		}
		if (g_gui.isUsingAutosaveState())
		{
			click.setResponse(LOC("AUTOSAVE2_T"));
			return;
		}
		if (auto targetPhysical = getTargetPhysical())
		{
			if (auto stateCommand = targetPhysical->getStateCommand())
			{
				if (!(stateCommand->flags & CMDFLAG_TEMPORARY)
					&& stateCommand->supportsSavedState()
					)
				{
					auto state = stateCommand->getState();
					if (state != stateCommand->getDefaultState())
					{
						g_gui.active_profile.data[stateCommand->getPathConfig()] = std::move(state);
						g_gui.active_profile.save();
					}
					else
					{
						auto ent = g_gui.active_profile.data.find(stateCommand->getPathConfig());
						if (ent != g_gui.active_profile.data.end())
						{
							g_gui.active_profile.data.erase(ent);
							g_gui.active_profile.save();
						}
					}
					return;
				}
			}
		}
	}

	void ContextMenu::doLoadState(Click& click)
	{
		if (!g_gui.active_profile.isInitialised())
		{
			return;
		}
		if (auto targetPhysical = getTargetPhysical())
		{
			if (auto stateCommand = targetPhysical->getStateCommand())
			{
				if (!(stateCommand->flags & CMDFLAG_TEMPORARY)
					&& stateCommand->supportsSavedState()
					)
				{
					stateCommand->loadState(click.type);
					return;
				}
			}
		}
	}

	void ContextMenu::doApplyDefaultState(Click& click)
	{
		if (auto targetPhysical = getTargetPhysical())
		{
			if (auto stateCommand = targetPhysical->getStateCommand())
			{
				ExecCtx::get().ensureScript([stateCommand]
				{
					stateCommand->applyDefaultState();
				});
				return;
			}
		}
	}

	void ContextMenu::doApplyDefaultStateToChildren(Click& click)
	{
		if (auto targetPhysical = getTargetPhysical())
		{
			if (targetPhysical->isT<CommandList>())
			{
				ExecCtx::get().ensureScript([targetPhysical]
				{
					targetPhysical->as<CommandList>()->recursivelyApplyDefaultState();
				});
				return;
			}
		}
	}

	void ContextMenu::doMin(Click& click)
	{
		if (auto targetPhysical = getTargetPhysical())
		{
			if (targetPhysical->isT<CommandSlider>())
			{
				auto focus = targetPhysical->as<CommandSlider>();
				focus->setValue(click, focus->min_value);
				return;
			}
		}
	}

	void ContextMenu::doMax(Click& click)
	{
		if (auto targetPhysical = getTargetPhysical())
		{
			if (targetPhysical->isT<CommandSlider>())
			{
				auto focus = targetPhysical->as<CommandSlider>();
				focus->setValue(click, focus->max_value);
				return;
			}
		}
	}
}
