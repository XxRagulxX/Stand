#include "Commands/Widgets/CommandInputRid.hpp"

#include <fmt/format.h>

#include "Rendering/StandPort/get_next_arg.hpp"
#include "Util/lang.hpp"
#include "Util/str2int.hpp"

namespace Stand
{
	std::string CommandInputRid::getCommandSyntax() const
	{
		std::string syntax{ LANG_GET("CMD") };
		syntax.append(": ");
		syntax.append(command_names.at(0));
		syntax.append(" [rid]");
		return syntax;
	}

	void CommandInputRid::onCommand(Click& click, std::wstring& args)
	{
		auto arg = get_next_arg(args);
		if (arg.empty())
		{
			return onClick(click);
		}
		try
		{
			int64_t rid = std::stoll(arg);
			onCommand(click, rid);
			setValue(fmt::to_string(rid), click.type);
		}
		catch (const std::exception&)
		{
			onCommandInvalidArg(click, arg);
		}
	}

	int64_t CommandInputRid::getRid() const noexcept
	{
		return str2int<int64_t>(value, 0);
	}
}
