#pragma once

#include <Windows.h>

#include <atomic>
#include <cstdint>
#include <functional>
#include <string>

#include "Core/ErrorCommons.hpp"
#include "Core/Spinlock.hpp"

namespace Stand
{
	enum class ExceptionContext : uint8_t
	{
		OTHER = 0,
		LUA = 1
	};

	struct ExceptionData
	{
		_EXCEPTION_POINTERS* const exp;
		const ExceptionContext ctx;
	};

	long handle_exception_spec(void*) noexcept;

	using handle_exception_t = decltype(&handle_exception_spec);

	long handle_caught_exception_spec(ExceptionData*) noexcept;

	using handle_caught_exception_t = decltype(&handle_caught_exception_spec);

	long handle_uncaught_exception_spec(_EXCEPTION_POINTERS*) noexcept;

	using handle_uncaught_exception_t = decltype(&handle_uncaught_exception_spec);
}

extern "C"
{
	void exceptional_init(Stand::handle_caught_exception_t fpCaughtExp, Stand::handle_uncaught_exception_t fpLogUncaughtExp);

	void disable_exception_handling();

	[[nodiscard]] bool exceptional_has_exp();

	void exceptional_get_exp(void** pOutData, Stand::handle_exception_t* pOutHandler);

	void exceptional_clear_exp();

	long exceptional_on_exception(Stand::ExceptionData* data, Stand::handle_caught_exception_t handler);

	long exceptional_on_caught_exception(Stand::ExceptionData* data); long exceptional_on_uncaught_exception(_EXCEPTION_POINTERS* exp);

	void exceptional_set_retval(long retval);
}


//
// Standard exception handling macros
//

#define __EXCEPTIONAL_CTX(ctx)             \
	__except (                             \
	    ::exceptional_on_caught_exception( \
	        new ::Stand::ExceptionData{  \
	            GetExceptionInformation(), \
	            ::Stand::ExceptionContext::##ctx}))

#define __EXCEPTIONAL() \
	__EXCEPTIONAL_CTX(OTHER)

#define __EXCEPTIONAL_LUA() \
	__EXCEPTIONAL_CTX(LUA)

#define __EXCEPTIONAL_CUSTOM_CTX(handler, ctx)       \
	__except (                                       \
	    ::exceptional_on_exception(                  \
	        new ::Stand::ExceptionData{            \
	            GetExceptionInformation(),           \
	            ::Stand::ExceptionContext::##ctx}, \
	        handler))

#define __EXCEPTIONAL_CUSTOM(handler) \
	__EXCEPTIONAL_CUSTOM_CTX(handler, OTHER)

#define __EXCEPTIONAL_CUSTOM_LUA(handler) \
	__EXCEPTIONAL_CUSTOM_CTX(handler, LUA)

#define __EXCEPTIONAL_LOG_IF(cond)                              \
	__except (                                                  \
	    (cond) ? ::exceptional_on_caught_exception(             \
	                 new ::Stand::ExceptionData{              \
	                     GetExceptionInformation(),             \
	                     ::Stand::ExceptionContext::OTHER}) : \
	             EXCEPTION_EXECUTE_HANDLER)

#define __EXCEPTIONAL_IF(cond)                                  \
	__except (                                                  \
	    (cond) ? ::exceptional_on_caught_exception(             \
	                 new ::Stand::ExceptionData{              \
	                     GetExceptionInformation(),             \
	                     ::Stand::ExceptionContext::OTHER}) : \
	             EXCEPTION_CONTINUE_SEARCH)


//
// Exception-safe locking
//

#define EXCEPTIONAL_LOCK(mtx) \
	mtx.lock();               \
	__try                     \
	{
#define EXCEPTIONAL_TRY_LOCK(mtx) \
	if (mtx.tryLock())            \
		__try                     \
		{
#define EXCEPTIONAL_UNLOCK(mtx) \
	}                           \
	__EXCEPTIONAL()             \
	{                           \
	}                           \
	mtx.unlock();

#define EXCEPTIONAL_QUIET_UNLOCK(mtx)    \
	}                                    \
	__except (EXCEPTION_EXECUTE_HANDLER) \
	{                                    \
	}                                    \
	mtx.unlock();

namespace Stand
{
	class Exceptional
	{
	public:
		inline static std::atomic_uint32_t counted_threads = 0;

		static void hideThread(HANDLE hThread) noexcept;

		[[nodiscard]] static HANDLE createThread(std::function<void()>&& func) noexcept;

		[[nodiscard]] static HANDLE createExceptionalThread(std::function<void()>&& func) noexcept;

		static void createManagedThread(std::function<void()>&& func) noexcept;

		static void createManagedCountedThread(std::function<void()>&& func) noexcept;

		static void createManagedExceptionalThread(std::function<void()>&& func) noexcept;

		[[nodiscard]] static LPTOP_LEVEL_EXCEPTION_FILTER setUnhandledExceptionHandler();

		static void restoreUnhandledExceptionHandler() noexcept;

		inline static volatile bool thread_can_run = true;

		inline static volatile bool thread_is_running = true;

		static void disableExceptionHandling() noexcept;

		static void thread_func() noexcept;

		static void report(const std::string& type, std::string&& message, const ErrorInfo& err = ErrorInfo::fromStack(), 
			void custom_footer_info() = nullptr, ExceptionContext ctx = ExceptionContext::OTHER) noexcept;

		static void report(const std::string& type, std::string&& message, ExceptionData* data, void custom_footer_info() = nullptr) noexcept;

		static void report(const std::string& type, const ErrorInfo& err = ErrorInfo::fromStack()) noexcept;

		static long handleCaughtException(ExceptionData* data) noexcept;

		static long handleUncaughtException(_EXCEPTION_POINTERS* exp) noexcept;

		[[nodiscard]] static std::string getExceptionName(_EXCEPTION_POINTERS* exp) noexcept;

		[[nodiscard]] static std::string getExceptionName(const ExceptionData* data) noexcept;


	public:
		static long handleExceptionInErrorReporting(_EXCEPTION_POINTERS* exp) noexcept;

		inline static LPTOP_LEVEL_EXCEPTION_FILTER m_OldExceptionFilter = nullptr;

		inline static UINT m_OldErrorMode = 0;

		inline static bool m_ExceptionHandlerInstalled = false;
	};
}