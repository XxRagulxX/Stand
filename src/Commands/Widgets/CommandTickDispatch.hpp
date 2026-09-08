#pragma once
#include <vector>

// Real Stand's CommandPhysical carries a generic tick-event-handler
// registration tied to its own TickMgr (see CommandPhysical.hpp's own
// class comment - not ported) that any command can hook into to run
// logic every game tick. This project has nothing like that yet - the
// first real feature migrated onto the Stand tree (Self > Immortality,
// see Commands/Self/CommandGod.hpp) needs it: real Stand's own onEnable()
// just flips a flag (g_gui.godmode) and lets some other, unported part
// of its renderer apply invincibility every tick elsewhere; this
// project's own equivalent (Self::GetPed().SetInvincible()) has no such
// "elsewhere" to live in, and the legacy Godmode.cpp this replaces
// needed exactly this same continuous per-tick enforcement (forcing
// invincibility back off the instant the ped is actually dead, via its
// own LoopedCommand::OnTick() - see Commands/LoopedCommand.hpp) for the
// same reason.
//
// Deliberately opt-in and NOT wired into CommandPhysical's own
// constructor/destructor (unlike CommandStateSerializer/
// CommandHotkeyDispatch, which register every qualifying command
// automatically) - most commands never need a per-tick hook at all, and
// CommandPhysical::onTick() defaults to an empty no-op (see its own doc
// comment), so a command that wants ticking calls AddCommand(this)/
// RemoveCommand(this) itself, typically from its own onEnable()/
// onDisable() (only while actually turned on - see CommandGod.hpp)
// rather than for its whole lifetime.
//
// Runs unconditionally every script tick, unlike CommandHotkeyDispatch's
// own RunScriptImpl() (which gates on foreground window/pause menu/not-
// typing) - this is about continuously enforcing world state (e.g.
// invincibility), not reacting to a keypress, so there's no reason to
// skip it while the game window isn't focused or the pause menu's up
// (matches the legacy system's own Commands::RunLoopedCommandsImpl(),
// which has no such gating either).
namespace Stand
{
	class CommandPhysical;

	class CommandTickDispatch
	{
	public:
		static void AddCommand(CommandPhysical* command)
		{
			GetInstance().AddCommandImpl(command);
		}

		static void RemoveCommand(CommandPhysical* command)
		{
			GetInstance().RemoveCommandImpl(command);
		}

		static void RunScript();

	private:
		std::vector<CommandPhysical*> m_Commands;

		void AddCommandImpl(CommandPhysical* command);
		void RemoveCommandImpl(CommandPhysical* command);
		void RunScriptImpl();

		static CommandTickDispatch& GetInstance()
		{
			static CommandTickDispatch instance{};
			return instance;
		}
	};
}
