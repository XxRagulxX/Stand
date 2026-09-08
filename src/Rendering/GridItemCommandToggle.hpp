#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	// A GridItemToggle wired to a real Stand::CommandToggle, looked up by
	// joaat hash - the Grid/GridItem equivalent of BoolCommandItem (src/
	// BoolCommandItem.cpp) for the ImGui menu. Reads the command's live
	// state every frame (rather than tracking its own), and calls
	// SetState() on click, mirroring BoolCommandItem::Draw() exactly.
	class GridItemCommandToggle : public GridItem
	{
	public:
		GridItemCommandToggle(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);

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
		const std::string& Label() const;

		CommandToggleLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
	};
}
