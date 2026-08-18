#include "Worker.hpp"

#include <soup/os.hpp>

#include "AbstractPlayer.hpp"
#include "Exceptional.hpp"
#include "ExecCtx.hpp"
#include "Gui.hpp"
#include "PlayerHistory.hpp"
#include "pointers.hpp"
#include "regular_event.hpp"

namespace Stand
{
	// No destructible locals here: only references, so it's safe to __try in this function.
	static void lockAndTakeJobs(Spinlock& jobs_mtx, std::queue<std::function<void()>>& next_jobs, std::queue<std::function<void()>>& jobs)
	{
		EXCEPTIONAL_LOCK(jobs_mtx)
		jobs = std::move(next_jobs);
		EXCEPTIONAL_UNLOCK(jobs_mtx)
	}

	// No destructible locals here: only a reference to the job, so it's safe to __try in this function.
	static void runJob(std::function<void()>& job)
	{
		__try
		{
			job();
		}
		__EXCEPTIONAL()
		{
		}
	}

	// Isolated from thread_func() so that the __trys above never share a stack
	// frame with jobs/job, which are owned locals requiring unwinding.
	static void processQueuedJobs(Spinlock& jobs_mtx, std::queue<std::function<void()>>& next_jobs)
	{
		std::queue<std::function<void()>> jobs;
		lockAndTakeJobs(jobs_mtx, next_jobs, jobs);
		while (!jobs.empty())
		{
			auto job = std::move(jobs.front());
			jobs.pop();
			runJob(job);
		}
	}

	void Worker::thread_func()
	{
		THREAD_NAME("Worker");
		ExecCtx::get().tc = TC_WORKER;

		while (g_gui.unload_state < UnloadState::HAND_OVER_TO_MAIN)
		{
			__try
			{
				if (*pointers::CLoadingScreens_ms_Context == 0)
				{
					AbstractPlayer::doWork();
					if (root_update_mtx.tryLock())
					{
						__try
						{
							if (g_gui.doesRootStateAllowPlayerList())
							{
								g_gui.managePlayerList(TC_WORKER);
								if (PlayerHistory::needs_to_be_saved)
								{
									PlayerHistory::saveNow();
								}
							}
						}
						__EXCEPTIONAL()
						{
						}
						root_update_mtx.unlock();
					}
				}

				processQueuedJobs(jobs_mtx, next_jobs);

				soup::os::sleep(30);
			}
			__EXCEPTIONAL()
			{
			}
		}
		g_gui.worker_thread_running = false;
	}

	void Worker::queueJob(std::function<void()>&& func)
	{
		EXCEPTIONAL_LOCK(jobs_mtx)
		next_jobs.emplace(std::move(func));
		EXCEPTIONAL_UNLOCK(jobs_mtx)
	}
}
