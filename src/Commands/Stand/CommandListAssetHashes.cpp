#include "Commands/Stand/CommandListAssetHashes.hpp"

#include "Network/CNetworkAssetVerifier.hpp"
#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Game/pointers.hpp"

#include "Commands/Extra/CommandFixAssetHashes.hpp"
#include "Commands/Extra/CommandSetAssetChecksum.hpp"
#include "Commands/Online/CommandSpoofedExtraCrc.hpp"
#include "Commands/Stand/CommandExtraCrcPresets.hpp"

namespace Stand
{
	CommandListAssetHashes::CommandListAssetHashes(CommandList* const parent)
		: CommandList(parent, LOC("AST"))
	{
		instance = this;

		fix = this->createChild<CommandFixAssetHashes>();
		this->createChild<CommandDivider>(LOC("ASTCRC"));
		this->createChild<CommandSetAssetChecksum>();
		this->createChild<CommandLambdaAction>(LOC("ASTRST"), CMDNAMES_OBF("resetassetchecksum"), NOLABEL, [](Click& click)
		{
			(*pointers::asset_verifier)->refreshCrc();
		});
		this->createChild<CommandDivider>(LOC("XTRCRC"));
		this->createChild<CommandTogglePointer>(&g_hooking.spoof_extra_content_crc, LOC("OVRXTR"), CMDNAMES_OBF("extratoggle"));
		auto extra_crc = this->createChild<CommandSpoofedExtraCrc>();
		this->createChild<CommandExtraCrcPresets>(extra_crc);
	}

	void CommandListAssetHashes::processCrcUpdate()
	{
		if (fix->m_on)
		{
 			fix->as<CommandFixAssetHashes>()->setLegitValues();
		}
	}
}
