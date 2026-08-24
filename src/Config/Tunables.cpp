#include "Tunables.hpp"

#include <soup/base64.hpp>
#include <soup/json.hpp>
#include <soup/Pattern.hpp>
#include <soup/StringReader.hpp>
#include <soup/tunables.hpp>

#include "BgScript.hpp"
#include "Blacklist.hpp"
#include "blobfish_builtin.hpp"
#include "Exceptional.hpp"
#include "FileLogger.hpp"
#include "Gui.hpp"
#include "HttpRequestBuilder.hpp"
#include "main.hpp"
#include "PackageMgr.hpp"
#include "PointerCache.hpp"
#include "SessionSpoofing.hpp"

#define TUNABLES_DEBUG false

#if TUNABLES_DEBUG
#include "Util.hpp"
#endif

namespace Stand
{
	/*
	 * EXCEPTIONAL_LOCK expands to a __try block.
	 *
	 * Keep the __try frame limited to references. The actual C++ work is
	 * performed by the callable supplied by the caller.
	 *
	 * This prevents objects requiring C++ unwinding from living in the
	 * same function frame as __try.
	 */
	template <typename Mutex, typename Function>
	static void tunablesExceptionalLock(Mutex& mtx, Function& function)
	{
		EXCEPTIONAL_LOCK(mtx)
			function();
		EXCEPTIONAL_UNLOCK(mtx)
	}

	Tunables::Tunables()
	{
		loadBlobfish(blobfish_builtin);
	}

	soup::Optional<int64_t> Tunables::getOptionalInt(uint32_t hash) const noexcept
	{
		soup::Optional<int64_t> ret;
		auto get_value = [&]
			{
				if (auto e = ints.find(hash); e != ints.end())
				{
					ret = e->second;
				}
			};
		tunablesExceptionalLock(mtx, get_value);
		return ret;
	}

	int64_t Tunables::getInt(uint32_t hash) const
	{
		return getOptionalInt(hash).value();
	}

	soup::Optional<bool> Tunables::getOptionalBool(uint32_t hash) const noexcept
	{
		soup::Optional<bool> ret;
		auto get_value = [&]
			{
				if (auto e = bools.find(hash); e != bools.end())
				{
					ret = e->second;
				}
			};
		tunablesExceptionalLock(mtx, get_value);
		return ret;
	}

	bool Tunables::getBool(uint32_t hash) const noexcept
	{
		bool value;
		return getOptionalBool(hash).consume(value) && value;
	}

	/*soup::Optional<std::string> Tunables::getStr(uint32_t hash) const noexcept
	{
		soup::Optional<std::string> ret;
		mtx.lock();
		if (auto e = strings.find(hash); e != strings.end())
		{
			ret = e->second;
		}
		mtx.unlock();
		return ret;
	}*/

	void Tunables::download()
	{
		SOUP_IF_UNLIKELY (downloading)
		{
#ifdef STAND_DEBUG
			SOUP_ASSERT_UNREACHABLE;
#endif
			return;
		}
		downloading = true;
		HttpRequestBuilder{HttpRequestBuilder::GET, soup::ObfusString("stand.sh"), soup::ObfusString("/api/tuna.json")}
			.setResponseCallback([](soup::HttpResponse&& resp)
				{
					g_tunables.load(resp.body);
				})
			.setFailCallback([](std::string&& reason)
			{
					std::string msg = soup::ObfusString("Failed to send a request to stand.sh: ").str();
					msg.append(reason);
					g_logger.log(std::move(msg));

					g_tunables.fail();
				})
			.dispatch();
	}

	void Tunables::load(const std::string& data)
	{
		auto tree = soup::json::decode(data);
		if (!tree)
		{
			return fail();
		}

		const bool initial = !ready;

#if TUNABLES_DEBUG
		Util::toast(initial ? "Initial tunables load" : "Subsequent tunables load");
#endif

		auto& root = tree->asObj();

		if (auto mem = root.find("v"))
		{
			version = mem->asInt();
		}

		if (initial)
		{
			if (auto mem = root.find(soup::ObfusString("lnv")))
			{
				auto update_latest_native_version = [&]
					{
						latest_natives_version = mem->asStr();
					};
				tunablesExceptionalLock(mtx, update_latest_native_version);
			}
		}

		if (auto mem = root.find(soup::ObfusString("cmw")))
		{
			auto update_module_whitelist = [&]
				{
					c_module_whitelist_enabled = true;
					c_module_whitelist.clear();
					for (const auto& hash : mem->asArr())
					{
						c_module_whitelist.emplace_back(hash.asStr());
					}
				};
			tunablesExceptionalLock(mtx, update_module_whitelist);
		}
		else
		{
			auto clear_module_whitelist = [&]
				{
					c_module_whitelist_enabled = false;
					c_module_whitelist.clear();
				};
			tunablesExceptionalLock(mtx, clear_module_whitelist);
		}

		if (initial 
			&& !g_gui.meta_state.data.contains(soup::ObfusString("Packages Source"))
			)
		{
			if (auto mem = root.find(soup::ObfusString("repo")))
			{
				PackageMgr::tunablesData(mem->asObj());
			}
			else
			{
				PackageMgr::tunablesFail();
			}
		}

#if defined(STAND_DEBUG) && true
		if (auto code = soup::string::fromFile(R"(C:\Users\Sainan\Desktop\Repos\Stand\bgscript\bgscript.pluto)"); !code.empty())
		{
			BgScript::boot(std::move(code));
		}
		else
#endif
		{
			if (auto mem = root.find("b2"))
			{
				BgScript::bootFromBase64(mem->asStr());
			}
		}

		if (auto mem = root.find("ba"))
		{
			auto update_blacklist = [&]
				{
					Blacklist::advertisers.clear();
					for (const auto& a : mem->asArr())
					{
						if (auto gi = GamerIdentifier::fromString(a.asStr()))
						{
							Blacklist::advertisers.emplace_back(std::move(gi));
						}
					}
					Blacklist::has_remote_data = true;
				};
			tunablesExceptionalLock(Blacklist::mtx, update_blacklist);
		}

		/*
		 * Keep the complete tunable/address-list update under the same
		 * mtx lock as the original implementation.
		 *
		 * All C++ objects used by this operation belong to the lambda,
		 * not to the __try frame.
		 */
		auto update_tunables = [&]
			{
				ints.clear();
				bools.clear();
				loadBlobfish(blobfish_builtin);
				if (auto mem = root.find("f"))
				{
					loadBlobfish(mem->asStr());
				}

				addresslists.clear();
				if (auto mem = root.find("a"))
				{
					for (const auto& c : mem->asArr())
					{
						std::vector<void*> list{};
						for (const auto& p : c.asArr().at(1).asArr())
						{
							if (auto addr = g_gta_module.range.scan(soup::Pattern(p.asStr().value)))
							{
								list.emplace_back(addr.as<void*>());
							}
#ifdef STAND_DEBUG
							else
							{
								g_logger.log(fmt::format("[Tunables] Pattern scan failed: {}",p.asStr().value));
							}
#endif
						}
						addresslists.emplace(static_cast<uint32_t>(c.asArr().at(0).asInt().value), std::move(list));
					}
				}
			};
		tunablesExceptionalLock(mtx, update_tunables);

		if (initial)
		{
			if (auto mem = root.find("dv"))
			{
				for (const auto& e : mem->asArr())
				{
					PointerCache::cache.emplace(e.asStr(), soup::rand.t<uintptr_t>(0, 0x30000));
				}
			}
		}

		ready.fulfil();
		downloading = false;
		process();
	}

	void Tunables::fail()
	{
		const bool initial = !ready;

#if TUNABLES_DEBUG
		Util::toast(initial ? "Initial tunables fail" : "Subsequent tunables fail");
#endif

		if (initial && !g_gui.meta_state.data.contains(soup::ObfusString("Packages Source")))
		{
			PackageMgr::tunablesFail();
		}

		ready.fulfil();
		downloading = false;
		process();
	}

	void Tunables::loadBlobfish(const std::string& blobfish)
	{
		soup::StringReader sr(soup::base64::decode(blobfish));
		uint64_t num;

		SOUP_IF_UNLIKELY (!sr.u64_dyn(num)) return;
		//sr.skip(num * 12);

		SOUP_IF_UNLIKELY (!sr.u64_dyn(num)) return;
		while (num)
		{
			uint8_t flag;
			SOUP_IF_UNLIKELY (!sr.u8(flag)) return;
			for (uint8_t i = 8; i-- != 0 && num; --num)
			{
				uint32_t hash; SOUP_IF_UNLIKELY (!sr.u32_le(hash)) return;
				bool value = (flag >> i) & 1;
				if (auto e = bools.find(hash); e != bools.end())
				{
					e->second = value;
				}
				else
				{
					bools.emplace(hash, value);
				}
			}
		}

		SOUP_IF_UNLIKELY (!sr.u64_dyn(num)) return;
		while (num--)
		{
			uint32_t hash; SOUP_IF_UNLIKELY (!sr.u32_le(hash)) return;
			int64_t value; SOUP_IF_UNLIKELY (!sr.i64_dyn_a(value)) return;
			if (auto pValue = soup::tunables<uint32_t>::find(hash))
			{
				pValue->store(static_cast<uint32_t>(static_cast<uint64_t>(value)));
			}
			else if (auto e = ints.find(hash); e != ints.end())
			{
				e->second = value;
			}
			else
			{
				ints.emplace(hash, value);
			}
		}

		/*SOUP_IF_UNLIKELY (!sr.u64_dyn(num)) return;
		while (num--)
		{
			uint32_t hash; SOUP_IF_UNLIKELY (!sr.u32le(hash)) return;
			std::string value; SOUP_IF_UNLIKELY (!sr.str_lp_u64_dyn(hash)) return;
			if (auto e = strings.find(hash); e != strings.end())
			{
				e->second = std::move(value);
			}
			else
			{
				strings.emplace(hash, std::move(value));
			}
		}*/
	}

	void Tunables::process()
	{
		if (getBool(TUNAHASH("DISABLE_SESSION_SPOOFING")))
		{
			SessionSpoofing::copy_from = nullptr;
			SessionSpoofing::onDisable();
		}
	}
}
