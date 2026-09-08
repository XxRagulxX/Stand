#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "World/IPL.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class IPLSelectorHelper
	{
	public:
		std::vector<std::pair<int, const char*>> availableIplNames = {};

		IPLSelectorHelper()
		{
			int i = 0;

			for (const auto& ipl : IPLs)
			{
				availableIplNames.push_back({i, ipl.FriendlyName.data()});
				i++;
			}
		}
	};

	static IPLSelectorHelper g_IPLSelectorHelper{};
	static CommandListSelect _IPLSelector{"iplselector", "IPL", "The IPL to load", g_IPLSelectorHelper.availableIplNames, 0};

	class IPLLoader : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto requestedIpl = _IPLSelector.GetState();

			for (auto& ipl_name : IPLs[requestedIpl].IPLNames)
				if (!STREAMING::IS_IPL_ACTIVE(ipl_name.data()))
					STREAMING::REQUEST_IPL(ipl_name.data());

			for (auto& remove_ipl_name : IPLs[requestedIpl].IPLNamesRemoveWhenLoad)
				STREAMING::REMOVE_IPL(remove_ipl_name.data());
		}
	};

	class IPLUnloader : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto selectedIpl = _IPLSelector.GetState();

			for (auto& ipl_name : IPLs[selectedIpl].IPLNames)
				if (STREAMING::IS_IPL_ACTIVE(ipl_name.data()))
					STREAMING::REMOVE_IPL(ipl_name.data());

			for (auto& ipl_name : IPLs[selectedIpl].IPLNamesRemove)
				STREAMING::REMOVE_IPL(ipl_name.data());
		}
	};

	class IPLTeleport : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (auto self = Self::GetPed())
			{
				auto selectedIpl = _IPLSelector.GetState();
				auto iplLocation = IPLs[selectedIpl].Location;

				self.TeleportTo(iplLocation);
			}
		}
	};

	static IPLLoader _IPLLoader{"loadipl", "Load IPL", "Load IPL"};
	static IPLUnloader _IPLUnloader{"unloadipl", "Unload IPL", "Unload IPL"};
	static IPLTeleport _IPLTeleport{"ipltp", "Teleport to IPL", "Teleport to the selected IPL"};
};