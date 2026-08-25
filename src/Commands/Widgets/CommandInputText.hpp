#pragma once

#include "Commands/Widgets/CommandInput.hpp"

namespace Stand
{
	class CommandInputText : public CommandInput
	{
	public:
		using CommandInput::CommandInput;

		std::wstring getCommandSyntax() const final;
	};
}
