#include "Commands/Settings/CommandTabInput.hpp"

namespace Stand::Features
{
	Stand::CommandTabInput& GetCommandTabInput()
	{
		static Stand::CommandTabInput instance{};
		return instance;
	}
}
