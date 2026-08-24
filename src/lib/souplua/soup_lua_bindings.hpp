#pragma once

#include <memory>

// assuming <lua.h> & <lauxlib.h> are already included!

#include <soup/audDevice.hpp>
#include <soup/audMixer.hpp>
#include <soup/audPlayback.hpp>
#include <soup/audWav.hpp>
#include <soup/country_names.hpp>
#include <soup/FileReader.hpp>
#include <soup/joaat.hpp>
#include <soup/IpAddr.hpp>
#include <soup/Matrix.hpp>
#include <soup/netIntel.hpp>
#include <soup/StringReader.hpp>
#include <soup/Vector3.hpp>
#include <soup/ZipReader.hpp>

namespace soup
{
	// If you're not using Pluto, your compiler might raise warnings because the error functions don't have the [[noreturn]] attribute in stock lua.

	struct LuaBindings
	{
#define pushNewAndBeginMt(T, ...) pushNewAndBeginMtImpl<T>(L, "soup::" #T, __VA_ARGS__);

#pragma region C++ API

#ifdef PLUTO_VERSION
		static constexpr auto PLUTO_MAJOR = (PLUTO_VERSION[6] - '0');
		static constexpr auto PLUTO_MINOR = (PLUTO_VERSION[8] - '0');
		static constexpr auto PLUTO_PATCH = (PLUTO_VERSION[10] - '0');
#endif

		struct DataProvider
		{
			virtual netIntel& getNetIntel(lua_State* L)
			{
				Exception::purecall();
			}
		};

		static inline UniquePtr<DataProvider> data_provider{};

		static void open(lua_State* L)
		{
			if (!data_provider)
			{
				data_provider = soup::make_unique<DataProvider>();
			}

			open_pushTable(L);

			lua_setglobal(L, "soup");
		}

		static void open_pushTable(lua_State* L)
		{
			lua_newtable(L);

			open_setAudioFields(L);
			open_setIoFields(L);
			open_setMathFields(L);
			open_setNetFields(L);
		}
#pragma endregion C++ API

#pragma region Lua API - Audio
		static void open_setAudioFields(lua_State* L)
		{
			// audDevice
			{
				const luaL_Reg functions[] = {
					{"getDefault", &lua_audDevice_getDefault},
					{nullptr, nullptr}
				};
				luaL_newlib(L, functions);
				lua_setfield(L, -2, "audDevice");
			}

			lua_pushcfunction(L, &lua_audMixer);
			lua_setfield(L, -2, "audMixer");

			lua_pushcfunction(L, &lua_audWav);
			lua_setfield(L, -2, "audWav");
		}

		static int lua_audDevice_getDefault(lua_State* L)
		{
			return tryCatch(L, [](lua_State* L)
			{
				pushNewAndBeginMt(audDevice, audDevice::getDefault());
				{
					lua_pushstring(L, "__index");
					lua_pushcfunction(L, [](lua_State* L) -> int
					{
						switch (joaat::hash(luaL_checkstring(L, 2)))
						{
						case joaat::hash("getName"):
							lua_pushcfunction(L, [](lua_State* L) -> int
							{
								pushString(L, reinterpret_cast<audDevice*>(lua_touserdata(L, 1))->getName());
								return 1;
							});
							return 1;

						case joaat::hash("open"):
							lua_pushcfunction(L, [](lua_State* L) -> int
							{
								auto pb = pushNewAudPlayback(L);
								pb->open(*reinterpret_cast<audDevice*>(lua_touserdata(L, 1)), (int)luaL_optinteger(L, 2, 1));
								return 1;
							});
							return 1;
						}
						return 0;
					});
					lua_settable(L, -3);
				}
				lua_setmetatable(L, -2);
				return 1;
			});
		}

		static int lua_audMixer(lua_State* L)
		{
			pushNewAndBeginMt(audMixer);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("stop_playback_when_done"):
						lua_pushboolean(L, reinterpret_cast<audMixer*>(lua_touserdata(L, 1))->stop_playback_when_done);
						return 1;

					case joaat::hash("setOutput"):
						lua_pushcfunction(L, [](lua_State* L) -> int
						{
							checkTypename(L, 2, "soup::audPlayback");
							reinterpret_cast<audMixer*>(lua_touserdata(L, 1))->setOutput(*reinterpret_cast<audPlayback*>(lua_touserdata(L, 2)));
							return 0;
						});
						return 1;

					case joaat::hash("playSound"):
						lua_pushcfunction(L, [](lua_State* L) -> int
						{
							checkTypeExtendsAudSound(L, 2);
							return tryCatch(L, [](lua_State* L)
							{
								reinterpret_cast<audMixer*>(lua_touserdata(L, 1))->playSound(*reinterpret_cast<SharedPtr<audSound>*>(lua_touserdata(L, 2)));
								return 0;
							});
						});
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__newindex");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("stop_playback_when_done"):
						reinterpret_cast<audMixer*>(lua_touserdata(L, 1))->stop_playback_when_done = lua_toboolean(L, 2);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_audWav(lua_State* L)
		{
			checkTypeExtendsReader(L, 1);
			return tryCatch(L, [](lua_State* L)
			{
				pushNewAndBeginMt(SharedPtr<soup::audWav>, soup::make_shared<audWav>(*reinterpret_cast<soup::Reader*>(lua_touserdata(L, 1))));
				{
					lua_pushstring(L, "__index");
					lua_pushcfunction(L, [](lua_State* L) -> int
					{
						switch (joaat::hash(luaL_checkstring(L, 2)))
						{
						case joaat::hash("channels"):
							lua_pushinteger(L, (*reinterpret_cast<SharedPtr<audWav>*>(lua_touserdata(L, 1)))->channels);
							return 1;
						}
						return 0;
					});
					lua_settable(L, -3);
				}
				lua_setmetatable(L, -2);
				return 1;
			});
		}
#pragma endregion Lua API - Audio

#pragma region Lua API - Net
		static void open_setNetFields(lua_State* L)
		{
			// netIntel
			{
				const luaL_Reg functions[] = {
					{"getAsByIp", &lua_netIntel_getAsByIp},
					{"getLocationByIp", &lua_netIntel_getLocationByIp},
					{nullptr, nullptr}
				};
				luaL_newlib(L, functions);
				lua_setfield(L, -2, "netIntel");
			}

			lua_pushcfunction(L, &lua_getCountryName);
			lua_setfield(L, -2, "getCountryName");

			lua_pushcfunction(L, &lua_IpAddr);
			lua_setfield(L, -2, "IpAddr");
		}

		static int lua_netIntel_getAsByIp(lua_State* L)
		{
			return tryCatch(L, [](lua_State* L)
			{
				auto as = data_provider->getNetIntel(L).getAsByIp(checkIpAddr(L, 1));
				if (!as)
				{
					return 0;
				}
				pushNewAndBeginMt(netAs, *as);
				{
					lua_pushstring(L, "__index");
					lua_pushcfunction(L, [](lua_State* L) -> int
					{
						switch (joaat::hash(luaL_checkstring(L, 2)))
						{
						case joaat::hash("isValid"):
							lua_pushcfunction(L, &lua_mm_isValid);
							return 1;

						case joaat::hash("number"):
							lua_pushinteger(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->number);
							return 1;

						case joaat::hash("handle"):
							lua_pushstring(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->handle);
							return 1;

						case joaat::hash("name"):
							lua_pushstring(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->name);
							return 1;

						case joaat::hash("isHosting"):
							lua_pushcfunction(L, [](lua_State* L) -> int
							{
								lua_pushboolean(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->isHosting(data_provider->getNetIntel(L)));
								return 1;
							});
							return 1;
						}
						return 0;
					});
					lua_settable(L, -3);
				}
				lua_setmetatable(L, -2);
				return 1;
			});
		}

		static int lua_netIntel_getLocationByIp(lua_State* L)
		{
			return tryCatch(L, [](lua_State* L)
			{
				auto location = data_provider->getNetIntel(L).getLocationByIp(checkIpAddr(L, 1));
				if (!location)
				{
					return 0;
				}
				pushNewAndBeginMt(netIntelLocationData, *location);
				{
					lua_pushstring(L, "__index");
					lua_pushcfunction(L, [](lua_State* L) -> int
					{
						switch (joaat::hash(luaL_checkstring(L, 2)))
						{
						case joaat::hash("isValid"):
							lua_pushcfunction(L, &lua_mm_isValid);
							return 1;

						case joaat::hash("city"):
							lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->city);
							return 1;

						case joaat::hash("state"):
							lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->state);
							return 1;

						case joaat::hash("country_code"):
							lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->country_code.c_str());
							return 1;
						}
						return 0;
					});
					lua_settable(L, -3);
				}
				lua_setmetatable(L, -2);
				return 1;
			});
		}

		static int lua_getCountryName(lua_State* L)
		{
			if (lua_gettop(L) >= 2)
			{
				pushString(L, getCountryName(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
			}
			else
			{
				pushString(L, getCountryName(luaL_checkstring(L, 1)));
			}
			return 1;
		}

		static int lua_IpAddr(lua_State* L)
		{
			pushNewAndBeginMt(IpAddr, checkIpAddr(L, 1));
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("getReverseDns"):
						lua_pushcfunction(L, [](lua_State* L) -> int
						{
							pushString(L, reinterpret_cast<IpAddr*>(lua_touserdata(L, 1))->getReverseDns());
							return 1;
						});
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__tostring");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					pushString(L, reinterpret_cast<IpAddr*>(lua_touserdata(L, 1))->toString());
					return 1;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}
#pragma endregion Lua API - Net

#pragma region Lua API - Math
		static void open_setMathFields(lua_State* L)
		{
			lua_pushcfunction(L, &lua_Matrix);
			lua_setfield(L, -2, "Matrix");

			lua_pushcfunction(L, &lua_Vector3);
			lua_setfield(L, -2, "Vector3");
		}

		static int lua_Matrix(lua_State* L)
		{
			pushNewAndBeginMt(Matrix);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("setPosRotXYZ"):
						lua_pushcfunction(L, &lua_Matrix_setPosRotXYZ);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__mul");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					checkTypename(L, 2, "soup::Vector3");
					Matrix& m = *reinterpret_cast<Matrix*>(lua_touserdata(L, 1));
					*pushNewVector3(L) = (m * *reinterpret_cast<Vector3*>(lua_touserdata(L, 2)));
					return 1;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_Matrix_setPosRotXYZ(lua_State* L)
		{
			Vector3 pos, rot;
			if (lua_gettop(L) == 3)
			{
				checkTypename(L, 2, "soup::Vector3"); pos = *reinterpret_cast<Vector3*>(lua_touserdata(L, 2));
				checkTypename(L, 3, "soup::Vector3"); rot = *reinterpret_cast<Vector3*>(lua_touserdata(L, 3));
			}
			else
			{
				pos.x = (float)luaL_checknumber(L, 2);
				pos.y = (float)luaL_checknumber(L, 3);
				pos.z = (float)luaL_checknumber(L, 4);
				rot.x = (float)luaL_checknumber(L, 5);
				rot.y = (float)luaL_checknumber(L, 6);
				rot.z = (float)luaL_checknumber(L, 7);
			}
			reinterpret_cast<Matrix*>(lua_touserdata(L, 1))->setPosRotXYZ(pos, rot);
			return 0;
		}

		static int lua_Vector3(lua_State* L)
		{
			const auto args = lua_gettop(L);
			auto v = pushNewVector3(L);
			if (args >= 3)
			{
				v->x = (float)luaL_checknumber(L, 1);
				v->y = (float)luaL_checknumber(L, 2);
				v->z = (float)luaL_checknumber(L, 3);
			}
			return 1;
		}
#pragma endregion Lua API - Math

#pragma region Lua API - I/O
		static void open_setIoFields(lua_State* L)
		{
			lua_pushcfunction(L, &lua_FileReader);
			lua_setfield(L, -2, "FileReader");

			lua_pushcfunction(L, &lua_StringReader);
			lua_setfield(L, -2, "StringReader");

			lua_pushcfunction(L, &lua_ZipReader);
			lua_setfield(L, -2, "ZipReader");
		}

		static int lua_FileReader(lua_State* L)
		{
			pushNewAndBeginMt(FileReader, luaL_checkstring(L, 1));
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_StringReader(lua_State* L)
		{
			pushNewAndBeginMt(StringReader, checkString(L, 1));
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_ZipReader(lua_State* L)
		{
			checkTypeExtendsReader(L, 1);
			pushNewAndBeginMt(ZipReader, *reinterpret_cast<soup::Reader*>(lua_touserdata(L, 1)));
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("getFileList"):
						lua_pushcfunction(L, &lua_ZipReader_getFileList);
						return 1;

					case joaat::hash("getFileContents"):
						lua_pushcfunction(L, &lua_ZipReader_getFileContents);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_ZipReader_getFileList(lua_State* L)
		{
			lua_newtable(L);
			size_t i = 1;
			for (const auto& f : reinterpret_cast<soup::ZipReader*>(lua_touserdata(L, 1))->getFileList())
			{
				lua_pushinteger(L, i++);
				lua_newtable(L);
				{
					pushString(L, "name");
					pushString(L, f.name);
					lua_settable(L, -3);
				}
				{
					pushString(L, "offset");
					lua_pushinteger(L, f.offset);
					lua_settable(L, -3);
				}
				lua_settable(L, -3);
			}
			return 1;
		}

		static int lua_ZipReader_getFileContents(lua_State* L)
		{
			return tryCatch(L, [](lua_State* L)
			{
				uint32_t offset;
				if (lua_type(L, 2) == LUA_TTABLE)
				{
					if (lua_getfield(L, 2, "offset"))
					{
						offset = (uint32_t)luaL_checkinteger(L, -1);
						lua_pop(L, 1);
					}
					else
					{
						offset = (uint32_t)luaL_checkinteger(L, 2);
					}
				}
				else
				{
					offset = (uint32_t)luaL_checkinteger(L, 2);
				}

				pushString(L, reinterpret_cast<soup::ZipReader*>(lua_touserdata(L, 1))->getFileContents(offset));
				return 1;
			});
		}
#pragma endregion Lua API - I/O

#pragma region Lua Helpers
		[[nodiscard]] static IpAddr checkIpAddr(lua_State* L, int i)
		{
			if (lua_type(L, i) == LUA_TSTRING)
			{
				IpAddr addr;
				if (!addr.fromString(lua_tostring(L, i)))
				{
					luaL_argerror(L, i, "invalid ip address");
				}
				return addr;
			}
			else if (lua_type(L, i) == LUA_TUSERDATA)
			{
				return *(IpAddr*)lua_touserdata(L, 1);
			}
			return IpAddr(native_u32_t((uint32_t)luaL_checkinteger(L, i)));
		}

		static void pushMediumUserdata(lua_State* L, const void* ud)
		{
			*(const void**)lua_newuserdata(L, sizeof(const void*)) = ud;
		}

		static void checkType(lua_State* L, int i, int tp)
		{
			if (lua_type(L, i) != tp)
			{
				luaL_typeerror(L, i, lua_typename(L, tp));
			}
		}

		[[nodiscard]] static void* checkMediumUserdataNullable(lua_State* L, int i)
		{
			checkType(L, i, LUA_TUSERDATA);
			return *(void**)lua_touserdata(L, i);
		}

		[[nodiscard]] static void* checkMediumUserdata(lua_State* L, int i)
		{
			auto ud = checkMediumUserdataNullable(L, i);
			if (ud == nullptr)
			{
				luaL_error(L, "nullptr access");
			}
			return ud;
		}

		static int lua_mm_isValid(lua_State* L)
		{
			lua_pushboolean(L, checkMediumUserdataNullable(L, 1) != nullptr);
			return 1;
		}

		template <typename T, typename...Args>
		static T* pushNew(lua_State* L, Args&&...args)
		{
			return std::construct_at((T*)lua_newuserdata(L, sizeof(T)), std::forward<Args>(args)...);
		}

		template <typename T, typename...Args>
		static T* pushNewAndBeginMtImpl(lua_State* L, const char* tn, Args&&...args)
		{
			auto inst = pushNew<T>(L, std::forward<Args>(args)...);
			lua_newtable(L);
			addDtorToMt<T>(L);
			{
				lua_pushstring(L, "__name");
				lua_pushstring(L, tn);
				lua_settable(L, -3);
			}
			return inst;
		}

		template <typename T>
		static void addDtorToMt(lua_State* L)
		{
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, [](lua_State* L) -> int
			{
				std::destroy_at((T*)lua_touserdata(L, 1));
				return 0;
			});
			lua_settable(L, -3);
		}

		[[nodiscard]] static const char* getTypename(lua_State* L, int i)
		{
			const char* ret = nullptr;
			if (lua_getmetatable(L, i))
			{
				if (lua_getfield(L, -1, "__name"))
				{
					ret = lua_tostring(L, -1);
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
			}
			return ret;
		}

		static Vector3* pushNewVector3(lua_State* L)
		{
			auto v = pushNewAndBeginMt(Vector3);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("x"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->x);
						return 1;

					case joaat::hash("y"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->y);
						return 1;

					case joaat::hash("z"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->z);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__newindex");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("x"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->x = (float)luaL_checknumber(L, 3);
						return 1;

					case joaat::hash("y"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->y = (float)luaL_checknumber(L, 3);
						return 1;

					case joaat::hash("z"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->z = (float)luaL_checknumber(L, 3);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return v;
		}

		[[nodiscard]] static bool isTypename(lua_State* L, int i, const char* tn)
		{
			auto val_tn = getTypename(L, i);
			return val_tn != nullptr
				&& strcmp(val_tn, tn) == 0
				;
		}

		static void checkTypename(lua_State* L, int i, const char* tn)
		{
			if (!isTypename(L, i, tn))
			{
				luaL_typeerror(L, i, tn);
			}
		}

		[[nodiscard]] static std::string checkString(lua_State* L, int i)
		{
			size_t size;
			const char* data = luaL_checklstring(L, i, &size);
			return std::string(data, size);
		}

		static void pushString(lua_State* L, const char* str)
		{
			lua_pushstring(L, str);
		}

		static void pushString(lua_State* L, const std::string& str)
		{
			lua_pushlstring(L, str.data(), str.size());
		}

		static bool isIndexBasedTable(lua_State* L, int i)
		{
			lua_pushvalue(L, i);
			lua_pushnil(L);
			size_t k = 1;
			for (; lua_next(L, -2); ++k)
			{
				lua_pushvalue(L, -2);
				if (lua_type(L, -1) != LUA_TNUMBER)
				{
					lua_pop(L, 4);
					return false;
				}
				if (!lua_isinteger(L, -1))
				{
					lua_pop(L, 4);
					return false;
				}
				if (lua_tointeger(L, -1) != k)
				{
					lua_pop(L, 4);
					return false;
				}
				lua_pop(L, 2);
			}
			lua_pop(L, 1);
			return k != 1; // say it's not an index based table if it's empty
		}

		static int tryCatch(lua_State* L, lua_CFunction f)
		{
			try
			{
				return f(L);
			}
			catch(std::exception& e)
			{
				luaL_error(L, e.what());
			}
		}

		static void checkTypeExtendsReader(lua_State* L, int i)
		{
			if (!isTypename(L, i, "soup::FileReader")
				&& !isTypename(L, i, "soup::StringReader")
				)
			{
				luaL_typeerror(L, 1, "soup::Reader");
			}
		}

		static void checkTypeExtendsAudSound(lua_State* L, int i)
		{
			if (!isTypename(L, i, "soup::SharedPtr<soup::audWav>"))
			{
				luaL_typeerror(L, 1, "soup::SharedPtr<soup::audSound>");
			}
		}

		static audPlayback* pushNewAudPlayback(lua_State* L)
		{
			auto v = pushNewAndBeginMt(audPlayback);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("isPlaying"):
						lua_pushcfunction(L, [](lua_State* L)
						{
							lua_pushboolean(L, reinterpret_cast<audPlayback*>(lua_touserdata(L, 1))->isPlaying());
							return 1;
						});
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return v;
		}
#pragma endregion Lua Helpers

#undef pushNewAndBeginMt
	};
}
