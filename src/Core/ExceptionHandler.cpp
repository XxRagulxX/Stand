#include "Core/ExceptionHandler.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <signal.h>
#include <string>

#include <rtcapi.h>

#include <soup/os.hpp>

#include "Core/StackTrace.hpp"

#include <AsyncLogger/Logger.hpp>

namespace Stand
{
	long handle_exception_spec(void*) noexcept
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	long handle_caught_exception_spec(ExceptionData*) noexcept
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	long handle_uncaught_exception_spec(_EXCEPTION_POINTERS*) noexcept
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

extern "C"
{
	void exceptional_init(
	    Stand::handle_caught_exception_t fpCaughtExp,
	    Stand::handle_uncaught_exception_t fpLogUncaughtExp);

	void disable_exception_handling();

	[[nodiscard]] bool exceptional_has_exp();

	void exceptional_get_exp(void** pOutData, Stand::handle_exception_t* pOutHandler);

	void exceptional_clear_exp();

	long exceptional_on_exception(Stand::ExceptionData* data, Stand::handle_caught_exception_t handler);

	long exceptional_on_caught_exception(Stand::ExceptionData* data);

	long exceptional_on_uncaught_exception(_EXCEPTION_POINTERS* exp);

	void exceptional_set_retval(long retval);
}


namespace Stand
{
	namespace
	{
		long callHandlerSafely(handle_exception_t handler, void* data) noexcept
		{
			if (!handler)
			{
				return EXCEPTION_EXECUTE_HANDLER;
			}

			__try
			{
				return handler(static_cast<ExceptionData*>(data));
			}
			__except (Exceptional::handleExceptionInErrorReporting(GetExceptionInformation()))
			{
				return EXCEPTION_EXECUTE_HANDLER;
			}
		}

		void runExceptionalFunction(std::function<void()>* function) noexcept
		{
			if (!function)
			{
				return;
			}

			__try
			{
				(*function)();
			}
			__EXCEPTIONAL()
			{
			}
		}

		bool tryGetCppExceptionMessage(_EXCEPTION_POINTERS* exp, char* output, size_t outputSize) noexcept
		{
			if (!exp || !exp->ExceptionRecord || !output || outputSize == 0)
			{
				return false;
			}

			__try
			{
				auto* exception = reinterpret_cast<std::exception*>(exp->ExceptionRecord->ExceptionInformation[1]);

				if (!exception)
				{
					return false;
				}

				const char* message = exception->what();

				if (!message)
				{
					return false;
				}

				std::strncpy(output, message, outputSize - 1);

				output[outputSize - 1] = '\0';

				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
		}

		// Format a 64-bit address.

		std::string formatAddress(ULONG_PTR address)
		{
			char buffer[19]{};

			std::snprintf(buffer, sizeof(buffer), "0x%016llX", static_cast<unsigned long long>(address));

			return buffer;
		}

		// Format a Windows exception code.

		std::string formatExceptionCode(DWORD exceptionCode)
		{
			char buffer[11]{};

			std::snprintf(buffer, sizeof(buffer), "0x%08X", static_cast<uint32_t>(exceptionCode));

			return buffer;
		}

		// Decode exception information.

		void parseExceptionInformation(std::string& exceptionName, const ULONG_PTR* info)
		{
			if (!info)
			{
				return;
			}

			switch (info[0])
			{
			case 0:
				exceptionName.append(" while reading from ");

				exceptionName.append(formatAddress(info[1]));

				break;

			case 1:
				exceptionName.append(" while writing to ");

				exceptionName.append(formatAddress(info[1]));

				break;

			case 8:
				exceptionName.append(" (DEP at ");

				exceptionName.append(formatAddress(info[1]));

				exceptionName.push_back(')');

				break;

			default:
				exceptionName.append(" while ");

				exceptionName.append(std::to_string(info[0]));

				exceptionName.append(" at ");

				exceptionName.append(formatAddress(info[1]));

				break;
			}
		}
	}


	//
	// Thread management
	//

	void Exceptional::hideThread(HANDLE hThread) noexcept
	{
		(void)hThread;
	}


	HANDLE Exceptional::createThread(
	    std::function<void()>&& func) noexcept
	{
		auto* function = new (std::nothrow) std::function<void()>(std::move(func));

		if (!function)
		{
			return nullptr;
		}

		const HANDLE thread = CreateThread(
		    nullptr,
		    0,
		    [](LPVOID parameter) -> DWORD {
			    /*
			     * Stand installs these CRT/runtime handlers when its
			     * worker threads start. They are process-wide handlers,
			     * so do not reinstall the StandEnhanced unhandled-exception
			     * filter here; it is installed during startup in main.cpp.
			     */

			    _set_abort_behavior(0, 0);

			    _RTC_SetErrorFuncW(
			        [](int,
			            const wchar_t*,
			            int,
			            const wchar_t*,
			            const wchar_t*,
			            ...) {
				        Exceptional::report(
				            "CRT Run-Time Error",
				            {});
				        return 0;
			        });

			    _set_purecall_handler(
			        [] {
				        Exceptional::report(
				            "Pure Virtual Call",
				            {});
			        });

			    signal(
			        SIGINT,
			        [](int) {
				        throw std::exception("SIGINT");
			        });

			    signal(
			        SIGTERM,
			        [](int) {
				        throw std::exception("SIGTERM");
			        });

			    signal(
			        SIGABRT,
			        [](int) {
				        throw std::exception("SIGABRT");
			        });

			    set_terminate(
			        [] {
				        Exceptional::report(
				            "Terminate Called",
				            {});
				        std::abort();
			        });

			    auto* function =
			        static_cast<std::function<void()>*>(parameter);

			    (*function)();

			    delete function;
			    return 0;
		    },
		    function,
		    0,
		    nullptr);

		if (!thread)
		{
			delete function;
			return nullptr;
		}

		hideThread(thread);

		return thread;
	}


	HANDLE Exceptional::createExceptionalThread(std::function<void()>&& func) noexcept
	{
		auto* function = new (std::nothrow) std::function<void()>(std::move(func));

		if (!function)
		{
			return nullptr;
		}

		return createThread([function] {
			runExceptionalFunction(function);

			delete function;
		});
	}


	void Exceptional::createManagedThread(std::function<void()>&& func) noexcept
	{
		HANDLE thread = createThread(std::move(func));

		if (thread)
		{
			CloseHandle(thread);
		}
	}


	void Exceptional::createManagedCountedThread(std::function<void()>&& func) noexcept
	{
		SOUP_ASSERT(++counted_threads != 0);

		createManagedThread([func = std::move(func)]() mutable {
			func();
			--counted_threads;
		});
	}


	void Exceptional::createManagedExceptionalThread(std::function<void()>&& func) noexcept
	{
		SOUP_ASSERT(++counted_threads != 0);

		createManagedThread([func = std::move(func)]() mutable {
			auto* function = new (std::nothrow) std::function<void()>(std::move(func));

			if (function)
			{
				runExceptionalFunction(function);

				delete function;
			}

			--counted_threads;
		});
	}

	LPTOP_LEVEL_EXCEPTION_FILTER
	Exceptional::setUnhandledExceptionHandler()
	{
		if (m_ExceptionHandlerInstalled)
		{
			return m_OldExceptionFilter;
		}

		m_OldErrorMode = SetErrorMode(0);

		m_OldExceptionFilter = SetUnhandledExceptionFilter(&::exceptional_on_uncaught_exception);

		m_ExceptionHandlerInstalled = true;

		LOG(INFO) << "Unhandled exception handler installed";

		return m_OldExceptionFilter;
	}

	void Exceptional::restoreUnhandledExceptionHandler() noexcept
	{
		if (!m_ExceptionHandlerInstalled)
		{
			return;
		}

		SetUnhandledExceptionFilter(m_OldExceptionFilter);

		SetErrorMode(m_OldErrorMode);

		m_OldExceptionFilter = nullptr;

		m_OldErrorMode = 0;

		m_ExceptionHandlerInstalled = false;

		LOG(INFO) << "Unhandled exception handler restored";
	}

	void Exceptional::disableExceptionHandling() noexcept
	{
		::disable_exception_handling();

		if (thread_can_run && thread_is_running)
		{
			thread_can_run = false;

			while (thread_is_running)
			{
				soup::os::sleep(1);
			}
		}

		restoreUnhandledExceptionHandler();
	}

	void Exceptional::thread_func() noexcept
	{
		LOG(INFO) << "Exceptional worker thread started";

		thread_is_running = true;

		while (thread_can_run)
		{
			if (exceptional_has_exp())
			{
				LOG(INFO) << "Exceptional worker received exception";

				void* data = nullptr;

				handle_exception_t handler = nullptr;

				exceptional_get_exp(&data, &handler);

				const long retval = callHandlerSafely(handler, data);

				exceptional_set_retval(retval);

				exceptional_clear_exp();

				if (data)
				{
					delete static_cast<ExceptionData*>(data);
				}
			}

			soup::os::sleep(1);
		}

		thread_is_running = false;

		LOG(INFO) << "Exceptional worker thread stopped";
	}

	static Spinlock report_mtx{};


	void Exceptional::report(const std::string& type, std::string&& message, const ErrorInfo& err, void custom_footer_info(), ExceptionContext ctx) noexcept
	{
		report_mtx.lock();

		try
		{
			if (!type.empty())
			{
				LOG(INFO) << "Exception: " << type;
			}

			if (!message.empty())
			{
				LOG(INFO) << "Message: " << message;
			}

			if (ctx == ExceptionContext::LUA)
			{
				LOG(INFO) << "Exception context: LUA";
			}

			LOG(INFO) << "Stack trace:";

			if (err.is_exp && err.exp)
			{
				try
				{
					StackTrace trace;

					trace.NewStackTrace(err.exp);

					LOG(INFO) << trace.GetString();
				}
				catch (...)
				{
					LOG(INFO) << "Failed to generate stack trace.";
				}
			}
			else
			{
				LOG(INFO) << "Stack trace unavailable.";
			}

			if (custom_footer_info)
			{
				custom_footer_info();
			}
		}
		catch (...)
		{
			// Exception reporting must never propagate
		}

		report_mtx.unlock();
	}


	void Exceptional::report(const std::string& type, std::string&& message, ExceptionData* data, void custom_footer_info()) noexcept
	{
		if (!data)
		{
			return;
		}

		report(type, std::move(message), ErrorInfo(data->exp), custom_footer_info, data->ctx);
	}


	void Exceptional::report(const std::string& type, const ErrorInfo& err) noexcept
	{
		report(type, "An exception occurred.", err);
	}

	long Exceptional::handleCaughtException(ExceptionData* data) noexcept
	{
		std::string exceptionName = getExceptionName(data);

		exceptionName.insert(0, "Caught ");

		report(exceptionName, "An exception was caught.", data);

		return EXCEPTION_EXECUTE_HANDLER;
	}

	long Exceptional::handleUncaughtException(_EXCEPTION_POINTERS* exp) noexcept
	{
		std::string exceptionName = getExceptionName(exp);

		exceptionName.insert(0, "Uncaught ");

		report(exceptionName, "An unhandled exception occurred.", ErrorInfo(exp));

		return EXCEPTION_CONTINUE_SEARCH;
	}

	std::string Exceptional::getExceptionName(_EXCEPTION_POINTERS* exp) noexcept
	{
		if (!exp || !exp->ExceptionRecord)
		{
			return "UNKNOWN_EXCEPTION";
		}

		const DWORD exceptionCode = exp->ExceptionRecord->ExceptionCode;

		switch (exceptionCode)
		{
		case 0xE06D7363:
		{
			char message[1024]{};

			if (tryGetCppExceptionMessage(exp, message, sizeof(message)))
			{
				std::string result = message;

				result.insert(result.begin(), '"');

				result.push_back('"');

				return result;
			}

			return "C++ exception";
		}

		case EXCEPTION_ACCESS_VIOLATION:
		{
			std::string result = "ACCESS_VIOLATION";

			parseExceptionInformation(result, exp->ExceptionRecord->ExceptionInformation);

			return result;
		}

		case EXCEPTION_IN_PAGE_ERROR:
		{
			std::string result = "IN_PAGE_ERROR";

			parseExceptionInformation(result, exp->ExceptionRecord->ExceptionInformation);

			result.append(" (Status ");

			result.append(formatAddress(exp->ExceptionRecord->ExceptionInformation[2]));

			result.push_back(')');

			return result;
		}

		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "ARRAY_BOUNDS_EXCEEDED";

		case EXCEPTION_BREAKPOINT:
			return "BREAKPOINT";

		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "DATATYPE_MISALIGNMENT";

		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "FLT_DENORMAL_OPERAND";

		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "FLT_DIVIDE_BY_ZERO";

		case EXCEPTION_FLT_INEXACT_RESULT:
			return "FLT_INEXACT_RESULT";

		case EXCEPTION_FLT_INVALID_OPERATION:
			return "FLT_INVALID_OPERATION";

		case EXCEPTION_FLT_OVERFLOW:
			return "FLT_OVERFLOW";

		case EXCEPTION_FLT_STACK_CHECK:
			return "FLT_STACK_CHECK";

		case EXCEPTION_FLT_UNDERFLOW:
			return "FLT_UNDERFLOW";

		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "ILLEGAL_INSTRUCTION";

		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "INT_DIVIDE_BY_ZERO";

		case EXCEPTION_INT_OVERFLOW:
			return "INT_OVERFLOW";

		case EXCEPTION_INVALID_DISPOSITION:
			return "INVALID_DISPOSITION";

		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "NONCONTINUABLE_EXCEPTION";

		case EXCEPTION_PRIV_INSTRUCTION:
			return "PRIV_INSTRUCTION";

		case EXCEPTION_SINGLE_STEP:
			return "SINGLE_STEP";

		case EXCEPTION_STACK_OVERFLOW:
			return "STACK_OVERFLOW";

		default:
			return formatExceptionCode(
			    exceptionCode);
		}
	}


	std::string Exceptional::getExceptionName(const ExceptionData* data) noexcept
	{
		if (!data)
		{
			return "UNKNOWN_EXCEPTION";
		}

		return getExceptionName(data->exp);
	}

	long Exceptional::handleExceptionInErrorReporting(_EXCEPTION_POINTERS* exp) noexcept
	{
		(void)exp;

		return EXCEPTION_EXECUTE_HANDLER;
	}
}