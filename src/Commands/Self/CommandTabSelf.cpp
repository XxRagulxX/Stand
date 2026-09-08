#include "Commands/Self/CommandTabSelf.hpp"

namespace Stand::Features
{
	Stand::CommandTabSelf& GetCommandTabSelf()
	{
		static Stand::CommandTabSelf instance{};
		return instance;
	}
}
