#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

#include "Vehicle/CVehicleModelInfo.hpp"

namespace Stand
{
	class CommandTrafficModelFlag : public CommandListSelect
	{
	private:
		const CVehicleModelInfo::Flags flag;

	public:
		explicit CommandTrafficModelFlag(CommandList* parent, CVehicleModelInfo::Flags flag, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL);

		void onChange(Click& click, long long prev_value) final;
	};
}
