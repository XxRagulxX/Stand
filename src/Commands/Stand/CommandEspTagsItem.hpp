#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "Rendering/create_rainbow.hpp"
#include "Network/player_tags.hpp"
#include "Rendering/Renderer.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandEspTagsItem : public CommandList
	{
	private:
		CommandColour* colour = nullptr;
		CommandToggle* toggle = nullptr;

	public:
		explicit CommandEspTagsItem(CommandList* const parent, playerflag_t flag)
			: CommandList(parent, PlayerTag_getLabel(flag))
		{
			colour = createChild<CommandColour>(LOC("CLR"), { combineCommandNames(CMDNAMES_OBF("espcolour"), StringUtils::simplify(getMenuName().getEnglishForCommandName())) }, NOLABEL, g_renderer.arColour);
			toggle = createChild<CommandToggle>(LOC("UCLR"));

			create_rainbow(colour);
		}

		[[nodiscard]] DirectX::SimpleMath::Color getColour() const
		{
			return colour->getRGBA();
		}

		[[nodiscard]] bool isEnabled() const
		{
			return toggle->m_on;
		}
	};
}