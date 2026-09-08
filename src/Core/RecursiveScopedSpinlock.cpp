#include "Core/RecursiveScopedSpinlock.hpp"

#include "Core/ExceptionHandler.hpp"

#include <windows.h>

#define KEEP_READERS_UPGRADED true

namespace Stand
{
	bool RecursiveScopedSpinlock::isReadLocked() const noexcept
	{
		return !readers.empty();
	}

	bool RecursiveScopedSpinlock::isReadLockedByThisThread() const noexcept
	{
		return readers.contains(GetCurrentThreadId());
	}

	bool RecursiveScopedSpinlock::isWriteLocked() const noexcept
	{
		return writer != 0;
	}

	bool RecursiveScopedSpinlock::isWriteLockedByThisThread() const noexcept
	{
		return writer == GetCurrentThreadId();
	}

	bool RecursiveScopedSpinlock::isLockedByThisThread() const noexcept
	{
		return isWriteLockedByThisThread() || isReadLockedByThisThread();
	}

	void RecursiveScopedSpinlock::lockRead() noexcept
	{
		if (isReadLockedByThisThread())
			extendReadLock();
		else
			lockReadInner();
	}

	void RecursiveScopedSpinlock::lockReadInner() noexcept
	{
#if KEEP_READERS_UPGRADED
		if (isWriteLockedByThisThread())
			++write_recursions;
#endif

		auto tid = GetCurrentThreadId();

		readers.emplace(tid);

		if (isWriteLocked() && writer != tid)
		{
			const auto start = GetTickCount64();
			do
			{
				if (GetTickCount64() - start > 3000)
				{
					Stand::Exceptional::report("Bad Timing", "RecursiveScopedSpinlock::lockReadInner");
					break;
				}
			} while (isWriteLocked());
		}
	}

	void RecursiveScopedSpinlock::extendReadLock() noexcept
	{
		readers.emplace(GetCurrentThreadId());
	}

	void RecursiveScopedSpinlock::unlockRead() noexcept
	{
		readers.erase(GetCurrentThreadId());

#if KEEP_READERS_UPGRADED
		if (!isReadLockedByThisThread() && isWriteLockedByThisThread())
			unlockWrite();
#endif
	}

	void RecursiveScopedSpinlock::lockWrite() noexcept
	{
		if (!isWriteLockedByThisThread())
			lockWriteInner();
		else
			++write_recursions;
	}

	void RecursiveScopedSpinlock::lockWriteInner() noexcept
	{
		const auto start = GetTickCount64();
	retry:

	{
		for (uint32_t expected; expected = 0, !writer.compare_exchange_weak(expected, GetCurrentThreadId());)
		{
			if (GetTickCount64() - start > 3000)
			{
				Stand::Exceptional::report("Bad Timing", "RecursiveScopedSpinlock::lockWriteInner (set writer)");
				return;
			}
		}
	}

		if (readers.countExcept(GetCurrentThreadId()) != 0)
		{
			if (GetTickCount64() - start > 3000)
			{
				Stand::Exceptional::report("Bad Timing", "RecursiveScopedSpinlock::lockWriteInner (wait for readers)");
				return;
			}
			writer = 0;
			YieldProcessor();
			goto retry;
		}
	}

	bool RecursiveScopedSpinlock::tryLockWrite() noexcept
	{
		if (readers.countExcept(GetCurrentThreadId()) != 0)
			return false;

		uint32_t expected = 0;
		if (writer.compare_exchange_strong(expected, GetCurrentThreadId()))
		{
			if (readers.countExcept(GetCurrentThreadId()) != 0)
			{
				writer = 0;
				return false;
			}

			return true;
		}

		if (expected == GetCurrentThreadId())
		{
			++write_recursions;
			return true;
		}

		return false;
	}

	void RecursiveScopedSpinlock::unlockWrite() noexcept
	{
		if (!isWriteLockedByThisThread())
			return;

		if (write_recursions == 0)
		{
#if KEEP_READERS_UPGRADED
			if (isReadLockedByThisThread())
				return;
#endif
			writer = 0;
		}
		else
		{
			--write_recursions;
		}
	}
}
