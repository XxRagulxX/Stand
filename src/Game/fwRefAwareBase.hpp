#pragma once

#include "Game/datBase.hpp"
#include "Game/fwRefAwareBaseImpl.hpp"

namespace rage
{

	class fwRefAwareBase : public fwRefAwareBaseImpl<datBase>
	{
	};
	static_assert(sizeof(fwRefAwareBase) == 0x10);

}