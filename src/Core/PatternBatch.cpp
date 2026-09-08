#include "Core/PatternBatch.hpp"

#include <algorithm>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#include "Core/LogHelper.hpp"
#include "Core/PatternCache.hpp"

namespace Stand
{
	PatternHash PatternBatch::MakePatternHash(
	    std::string_view signature)
	{
		PatternHash hash;
		std::size_t signatureByteLength = 0;

		for (const char c : signature)
		{
			hash = hash.Update(c);

			if (c == ' ')
			{
				++signatureByteLength;
			}
		}

		hash = hash.Update('\0');
		++signatureByteLength;
		hash = hash.Update(signatureByteLength);

		return hash;
	}

	void PatternBatch::Add(
	    std::string name,
	    soup::Range range,
	    std::string signature,
	    PatternCallback callback)
	{
		auto hash = MakePatternHash(signature);
		soup::Pattern pattern(signature);

		AddImpl(
		    std::move(name),
		    std::move(signature),
		    std::move(range),
		    std::move(pattern),
		    hash,
		    std::move(callback),
		    {});
	}

	void PatternBatch::AddOptional(
	    std::string name,
	    soup::Range range,
	    std::string signature,
	    PatternCallback callback,
	    PatternFailCallback failCallback)
	{
		auto hash = MakePatternHash(signature);
		soup::Pattern pattern(signature);

		AddImpl(
		    std::move(name),
		    std::move(signature),
		    std::move(range),
		    std::move(pattern),
		    hash,
		    std::move(callback),
		    std::move(failCallback));
	}

	void PatternBatch::AddImpl(
	    std::string name,
	    std::string signature,
	    soup::Range range,
	    soup::Pattern pattern,
	    PatternHash hash,
	    PatternCallback callback,
	    PatternFailCallback failCallback)
	{
		std::lock_guard lock(m_Mutex);

		m_Entries.emplace(
		    std::move(name),
		    std::move(signature),
		    std::move(range),
		    std::move(pattern),
		    hash,
		    std::move(callback),
		    std::move(failCallback));
	}

	void PatternBatch::ClearState()
	{
		std::lock_guard lock(m_Mutex);

		m_ErrorMessage.clear();
		m_HasFailed = false;
		m_CacheUtilisation.store(0, std::memory_order_relaxed);
	}

	bool PatternBatch::ProcessEntry(Entry& entry)
	{
		/*
		 * Try the persistent cache first.
		 *
		 * A cached offset is never trusted blindly. The resulting
		 * address must still be inside the scan range and the pattern
		 * must still match at that address.
		 */
		if (PatternCache::IsInitialized())
		{
			if (const auto cached =
			        PatternCache::GetCachedOffset(entry.m_Hash))
			{
				const auto cachedAddress =
				    entry.m_Range.base.as<std::uintptr_t>()
				    + static_cast<std::uintptr_t>(*cached);

				const soup::Pointer cachedResult(cachedAddress);

				if (cachedResult.isInRange(entry.m_Range)
				    && entry.m_Pattern.matches(
				        cachedResult.as<std::uint8_t*>()))
				{
					m_CacheUtilisation.fetch_add(
					    1,
					    std::memory_order_relaxed);

					LOG(INFO)
					    << "Using cached pattern ["
					    << entry.m_Name
					    << "] : ["
					    << HEX(cachedAddress)
					    << "]";

					/*
					 * The cached address is valid.
					 * Do not perform a full pattern scan.
					 */
					if (entry.m_Callback)
					{
						entry.m_Callback(cachedResult);
					}

					return true;
				}

				LOG(WARNING)
				    << "Cached pattern ["
				    << entry.m_Name
				    << "] is invalid, rescanning...";
			}
		}

		/*
		 * No usable cached address exists.
		 *
		 * This is the only path that performs a full SOUP scan.
		 */
		const auto result =
		    entry.m_Range.scan(entry.m_Pattern);

		if (result)
		{
			LOG(INFO)
			    << "Found pattern ["
			    << entry.m_Name
			    << "] : ["
			    << HEX(result.as<std::uintptr_t>())
			    << "]";

			if (entry.m_Callback)
			{
				entry.m_Callback(result);
			}

			/*
			 * Save the newly discovered offset for future runs.
			 */
			if (PatternCache::IsInitialized())
			{
				const auto offset =
				    result.as<std::uintptr_t>()
				    - entry.m_Range.base.as<std::uintptr_t>();

				PatternCache::UpdateCachedOffset(
				    entry.m_Hash,
				    static_cast<int>(offset));
			}

			return true;
		}

		LOG(WARNING)
		    << "Failed to find pattern ["
		    << entry.m_Name
		    << "]";

		/*
		 * Optional patterns are allowed to fail.
		 */
		if (entry.m_FailCallback)
		{
			entry.m_FailCallback(*this);
			return true;
		}

		/*
		 * Required pattern failed.
		 */
		{
			std::lock_guard lock(m_Mutex);

			m_HasFailed = true;

			if (m_ErrorMessage.empty())
			{
				m_ErrorMessage =
				    "Failed to find pattern(s): "
				    + entry.m_Name;
			}
			else
			{
				m_ErrorMessage += ", ";
				m_ErrorMessage += entry.m_Name;
			}
		}

		return false;
	}

	void PatternBatch::Worker()
	{
		while (true)
		{
			Entry entry{
			    "",
			    "",
			    soup::Range{},
			    soup::Pattern{},
			    PatternHash{},
			    {},
			    {}};

			{
				std::lock_guard lock(m_Mutex);

				if (m_Entries.empty())
				{
					return;
				}

				entry = std::move(m_Entries.front());
				m_Entries.pop();
			}

			ProcessEntry(entry);
		}
	}

	bool PatternBatch::Run()
	{
		ClearState();

		std::size_t entryCount = 0;

		{
			std::lock_guard lock(m_Mutex);
			entryCount = m_Entries.size();
		}

		if (entryCount == 0)
		{
			return true;
		}

		auto threadCount =
		    static_cast<std::size_t>(
		        std::thread::hardware_concurrency());

		if (threadCount == 0)
		{
			threadCount = 1;
		}

		threadCount = std::min(threadCount, entryCount);

		std::vector<std::thread> workers;
		workers.reserve(threadCount);

		for (std::size_t i = 0; i < threadCount; ++i)
		{
			workers.emplace_back([this] {
				Worker();
			});
		}

		for (auto& worker : workers)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}

		return !HasFailed();
	}

	bool PatternBatch::HasFailed() const noexcept
	{
		std::lock_guard lock(m_Mutex);
		return m_HasFailed;
	}
}