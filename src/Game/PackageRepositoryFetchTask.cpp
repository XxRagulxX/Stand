#include "Game/PackageRepositoryFetchTask.hpp"

#include "lib/soup/json.hpp"

#include "Game/PackageMgr.hpp"

namespace Stand
{
	void PackageRepositoryFetchTask::onTick()
	{
		if (gfc.tickUntilDone())
		{
			if (auto root = soup::json::decode(gfc.result))
			{
				repo.fromJson(root->asObj());
			}
			setWorkDone();
		}
	}

	void InitRepoFromCustomSourceTask::onTick()
	{
		if (gfc.tickUntilDone())
		{
			if (auto root = soup::json::decode(gfc.result))
			{
				PackageMgr::tunablesData(root->asObj());
			}
			else
			{
				PackageMgr::tunablesFail();
			}
			setWorkDone();
		}
	}

	std::string InitRepoFromCustomSourceTask::toString() const SOUP_EXCAL
	{
		return soup::ObfusString("InitRepoFromCustomSourceTask");
	}
}
