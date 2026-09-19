#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/Appearance/CommandHairHighlight.hpp"
#include "Commands/Self/Appearance/CommandLockOutfit.hpp"
#include "Commands/Self/Appearance/CommandOutfitComponent.hpp"
#include "Commands/Self/Appearance/CommandOutfitComponentVariation.hpp"
#include "Commands/Self/Appearance/CommandOutfitProp.hpp"
#include "Commands/Self/Appearance/CommandOutfitPropVariation.hpp"
#include "Ped/outfit_data.hpp"

namespace Stand
{
	class CommandTabOutfit : public CommandList
	{
	public:
		CommandLockOutfit* const lockOutfit;

		explicit CommandTabOutfit(CommandList* parent)
			: CommandList(parent, LIT("Outfit"), CMDNAMES("outfit"))
			, lockOutfit(initChildren())
		{
		}

	private:
		CommandLockOutfit* initChildren()
		{
			for (const auto& c : ped_components)
			{
				auto* comp = createChild<CommandOutfitComponent>(&c);
				auto* var  = createChild<CommandOutfitComponentVariation>(&c);
				CommandLockOutfit::ped_component_commands[c.id] = {comp, var};
			}
			for (const auto& p : ped_props)
			{
				auto* prop = createChild<CommandOutfitProp>(&p);
				auto* var  = createChild<CommandOutfitPropVariation>(&p);
				CommandLockOutfit::ped_prop_commands[p.id] = {prop, var};
			}
			CommandLockOutfit::hair_highlight = createChild<CommandHairHighlight>();
			return createChild<CommandLockOutfit>();
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabOutfit& GetCommandTabOutfit();
}
