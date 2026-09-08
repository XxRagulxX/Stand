#pragma once
#include "Commands/Online/CommandName.hpp"
#include "Commands/Online/CommandPerm.hpp"
#include "Commands/Widgets/CommandLegacy.hpp"
#include "Commands/Widgets/CommandRegistry.hpp"
#include "Menu/Click.hpp"

#include <vector>

// Real Stand's CommandIssuable is the chat-command layer: command_names/
// perm feed its own chat-command parser and permission gating, and
// onCommand(Click&, wstring& args) is how a chat command actually parses
// its own text arguments. This project has no chat-command parser and
// isn't building one - kept command_names/perm and the CMDNAME(S)
// macros purely so a command's constructor call, copy-pasted from Stand,
// still compiles unchanged; nothing reads them for permission checks
// yet. onCommand() is a virtual with an empty default body rather than
// Stand's own pure virtual, so a ported command doesn't need a
// meaningless chat-parsing override just to satisfy the vtable.
namespace Stand
{
#if COMPACT_COMMAND_NAMES
	#define CMDNAME(x) x
	#define utf8ToCmdName(x) x
	#define cmdNameToUtf8(x) x
#else
	#define CMDNAME(x) (L##x)
	[[nodiscard]] std::wstring utf8ToCmdName(const std::string& str);
	[[nodiscard]] std::string cmdNameToUtf8(const std::wstring& str);
	#define cmdNameToUtf16(x) x
#endif

#define CMDNAMES_1(a1)       \
	std::vector<CommandName> \
	{                        \
		CMDNAME(a1)          \
	}
#define CMDNAMES_2(a1, a2)       \
	std::vector<CommandName>     \
	{                            \
		CMDNAME(a1), CMDNAME(a2) \
	}
#define CMDNAMES_3(a1, a2, a3)                \
	std::vector<CommandName>                  \
	{                                         \
		CMDNAME(a1), CMDNAME(a2), CMDNAME(a3) \
	}
#define CMDNAMES_4(a1, a2, a3, a4)                         \
	std::vector<CommandName>                               \
	{                                                      \
		CMDNAME(a1), CMDNAME(a2), CMDNAME(a3), CMDNAME(a4) \
	}
#define CMDNAMES_5(a1, a2, a3, a4, a5)                                  \
	std::vector<CommandName>                                            \
	{                                                                   \
		CMDNAME(a1), CMDNAME(a2), CMDNAME(a3), CMDNAME(a4), CMDNAME(a5) \
	}
#define CMDNAMES_PICK(_1, _2, _3, _4, _5, NAME, ...) NAME
#define CMDNAMES(...) CMDNAMES_PICK(__VA_ARGS__, CMDNAMES_5, CMDNAMES_4, CMDNAMES_3, CMDNAMES_2, CMDNAMES_1)(__VA_ARGS__)

#define CMDNAME_OBF(x) utf8ToCmdName(x)
#define CMDNAMES_OBF_1(a1)   \
	std::vector<CommandName> \
	{                        \
		CMDNAME_OBF(a1)      \
	}
#define CMDNAMES_OBF_2(a1, a2)           \
	std::vector<CommandName>             \
	{                                    \
		CMDNAME_OBF(a1), CMDNAME_OBF(a2) \
	}
#define CMDNAMES_OBF_3(a1, a2, a3)                        \
	std::vector<CommandName>                              \
	{                                                     \
		CMDNAME_OBF(a1), CMDNAME_OBF(a2), CMDNAME_OBF(a3) \
	}
#define CMDNAMES_OBF_4(a1, a2, a3, a4)                                     \
	std::vector<CommandName>                                               \
	{                                                                      \
		CMDNAME_OBF(a1), CMDNAME_OBF(a2), CMDNAME_OBF(a3), CMDNAME_OBF(a4) \
	}
#define CMDNAMES_OBF_PICK(_1, _2, _3, _4, NAME, ...) NAME
#define CMDNAMES_OBF(...) CMDNAMES_OBF_PICK(__VA_ARGS__, CMDNAMES_OBF_4, CMDNAMES_OBF_3, CMDNAMES_OBF_2, CMDNAMES_OBF_1)(__VA_ARGS__)

	class CommandIssuable : public CommandLegacy
	{
	public:
		std::vector<CommandName> command_names;
		CommandPerm perm;

		explicit CommandIssuable(CommandList* parent, std::vector<CommandName>&& command_names, CommandPerm perm = COMMANDPERM_USERONLY, commandflags_t flags = 0, CommandType type = COMMAND_ISSUABLE) :
		    CommandLegacy(type, parent, flags),
		    command_names(std::move(command_names)),
		    perm(perm)
		{
			// See CommandRegistry.hpp's own class comment - every command's
			// aliases become findable by hash the moment it's constructed.
			CommandRegistry::Add(this);
		}

		~CommandIssuable() override
		{
			CommandRegistry::Remove(this);
		}

		void addSuffixToCommandNames(CommandName&& suffix);
		void addSuffixToCommandNamesSimple(const CommandName& suffix);
		[[nodiscard]] std::vector<CommandName> getSuffixedCommandNames(const CommandName& suffix) const;
		[[nodiscard]] std::vector<CommandName> getSuffixedCommandNames(const std::vector<CommandName>& suffixes) const;

		static void combineCommandNames(std::vector<CommandName>& out_command_names, const std::vector<CommandName>& prefixes, const std::vector<CommandName>& suffixes);
		[[nodiscard]] static std::vector<CommandName> combineCommandNames(const std::vector<CommandName>& prefixes, const std::vector<CommandName>& suffixes);
		[[nodiscard]] static std::vector<CommandName> combineCommandNames(const std::vector<CommandName>& prefixes, const CommandName& suffix);
		[[nodiscard]] static std::vector<CommandName> combineCommandNames(const CommandName& prefix, const std::vector<CommandName>& suffixes);

		virtual void onCommand(Click& click, std::wstring& args)
		{
		}
	};
}
