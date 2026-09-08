#pragma once

#include <Windows.h>

#include <cstdint>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace Stand
{
	class StackTrace
	{
	public:
		StackTrace();
		~StackTrace();

		StackTrace(const StackTrace&) = delete;
		StackTrace& operator=(const StackTrace&) = delete;

		[[nodiscard]]
		const std::vector<uint64_t>& GetFramePointers() const noexcept;

		void NewStackTrace(
		    EXCEPTION_POINTERS* exception_info);

		[[nodiscard]]
		std::string GetString() const;

		void Clear() noexcept;

		friend std::ostream& operator<<(
		    std::ostream& os,
		    const StackTrace& st);

		friend std::ostream& operator<<(
		    std::ostream& os,
		    const StackTrace* st);

	private:
		struct ModuleInfo
		{
			ModuleInfo(
			    std::filesystem::path path,
			    void* base);

			std::string m_Name;
			uintptr_t m_Base = 0;
			size_t m_Size = 0;
		};

	private:
		void DumpModuleInfo();
		void DumpRegisters();
		void DumpStacktrace();
		void DumpExceptionInfo();
		void GrabStacktrace();

		[[nodiscard]]
		const ModuleInfo* GetModuleByAddress(
		    uint64_t addr) const;

		[[nodiscard]]
		static std::string ExceptionCodeToString(
		    DWORD code);

	private:
		EXCEPTION_POINTERS* m_ExceptionInfo = nullptr;

		std::stringstream m_Dump;
		std::vector<uint64_t> m_FramePointers;

		inline static std::vector<ModuleInfo> m_Modules;
	};

	inline std::ostream& operator<<(
	    std::ostream& os,
	    const StackTrace& st)
	{
		os << st.GetString();
		return os;
	}

	inline std::ostream& operator<<(
	    std::ostream& os,
	    const StackTrace* st)
	{
		if (st)
			os << st->GetString();

		return os;
	}
}