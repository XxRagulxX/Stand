#include "Commands/Player/CommandPlayerIntelLambda.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"
#include "Network/PlayerProvider.hpp"

namespace Stand
{
	std::wstring CommandPlayerIntelLambda::getValue(const AbstractPlayer p) const
	{
		return getValue_impl(p);
	}
}
