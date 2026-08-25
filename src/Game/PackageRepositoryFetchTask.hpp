#pragma once

#include "lib/soup/Task.hpp"

#include "lib/soup/GetFileContentsTask.hpp"
#include "Game/PackageRepository.hpp"

namespace Stand
{
	struct PackageRepositoryFetchTask : public soup::Task
	{
		soup::GetFileContentsTask gfc;
		PackageRepository repo;

		PackageRepositoryFetchTask(const soup::Uri& uri)
			: gfc(uri)
		{
		}

		void onTick() final;
	};

	struct InitRepoFromCustomSourceTask : public soup::Task
	{
		soup::GetFileContentsTask gfc;

		InitRepoFromCustomSourceTask(const soup::Uri& uri)
			: gfc(uri)
		{
		}

		void onTick() final;

		[[nodiscard]] std::string toString() const SOUP_EXCAL final;
	};
}
