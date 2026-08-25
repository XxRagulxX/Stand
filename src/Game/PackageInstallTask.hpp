#pragma once

#include "lib/soup/Task.hpp"

#include <optional>

#include "lib/soup/HttpRequestTask.hpp"
#include "Game/Package.hpp"

namespace Stand
{
	class PackageInstallTask : public soup::Task
	{
	public:
		Package& package;
		size_t fi = 0;
		std::optional<soup::HttpRequestTask> hr;

		PackageInstallTask(Package& package)
			: package(package)
		{
		}

		void onTick() final;
	protected:
		void initiateDownload();
	};
}
