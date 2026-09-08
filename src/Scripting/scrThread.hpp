#pragma once

#include <cstdint>
#include <cstddef>

#include "Game/fwddecl.hpp"
#include "Core/tlsContext.hpp"

namespace rage
{
	class scrThread
	{
	public:
		enum class State : std::uint32_t
		{
			IDLE,
			RUNNING,
			KILLED,
			PAUSED,
			UNK4
		};

		class Context
		{
		public:
			std::uint32_t m_ThreadId;       // 0x00
			std::uint32_t m_Padding0;       // 0x04
			std::uint64_t m_ScriptHash;     // 0x08
			State m_State;                  // 0x10
			std::uint32_t m_ProgramCounter; // 0x14
			std::uint32_t m_FramePointer;   // 0x18
			std::uint32_t m_StackPointer;   // 0x1C
			float m_TimerA;                 // 0x20
			float m_TimerB;                 // 0x24
			float m_WaitTimer;              // 0x28
			char m_Padding1[0x2C];          // 0x2C
			std::uint32_t m_StackSize;      // 0x58
			char m_Padding2[0x54];          // 0x5C

			[[nodiscard]]
			std::int64_t* GetStackPtr(void* stack) noexcept;

			[[nodiscard]]
			std::uint8_t* GetCodePtr() const noexcept;
		};

		static_assert(sizeof(Context) == 0xB0);

		virtual ~scrThread() = default;

		virtual void Reset(
		    std::uint64_t script_hash,
		    void* args,
		    std::uint32_t arg_count) = 0;

		virtual State RunImpl() = 0;

		virtual State Run() = 0;

		virtual void Kill() = 0;

		virtual void GetInfo(void* info) = 0;

		[[nodiscard]]
		static scrThread* GetRunningThread() noexcept;

		[[nodiscard]]
		std::int64_t* GetStackPtr() noexcept;

		[[nodiscard]]
		void* GetStack() const noexcept
		{
			return m_Stack;
		}

		Context m_Context;             // 0x08
		void* m_Stack;                 // 0xB8
		char m_Pad[0x4];               // 0xC0
		std::uint32_t m_ParameterSize; // 0xC4
		std::uint32_t m_ParameterLoc;  // 0xC8
		char m_Pad2[0x4];              // 0xCC
		char m_ErrorMessage[128];      // 0xD0
		std::uint32_t m_ScriptHash;    // 0x150
		char m_ScriptName[64];         // 0x154
	};

	static_assert(sizeof(scrThread) == 0x198);
}