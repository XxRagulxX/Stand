#include "Commands/Self/CommandTabOutfit.hpp"

#include "Commands/Self/CommandTabSelf.hpp"

namespace Stand::Features
{
	Stand::CommandTabOutfit& GetCommandTabOutfit()
	{
		return *GetCommandTabSelf().outfit;
	}
}
