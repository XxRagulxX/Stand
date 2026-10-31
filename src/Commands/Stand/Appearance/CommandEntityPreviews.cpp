#include "Commands/Stand/Appearance/CommandEntityPreviews.hpp"
#include "Commands/Stand/CommandTabStand.hpp"

namespace Stand::Features
{
	Stand::CommandEntityPreviews& GetCommandEntityPreviews()
	{
		return *GetCommandTabStand().entityPreviews;
	}
}
