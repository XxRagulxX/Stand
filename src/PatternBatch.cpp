#include "PatternBatch.hpp"

#include <thread>

#include <soup/ObfusString.hpp>

#include "Codename.hpp"
#include "Exceptional.hpp"
#include "main.hpp"
#include "RecursiveSpinlock.hpp"
#include "StringUtils.hpp"

namespace Stand
{
	static RecursiveSpinlock mtx{};
	static PatternBatch* inst = nullptr;

	PatternBatch::PatternBatch(std::unordered_map<std::string, uintptr_t>* cache)
		: cache(cache)
	{
	}

	void PatternBatch::add(const Codename& name, soup::Range&& range, soup::Pattern&& pattern, pattern_callback_t callback)
	{
		add_impl(name, std::move(range), std::move(pattern), callback, nullptr);
	}

	void PatternBatch::add_optional(const Codename& name, soup::Range&& range, soup::Pattern&& pattern, pattern_callback_t callback, pattern_fail_callback_t fail_callback)
	{
		add_impl(name, std::move(range), std::move(pattern), callback, fail_callback);
	}

	// name.toString() constructs a temporary std::string, so this can't run inside add_impl's EXCEPTIONAL_LOCK __try.
	static void addPatternEntry(PatternBatch& self, const Codename& name, soup::Range&& range, soup::Pattern&& pattern, pattern_callback_t callback, pattern_fail_callback_t fail_callback)
	{
#ifdef STAND_DEBUG
		if (self.entry_names.contains(name.toString()))
		{
			Exceptional::report("Duplicate pattern name");
		}
		self.entry_names.emplace(name.toString());
#endif
		self.entries.emplace(name.toString(), std::move(range), std::move(pattern), callback, fail_callback);
	}

	void PatternBatch::add_impl(const Codename& name, soup::Range&& range, soup::Pattern&& pattern, pattern_callback_t callback, pattern_fail_callback_t fail_callback)
	{
		EXCEPTIONAL_LOCK(mtx)
		addPatternEntry(*this, name, std::move(range), std::move(pattern), callback, fail_callback);
		EXCEPTIONAL_UNLOCK(mtx)
	}

	// cache_i is an unordered_map iterator (non-trivially destructible under the debug CRT), so it must not exist inside a __try.
	static void updatePatternCache(PatternBatch* inst, const std::string& name, uintptr_t offset)
	{
		auto cache_i = inst->cache->find(name);
		if (cache_i == inst->cache->end())
		{
			inst->cache->emplace(name, std::move(offset));
		}
		else
		{
			cache_i->second = std::move(offset);
		}
	}

	// entry, owned by threadPatternScan below, has a non-trivial destructor, so the __try must live in a function that doesn't own it.
	static void updatePatternCacheLocked(PatternBatch* inst, const std::string& name, uintptr_t offset)
	{
		EXCEPTIONAL_LOCK(mtx)
		updatePatternCache(inst, name, offset);
		EXCEPTIONAL_UNLOCK(mtx)
	}

	// list_append's parameter is const StringCastable& (owns a std::string), so
	// passing a std::string into it still implicitly constructs a StringCastable
	// temporary at the call site - kept out of invokePatternFailCallbackLocked's __try below.
	static void appendAllowedFail(PatternBatch* inst, const std::string& name)
	{
		StringUtils::list_append(inst->allowed_fails, name);
	}

	// entry, owned by threadPatternScan below, has a non-trivial destructor, so the __try must live in a function that doesn't own it.
	static void invokePatternFailCallbackLocked(PatternBatch* inst, const PatternBatch::Entry& entry)
	{
		EXCEPTIONAL_LOCK(mtx)
		appendAllowedFail(inst, entry.name);
		entry.fail_callback(*inst);
		EXCEPTIONAL_UNLOCK(mtx)
	}

	// building this message constructs ObfusString/std::string temporaries, so it can't run inside the __try below.
	static void appendPatternFailureMessage(PatternBatch* inst, const std::string& name)
	{
		if (inst->error_message.empty())
		{
			inst->error_message = soup::ObfusString("Failed to find pattern(s): ").str().append(name);
		}
		else
		{
			inst->error_message.append(", ").append(name);
		}
	}

	// entry, owned by threadPatternScan below, has a non-trivial destructor, so the __try must live in a function that doesn't own it.
	static void appendPatternFailureMessageLocked(PatternBatch* inst, const PatternBatch::Entry& entry)
	{
		EXCEPTIONAL_LOCK(mtx)
		appendPatternFailureMessage(inst, entry.name);
		EXCEPTIONAL_UNLOCK(mtx)
	}

	void threadPatternScan()
	{
		do
		{
			mtx.lock();
			if (inst->entries.empty())
			{
				mtx.unlock();
				return;
			}
			PatternBatch::Entry entry = std::move(inst->entries.front());
			inst->entries.pop();
			bool from_cache = false;
			soup::Pointer result;
			if (inst->cache)
			{
				auto cached = inst->cache->find(entry.name);
				if (cached != inst->cache->end())
				{
					result = (entry.range.base.as<uintptr_t>() + cached->second);
					mtx.unlock();
					if (result.isInRange(entry.range)
						&& entry.pattern.matches(result.as<uint8_t*>())
						)
					{
						from_cache = true;
						inst->cache_utilisation++;
					}
					else
					{
						result = nullptr;
					}
				}
				else
				{
					mtx.unlock();
				}
			}
			else
			{
				mtx.unlock();
			}
			if (!result)
			{
				result = entry.range.scan(entry.pattern);
			}
			if (result)
			{
				entry.callback(result);
				if (!from_cache)
				{
					uintptr_t offset = (result.as<uintptr_t>() - entry.range.base.as<uintptr_t>());
					if (inst->cache)
					{
						updatePatternCacheLocked(inst, entry.name, offset);
					}
				}
			}
			else if (entry.fail_callback != nullptr)
			{
				invokePatternFailCallbackLocked(inst, entry);
			}
			else
			{
				appendPatternFailureMessageLocked(inst, entry);
			}
		} while (true);
	}

	void PatternBatch::run()
	{
		error_message.clear();
		inst = this;

		unsigned int threads_to_spin_up = std::thread::hardware_concurrency();
		if (threads_to_spin_up > entries.size())
		{
			threads_to_spin_up = (unsigned int)entries.size();
		}

		std::vector<HANDLE> threads;
		for (unsigned int i = 0; i < threads_to_spin_up; i++)
		{
			threads.emplace_back(Exceptional::createExceptionalThread(&threadPatternScan));
		}

		WaitForMultipleObjects((DWORD)threads.size(), &threads.at(0), TRUE, INFINITE);

		for (auto& thread : threads)
		{
			CloseHandle(thread);
		}
	}

	void PatternBatch::runSingleThread()
	{
		error_message.clear();
		inst = this;

		threadPatternScan();
	}
}
