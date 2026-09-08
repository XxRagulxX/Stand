#pragma once
#include "Util/Joaat.hpp"

#include <unordered_map>

// Real Stand has no hash-keyed command registry at all - CommandSearchMenu
// (Commands/Widgets/CommandSearchMenu.cpp on origin/stand-reference) finds
// commands by recursively walking g_gui.root_list and string-matching
// menu_name/help_text, and a command that wants another specific command
// by name (e.g. CommandPositionSelection reaching for "freecam") just
// walks a known parent list directly. This project already has one
// working, pervasive convention for "find a command elsewhere in the
// codebase by name" instead - Stand::Commands::GetCommand<T>(joaat_t)
// is a flat hash map keyed by Stand::Joaat(name), used everywhere
// (e.g. Commands/Widgets/CommandPositionPick.hpp's own
// Commands::GetCommand<CommandToggle>("freecam"_J)) - so newly-migrated
// Stand commands get the same shape here rather than forcing every
// future cross-reference to hold a raw pointer or walk a tree to find
// one: every Stand::CommandIssuable auto-registers each of its own
// command_names (its aliases) into this flat map, hashed the same way,
// the moment it's constructed, and removes itself on destruction. A
// command's tree position (which CommandList, if any, owns it) is
// unrelated to whether it can be found here - exactly like the legacy
// registry, this is a flat index over every constructed command, not a
// tree walk.
//
// Known, disclosed gap: registration happens once, at construction time,
// from whatever command_names held then. CommandIssuable::
// addSuffixToCommandNames() (used nowhere in this codebase yet) mutates
// command_names afterward without re-registering - a command that grew
// new aliases that way wouldn't be findable under them here. Revisit if
// a future ported command actually needs that.
namespace Stand
{
	class CommandIssuable;

	class CommandRegistry
	{
	public:
		static void Add(CommandIssuable* command);
		static void Remove(CommandIssuable* command);

		template<typename T = CommandIssuable>
		static T* GetCommand(Stand::joaat_t hash)
		{
			return reinterpret_cast<T*>(GetInstance().GetCommandImpl(hash));
		}

		// Enumeration, not just by-hash lookup - MenuCommandConsole needs
		// this to live-search every registered Stand command by name
		// prefix (real Stand has no hash-keyed registry at all to expose
		// this way - see this file's own top comment; this project's own
		// legacy Stand::Commands::GetCommands() already exposes its map
		// the same way, for the same reason).
		static const std::unordered_map<Stand::joaat_t, CommandIssuable*>& GetCommands()
		{
			return GetInstance().m_Commands;
		}

	private:
		std::unordered_map<Stand::joaat_t, CommandIssuable*> m_Commands;

		void AddImpl(CommandIssuable* command);
		void RemoveImpl(CommandIssuable* command);
		CommandIssuable* GetCommandImpl(Stand::joaat_t hash);

		static CommandRegistry& GetInstance()
		{
			static CommandRegistry instance{};
			return instance;
		}
	};
}
