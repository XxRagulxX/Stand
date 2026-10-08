#pragma once

#include "Commands/Widgets/Command.hpp"

#include <soup/WeakRef.hpp>
#include <vector>

namespace Stand
{
	class CommandLink : public Command
	{
	public:
		soup::WeakRef<Command> target;
		bool show_address_in_corner;
		std::vector<int> m_Chain;

		CommandLink() : Command(COMMAND_LINK, nullptr), show_address_in_corner(false) {}
		explicit CommandLink(CommandList* parent, Command* target, bool show_address_in_corner = false);
		explicit CommandLink(CommandList* parent, CommandPhysical* target, bool show_address_in_corner = false);

		[[nodiscard]] CommandPhysical* getTarget() const noexcept;
	};
}
