#include "Commands/Stand/Appearance/CommandTabEntityPreviews.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandTabEntityPreviews& GetCommandTabEntityPreviews()
	{
		return *GetCommandTabStand().entityPreviews;
	}
}
