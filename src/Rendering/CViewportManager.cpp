#include "Rendering/CViewportManager.hpp"

#include "Game/fwRenderThreadInterface.hpp"
#include "Game/pointers.hpp"

const rage::grcViewport* CViewportManager::GetCurrentGameGrcViewport() const
{
	return &m_cachedGameGrcViewport[Stand::pointers::render_thread_interface->GetCurrentBuffer()];
}
