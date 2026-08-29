#include "Util/String2Hash.hpp"

#include "Game/atStringHash.hpp"

namespace Stand
{
	void String2Hash::convertToHash()
	{
#ifdef STAND_DEBUG
		SOUP_ASSERT(!isInHashForm());
#endif
		hash = rage::atStringHash(str);
		remain = 0;
	}
}
