#pragma once

#include "Commands/Widgets/CommandActionName.hpp"

namespace Stand
{
	class CommandSavePv : public CommandActionName
	{
	public:
		explicit CommandSavePv(CommandList* parent);

		void onCommand(Click& click, std::wstring& args) final;
	};
}
