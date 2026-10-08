#pragma once

#include "Game/fwddecl.hpp"
#include "Game/TransitionState.hpp"

namespace Stand
{
	[[nodiscard]] extern bool is_session_started() noexcept;
	[[nodiscard]] extern bool is_session_started_and_transition_finished();
	[[nodiscard]] extern bool is_session_transition_active(bool when_in_doubt, bool when_almost_done = true) noexcept;
	[[nodiscard]] extern bool is_session_transition_active(const TransitionState transition_state, bool when_almost_done = true) noexcept;
	[[nodiscard]] extern bool is_session_or_transition_active(bool when_in_doubt = true) noexcept;
	[[nodiscard]] extern bool is_session_freeroam() noexcept;
}
using Stand::is_session_started;
using Stand::is_session_started_and_transition_finished;
using Stand::is_session_transition_active;
using Stand::is_session_or_transition_active;
using Stand::is_session_freeroam;
