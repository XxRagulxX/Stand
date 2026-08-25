#pragma once

#include "Core/evtEvent.hpp"

namespace Stand
{
	struct evtTransitionFinishedEvent : public evtEvent<evtTransitionFinishedEvent>
	{
	};
}
