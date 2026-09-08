#include "Core/ModuleMgr.hpp"
#include "Core/PointerCalculator.hpp"
#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LuaMainScript.hpp"
#include "Scripting/LuaUserInterface.hpp"
#include "Scripting/LuaMainUtils.hpp"
#include "Util/Joaat.hpp"
#include "Core/Pointers.hpp"

#include <soup/Pattern.hpp>
#include <soup/Range.hpp>

namespace Stand::Lua
{
	class Memory : LuaLibrary
	{
		using LuaLibrary::LuaLibrary;

		static int ScanPattern(lua_State* state)
		{
			auto sig = CheckStringSafe(state, 1);

			auto* gta5  = g_ModuleMgr.Get("GTA5_Enhanced.exe"_J);
			if (!gta5)
			{
				lua_pushnil(state);
				return 1;
			}

			soup::Range range(reinterpret_cast<void*>(gta5->Base()), gta5->Size());

			soup::Pattern pattern(sig);

			const auto result = range.scan(pattern);

			if (!result)
			{
				lua_pushnil(state);
				return 1;
			}

			PushObject<PointerCalculator>(state, PointerCalculator(result.as<void*>()));

			return 1;
		}

		static int HandleToPtr(lua_State* state)
		{
			int handle = static_cast<int>(luaL_checkinteger(state, 1));

			void* ptr = Pointers.HandleToPtr ? Pointers.HandleToPtr(handle) : nullptr;

			PushObject<PointerCalculator>(state, PointerCalculator(ptr));

			return 1;
		}

		static int PtrToHandle(lua_State* state)
		{
			auto& pc = GetObject<PointerCalculator>(state, 1);

			int handle = 0;

			if (Pointers.PtrToHandle)
			{
				handle = Pointers.PtrToHandle(pc.As<void*>());
			}

			lua_pushinteger(state, handle);

			return 1;
		}

		static int Allocate(lua_State* state)
		{
			auto size = static_cast<std::size_t>(luaL_checkinteger(state, 1));

			if (size == 0)
			{
				luaL_argerror(state, 1, "allocation size must be > 0");
			}

			void* block = std::calloc(1, size);

			if (!block)
			{
				luaL_error(state, "memory.allocate: out of memory " "(requested %zu bytes)", size);
			}

			LuaScript::GetScript(state).GetUserInterface().TrackScriptAllocation(block);

			PushObject<PointerCalculator>(state, PointerCalculator(block));

			return 1;
		}

		static int Free(lua_State* state)
		{
			auto& pc = GetObject<PointerCalculator>(state, 1);

			void* block = pc.As<void*>();

			if (!block)
			{
				return 0;
			}

			auto& iface = LuaScript::GetScript(state).GetUserInterface();

			if (!iface.ReleaseScriptAllocation(block))
			{
				luaL_error(state, "memory.free: pointer was not allocated " "by this script (or already freed)");
			}

			std::free(block);

			return 0;
		}

		virtual void Register(lua_State* state) override
		{
			lua_newtable(state);

			SetFunction(state, ScanPattern, "scan_pattern");

			SetFunction(state, HandleToPtr, "handle_to_ptr");

			SetFunction(state, PtrToHandle, "ptr_to_handle");

			SetFunction(state, Allocate, "allocate");

			SetFunction(state, Free, "free");

			lua_setglobal(state, "memory");
		}
	};

	Memory _Memory;
}