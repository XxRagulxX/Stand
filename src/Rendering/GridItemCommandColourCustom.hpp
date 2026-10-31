#pragma once
#include "Rendering/StandPort/GridItem.hpp"
#include "Util/Joaat.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Stand { class CommandColourCustom; }

namespace Stand::Rendering
{
	class Grid;
	
	void AddColorCommandRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, joaat_t id, std::optional<std::string> labelOverride = std::nullopt);
	void AddColorCommandRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, Stand::CommandColourCustom* command, std::optional<std::string> labelOverride = std::nullopt);

	void AddConditionalColorCommandRows(Grid& grid,
	    std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    joaat_t id,
	    std::function<bool()> condition,
	    std::optional<std::string> labelOverride = std::nullopt);
	void AddConditionalColorCommandRows(Grid& grid,
	    std::vector<std::unique_ptr<GridItem>>& items_draft,
	    int16_t width,
	    Stand::CommandColourCustom* command,
	    std::function<bool()> condition,
	    std::optional<std::string> labelOverride = std::nullopt);
}
