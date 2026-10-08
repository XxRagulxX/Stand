#pragma once

#include "Game/gta_extensible.hpp"

namespace rage
{
	struct fwScriptGuid : public fwExtension
	{
		fwExtensibleBase* m_pBase;
	};
	static_assert(sizeof(fwScriptGuid) == 0x10);
}
