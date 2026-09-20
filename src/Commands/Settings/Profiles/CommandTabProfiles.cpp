#include "Commands/Settings/Profiles/CommandTabProfiles.hpp"

namespace Stand::Features
{
	Stand::CommandTabProfiles& GetCommandTabProfiles()
	{
		static Stand::CommandTabProfiles instance{};
		return instance;
	}
}
