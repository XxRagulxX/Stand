#include "Core/NuancedEventWithExtraData.hpp"

#include "Core/FlowEvent.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	Label NuancedEventWithExtraData::getName() const noexcept
	{
		return FlowEvent::getName(type, extra_data);
	}
}
