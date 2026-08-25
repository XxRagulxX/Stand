#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandPackages : public CommandList
	{
	public:
		explicit CommandPackages(CommandList* parent);

		void onBack(ThreadContext thread_context) final;
	};
}
