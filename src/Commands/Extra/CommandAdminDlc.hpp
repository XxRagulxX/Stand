#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/DlcMgr.hpp"

namespace Stand
{
	class CommandAdminDlc : public CommandToggle
	{
	public:
		explicit CommandAdminDlc(CommandList* parent)
			: CommandToggle(parent, LOC("ADMDLC"), CMDNAMES("admindlc"), LOC("ADMDLC_H2"))
		{
		}

		void onChange(Click& click) final
		{
			DlcMgr::admin_dlc = m_on;
			DlcMgr::update();
		}
	};
}
