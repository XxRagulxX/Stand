#pragma once
#include "Commands/Widgets/CommandFlags.hpp"

#include <cstdint>

// Real Stand's Command also carries a Lua binding (CommandLuaData -
// unrelated to this project's own, separate Lua API), a soup::WeakRef/
// TransientToken pair for safe weak references, chat-command address/
// path resolution (getPathConfig/getPathEn/getPathWeb/getTab...), a
// warning-dialog confirm system (showWarning/warnSelfHarm), and the
// hotkeys-list UI (openHotkeysList) - none of which this project has or
// is building yet. Kept only the shape every widget (CommandToggle,
// CommandSlider, ...) actually needs to exist in a tree and be told
// apart by type: the CommandType bitfield-flagged enum, parent/flags,
// and the isX() type tests built on it.
namespace Stand
{
	class CommandList;
	class CommandPhysical;

	enum CommandType : uint8_t
	{
		COMMAND_FULLTYPEFLAG = 0b1110000,
		COMMAND_FLAG_LIST = 0b1000000,
		COMMAND_FLAG_LIST_ACTION = 0b1100000,
		COMMAND_FLAG_TOGGLE = 0b0010000,
		COMMAND_FLAG_SLIDER = 0b0100000,

		COMMAND_LINK = 0,
		COMMAND_ISSUABLE,

		COMMAND_FIRST_PHYSICAL,
		COMMAND_ACTION = COMMAND_FIRST_PHYSICAL,

		COMMAND_LIST = COMMAND_FLAG_LIST,

		COMMAND_LIST_ACTION = COMMAND_FLAG_LIST_ACTION,
		COMMAND_LIST_SELECT,

		COMMAND_TOGGLE = COMMAND_FLAG_TOGGLE,

		COMMAND_SLIDER = COMMAND_FLAG_SLIDER,
		COMMAND_SLIDER_FLOAT,
	};

	class Command
	{
	public:
		CommandList* parent;
		const CommandType type;
		commandflags_t flags;

		explicit Command(CommandType type, CommandList* parent, commandflags_t flags = 0) :
		    parent(parent),
		    type(type),
		    flags(flags)
		{
		}

		virtual ~Command() = default;

		// reinterpret_cast, not static_cast, deliberately - matches real
		// Stand's own as<T>(): lets this compile with T only forward-
		// declared (CommandPhysical, at the point getPhysical() below
		// needs it) since the real object is guaranteed to already be a
		// T whenever a caller has actually checked isT<T>()/isPhysical()
		// first.
		template<typename T>
		[[nodiscard]] T* as() noexcept
		{
			return reinterpret_cast<T*>(this);
		}

		template<typename T>
		[[nodiscard]] const T* as() const noexcept
		{
			return reinterpret_cast<const T*>(this);
		}

		[[nodiscard]] bool isLink() const noexcept
		{
			return type == COMMAND_LINK;
		}

		[[nodiscard]] bool isIssuable() const noexcept
		{
			return !isLink();
		}

		[[nodiscard]] bool isPhysical() const noexcept
		{
			return type >= COMMAND_FIRST_PHYSICAL;
		}

		[[nodiscard]] bool isList() const noexcept
		{
			return (type & COMMAND_FLAG_LIST) != 0;
		}

		[[nodiscard]] bool isListAction() const noexcept
		{
			return (type & COMMAND_FULLTYPEFLAG) == COMMAND_FLAG_LIST_ACTION;
		}

		[[nodiscard]] bool isToggle() const noexcept
		{
			return (type & COMMAND_FULLTYPEFLAG) == COMMAND_FLAG_TOGGLE;
		}

		[[nodiscard]] bool isSlider() const noexcept
		{
			return (type & COMMAND_FULLTYPEFLAG) == COMMAND_FLAG_SLIDER;
		}

		[[nodiscard]] bool isConcealed() const noexcept
		{
			return (flags & CMDFLAG_CONCEALED) != 0;
		}

		[[nodiscard]] CommandPhysical* getPhysical() noexcept;
		[[nodiscard]] const CommandPhysical* getPhysical() const noexcept;
	};
}
