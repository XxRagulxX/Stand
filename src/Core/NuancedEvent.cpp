#include "Core/NuancedEvent.hpp"

#include "Core/FlowEvent.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	Label NuancedEvent::getName() const noexcept
	{
		return FlowEvent::getName(type);
	}

	bool NuancedEvent::isPreemptive() const noexcept
	{
		return FlowEvent::is_preemptive(type, severity);
	}
}
