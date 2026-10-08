#pragma once

#include <unordered_set>
#include "Game/gta_player.hpp"

namespace Stand
{
	template <class E>
	struct evtEvent
	{
		using handler_t = void(*)(E&);

		static inline std::unordered_set<handler_t> handlers;

		static void trigger(E& e)
		{
			for (const auto& handler : handlers)
			{
				handler(e);
			}
		}

		static void trigger(E&& e)
		{
			return trigger(e);
		}

		static void registerHandler(handler_t handler)
		{
			handlers.insert(handler);
		}

		static void unregisterHandler(handler_t handler)
		{
			handlers.erase(handler);
		}
	};

	struct evtHostChangeEvent : public evtEvent<evtHostChangeEvent>
	{
		const compactplayer_t cur;

		explicit evtHostChangeEvent(compactplayer_t cur)
			: cur(cur)
		{
		}
	};
}
