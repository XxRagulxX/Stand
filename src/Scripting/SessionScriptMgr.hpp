#pragma once
#include "Game/fwddecl.hpp"
#include "Util/hashtype.hpp"

namespace Stand
{
	class Click;

	struct SessionScriptMgr
	{
		inline static bool busy = false;
		[[nodiscard]] static bool startPrecheck(Click& click);
		static void start(const hash_t hash);

		[[nodiscard]] static int hash_to_id(const hash_t hash);
		[[nodiscard]] static hash_t id_to_hash(const int id);
	};
}
