#include "Network/Blacklist.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Network/Auth.hpp"
#include "Util/Codename.hpp"
#include "Core/Exceptional.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	// Separate from the __try frame: `advertisers`' iterator is non-trivially destructible under the Debug CRT.
	static bool checkIsAdvertiserV2(const rage::rlGamerInfo& gi)
	{
		for (const auto& a : Blacklist::advertisers)
		{
			if (a->appliesTo(gi))
			{
				return true;
			}
		}
		return false;
	}

	bool Blacklist::isAdvertiserV2(const rage::rlGamerInfo& gi)
	{
		bool ret = false;
		EXCEPTIONAL_LOCK(mtx)
		ret = checkIsAdvertiserV2(gi);
		EXCEPTIONAL_UNLOCK(mtx)
		return ret;
	}

	// Separate from the __try frame: `advertisers`' iterator is non-trivially destructible under the Debug CRT.
	static bool checkIsAdvertiserByRid(int64_t rid)
	{
		for (const auto& a : Blacklist::advertisers)
		{
			if (a->appliesTo(rid))
			{
				return true;
			}
		}
		return false;
	}

	bool Blacklist::isAdvertiserByRid(int64_t rid)
	{
		bool ret = false;
		EXCEPTIONAL_LOCK(mtx)
		ret = checkIsAdvertiserByRid(rid);
		EXCEPTIONAL_UNLOCK(mtx)
		return ret;
	}

	void Blacklist::addAdvertiser(const rage::rlGamerInfo& gi, uint8_t ad_level)
	{
		// Does the remote care?
		if (g_auth.hasApiCredentials()
			&& !isAdvertiserV2(gi)
			)
		{
			auto gid = GamerIdentifierV4::fromGamerInfo(gi);

			// Did we not report them before or is it a higher ad level this time?
			auto e = reported_advertisers.find(gid.xored_and_scrambled_rockstar_id);
			if (e == reported_advertisers.end()
				|| e->second < ad_level
				)
			{
				// Inform remote
				if (has_remote_data)
				{
					std::string data = gid.toString();
					data.append(fmt::to_string(ad_level));
					g_auth.reportEvent("B3", std::move(data));
				}

				// Update local data
				if (e != reported_advertisers.end())
				{
					e->second = ad_level;
				}
				else
				{
					reported_advertisers.emplace(gid.xored_and_scrambled_rockstar_id, ad_level);
				}
			}
		}
	}
}
