#pragma once
#include "Rendering/GridItem.hpp"
#include "Util/Joaat.hpp"
#include "Commands/PlayerCommand.hpp"

#include <optional>
#include <string>

namespace Stand::Rendering
{
	// A GridItemButton-shaped row wired to a real Stand::PlayerCommand,
	// looked up by joaat hash - the Grid equivalent of PlayerCommandItem
	// (src/PlayerCommandItem.cpp) for the ImGui menu. Click behaviour
	// mirrors PlayerCommandItem::Draw() exactly: FiberPool::queueJob
	// calling Command::Call(Players::GetSelected()) if a player is
	// actually selected (see GridItemPlayerList, this system's own
	// equivalent of MenuPlayers.cpp's DrawPlayerList() for picking one).
	class GridItemCommandPlayer : public GridItem
	{
	public:
		GridItemCommandPlayer(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);

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

		PlayerCommand* m_Command;
		std::optional<std::string> m_LabelOverride;
	};
}
