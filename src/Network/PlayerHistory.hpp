#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <soup/Promise.hpp>
#include <soup/UniquePtr.hpp>

#include "Game/fwddecl.hpp"
#include "Game/typedecl.hpp"

#include "Commands/Widgets/Command.hpp"
#include "HistoricPlayer.hpp"
#include "Core/Spinlock.hpp"

namespace Stand
{
	class PlayerHistory
	{
	public:
		static inline CommandList* player_history_command = {};
		static inline std::unique_ptr<Command> retained_player_history_command = {};
		static inline Spinlock mtx{};
		static inline std::vector<soup::UniquePtr<HistoricPlayer>> player_history = {};
		static inline soup::Promise<> loaded_data{};
		static inline soup::Promise<> inited{};
		static inline CommandDivider* divider = nullptr;
		static inline cursor_t list_offset = 0;
	};
}
