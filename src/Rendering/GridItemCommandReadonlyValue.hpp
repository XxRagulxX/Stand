#pragma once
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <chrono>
#include <optional>
#include <string>

namespace Stand::Rendering
{
	// A label + a live, per-frame-recomputed value, bound to a real
	// Stand::StandWidgets::CommandReadonlyValue looked up by joaat hash -
	// clicking (or Enter) copies the current value to the clipboard,
	// flashing a brief "Copied!" confirmation in place of the value for a
	// second - ported from real Stand's own CommandReadonlyValueCopy
	// (Commands/Widgets/CommandReadonlyValueCopy.hpp on origin/
	// stand-reference), folded together with CommandReadonlyValueLambda's
	// own live-recompute (see CommandReadonlyValue.hpp's own class
	// comment for why both live in one row here).
	class GridItemCommandReadonlyValue : public GridItem
	{
	public:
		GridItemCommandReadonlyValue(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		void activate() override;

		[[nodiscard]] std::string GetDescription() const override;

	private:
		void CopyToClipboard();

		joaat_t m_Id;
		std::optional<std::string> m_LabelOverride;
		// Brief "Copied!" flash after a click - std::chrono, not a frame
		// counter, so it reads the same regardless of framerate,
		// matching every other flash-timing field this project already
		// uses (Notification::m_FlashUntil, ...).
		std::chrono::steady_clock::time_point m_CopiedUntil{};
	};
}
