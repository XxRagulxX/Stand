#include "Commands/Widgets/CommandStandWidgetsTest5.hpp"

#include "Commands/Widgets/CommandRegistry.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Menu/Hotkey.hpp"
#include "Util/Joaat.hpp"
#include "World/Self.hpp"

// CommandGod lives directly in namespace Stand (not Stand::Features) -
// matching where the user's own pasted reference example was written.
// CommandName/Label/CommandList/CommandToggle/Click/LOC()/CMDNAMES() all
// resolve unqualified only from inside namespace Stand itself (or a
// namespace nested under it) - writing this class inside
// Stand::Features instead (the original mistake here) left every one
// of those unqualified names unable to find Stand::CommandName et al.,
// since Stand::Features and Stand are unrelated sibling namespace
// trees, not parent/child.
namespace Stand
{
	namespace
	{
		// Ported from real Stand's own CommandGod - pasted by the user
		// as the exact target shape every command should end up in once
		// migrated (constructor taking CommandList* parent + LOC()
		// name/help text/CMDNAMES() aliases, onEnable()/onDisable()
		// doing the actual work). Reuses this project's own existing
		// Stand::Self::GetPed().SetInvincible() - the same native call
		// this project's own CommandGodmode.cpp (a Stand::LoopedCommand,
		// kept running unmodified and independently for now) already
		// uses - rather than reimplementing the behaviour a second way.
		// No LOC() translation database exists here (see Util/Label.hpp's
		// own comment) so LOC(...) just becomes the literal text passed
		// to it, same as real Stand's own LOC() would once resolved.
		//
		// Phase 4 addition: bound to G (Hotkey('G', ...) - VK_G and
		// ASCII 'G' are the same value on Win32, no separate VK_ constant
		// needed) purely to give CommandHotkeyDispatch
		// (Commands/Widgets/CommandHotkeyDispatch.hpp) something real to
		// prove against - press G outside the menu and this should flip
		// exactly like clicking the checkbox or the Phase 2 registry
		// button does.
		class CommandGod : public CommandToggle
		{
		public:
			explicit CommandGod(CommandList* parent) :
			    CommandToggle(parent,
			        LOC("God Mode (Stand Test)"),
			        CMDNAMES("standtest_godmode", "standtest_immortality"),
			        LOC("Makes your character unable to die."),
			        false,
			        CMDFLAGS_TOGGLE,
			        COMMANDPERM_USERONLY,
			        {Hotkey('G', false, false, false)})
			{
			}

			void onEnable(Click& click) override
			{
				if (Stand::Self::GetPed())
					Stand::Self::GetPed().SetInvincible(true);
			}

			void onDisable(Click& click) override
			{
				if (Stand::Self::GetPed())
					Stand::Self::GetPed().SetInvincible(false);
			}
		};

		// Phase 2 proof-of-concept for CommandRegistry (Commands/Widgets/
		// CommandRegistry.hpp) - this button does NOT hold a pointer to
		// CommandGod above (it doesn't even know it lives in the same
		// file); it looks CommandGod up purely by one of its aliases,
		// hashed the exact same way Stand::Commands::GetCommand<T>()
		// already does for the legacy system. Proves a command
		// constructed anywhere in the (eventually many) migrated files
		// can find another one by name alone, the same cross-referencing
		// shape Commands/Widgets/CommandPositionPick.hpp already relies
		// on for the legacy "freecam" toggle.
		class CommandGodToggleViaRegistry : public CommandPhysical
		{
		public:
			explicit CommandGodToggleViaRegistry(CommandList* parent) :
			    CommandPhysical(COMMAND_ACTION,
			        parent,
			        LOC("Toggle God Mode (via Registry)"),
			        CMDNAMES("standtest_togglegodviaregistry"),
			        LOC("Looks up \"standtest_godmode\" through CommandRegistry and clicks it - proves the registry, not a held pointer."))
			{
			}

			void onClick(Click& click) override
			{
				if (auto* god = CommandRegistry::GetCommand<CommandToggle>(Stand::Joaat("standtest_godmode")))
					god->onClick(click);
			}
		};
	}
}

namespace Stand::Features
{
	Stand::CommandList& GetStandTreeTestRoot()
	{
		static Stand::CommandList root{nullptr, LOC("Stand Tree Test")};
		// Built lazily, once, the first time this is reached - a plain
		// file-scope static Stand::CommandList would try to construct
		// its own child at static-init time via createChild<CommandGod>(),
		// before FiberPool/other statics this eventually touches are
		// necessarily ready; deferring the child's own construction to
		// first real use (same "lazy static" idiom this project's own
		// Commands::GetInstance()/Notifications::GetInstance() already
		// use) avoids that ordering risk.
		//
		// Stand::CommandGod, not just CommandGod - it's declared inside
		// an anonymous namespace nested under namespace Stand above, not
		// under Stand::Features, so it needs that qualifier from here.
		static bool initialized = [] {
			root.createChild<Stand::CommandGod>();
			root.createChild<Stand::CommandGodToggleViaRegistry>();
			return true;
		}();
		(void)initialized;

		return root;
	}
}
