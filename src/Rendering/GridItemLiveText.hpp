#pragma once
#include "Rendering/GridItem.hpp"

#include <DirectXMath.h>
#include <functional>
#include <string>

namespace Stand::Rendering
{
	// GridItemText's live-updating counterpart - same plain single-line
	// label, but the string comes from a callback re-run on every
	// drawText() call instead of being fixed at populate() time. Needed
	// wherever a stat changes from frame to frame without the row count
	// around it changing (health, coordinates, distance-to-target, ...) -
	// compare GridItemPlayerList, which re-reads its own live data
	// source (Players::GetPlayers()) for the same reason, since
	// populate() only ever runs once (see Grid.hpp's own class comment).
	// A stat whose *presence*, not just its value, changes (a whole
	// section appearing or disappearing) still needs a real
	// invalidate()/repopulate the way PlayersGrid's own Categories
	// section does - this only covers a value changing in place.
	class GridItemLiveText : public GridItem
	{
	public:
		GridItemLiveText(int16_t width, int16_t height, std::function<std::string()> text, DirectX::XMFLOAT4 colour);

		void drawText() override;

	private:
		std::function<std::string()> m_Text;
		DirectX::XMFLOAT4 m_Colour;
	};
}
