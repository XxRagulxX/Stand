#pragma once
#include "Commands/Widgets/CommandFlags.hpp"

#include <cstdint>

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

		[[nodiscard]] bool isListSelect() const noexcept
		{
			return type == COMMAND_LIST_SELECT;
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

		virtual void onFocus() {}
		virtual void onBlur() {}
	};
}
