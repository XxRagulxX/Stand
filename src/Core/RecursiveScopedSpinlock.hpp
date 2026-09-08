#pragma once
#include "Core/ExceptionHandler.hpp"
#include "Util/FixedVector.hpp"

#include <atomic>

#define EXCEPTIONAL_LOCK_READ(mtx) \
	mtx.lockRead();                \
	__try                          \
	{
#define EXCEPTIONAL_UNLOCK_READ(mtx) \
	}                                \
	__EXCEPTIONAL()                  \
	{                                \
	}                                \
	mtx.unlockRead();

#define EXCEPTIONAL_LOCK_WRITE(mtx) \
	mtx.lockWrite();                \
	__try                           \
	{
#define EXCEPTIONAL_UNLOCK_WRITE(mtx) \
	}                                 \
	__EXCEPTIONAL()                   \
	{                                 \
	}                                 \
	mtx.unlockWrite();

namespace Stand
{
	class RecursiveScopedSpinlock
	{
	private:
		FixedVector<uint32_t, 10> readers{};
		std::atomic<uint32_t> writer = 0;
		uint8_t write_recursions = 0;

	public:
		[[nodiscard]] bool isReadLocked() const noexcept;
		[[nodiscard]] bool isReadLockedByThisThread() const noexcept;
		[[nodiscard]] bool isWriteLocked() const noexcept;
		[[nodiscard]] bool isWriteLockedByThisThread() const noexcept;
		[[nodiscard]] bool isLockedByThisThread() const noexcept;

		void lockRead() noexcept;

	private:
		void lockReadInner() noexcept;
		void extendReadLock() noexcept;

	public:
		void unlockRead() noexcept;

		void lockWrite() noexcept;

	private:
		void lockWriteInner() noexcept;

	public:
		[[nodiscard]] bool tryLockWrite() noexcept;
		void unlockWrite() noexcept;
	};
}
