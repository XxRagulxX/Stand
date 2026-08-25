#pragma once

#include "Commands/Widgets/Command.hpp"

#include "lib/soup/WeakRef.hpp"

namespace Stand
{
	class CommandLink : public Command
	{
	public:
		soup::WeakRef<Command> target;
		bool show_address_in_corner;

		explicit CommandLink(CommandList* parent, Command* target, bool show_address_in_corner = false);
		explicit CommandLink(CommandList* parent, CommandPhysical* target, bool show_address_in_corner = false);

		[[nodiscard]] CommandPhysical* getTarget() const noexcept;
	};
}
