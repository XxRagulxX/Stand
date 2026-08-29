#pragma once

#include "Game/grcViewport.hpp"

class CViewportManager
{
public:
	rage::grcViewport m_cachedGameGrcViewport[2];

	const rage::grcViewport* GetCurrentGameGrcViewport() const;
};
