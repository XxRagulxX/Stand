#pragma once
#include "Core/ThreadContext.hpp"
#include "Menu/ClickType.hpp"
#include "Util/Label.hpp"

#include <functional>

// Real Stand's Click also carries a chat-command/web-command issuer
// (compactplayer_t issued_by/issued_for, AbstractPlayer, edition/licensing
// checks, command-box + chat-permission plumbing) - none of which this
// project has or is building (no chat-command parser, no web bridge, no
// commercial editions). Kept only what a Self-tab-style toggle/slider
// command actually needs: click type, thread context, and a response/
// sound-feedback pair. If a later ported command needs something dropped
// here, that's the point to add it back for real, not stub it.
namespace Stand
{
	enum ClickResponseType : uint8_t
	{
		RESPONSE_CUSTOM,
		RESPONSE_GENERIC,
		RESPONSE_NONE,
	};

	enum Sound : uint8_t
	{
		SOUND_SELECT,
		SOUND_YES,
		SOUND_NO,
		SOUND_ERROR,
	};

	class Click
	{
	public:
		const ClickType type;
		ThreadContext thread_context;
		Sound sound_feedback = SOUND_SELECT;
		Label response = NOLABEL;
		ClickResponseType response_type = RESPONSE_GENERIC;
		bool response_acknowledged = false;

		explicit Click(ClickType type) noexcept;
		explicit Click(ClickType type, ThreadContext thread_context) noexcept;

		Click(const Click& b) noexcept;
		Click(Click&& b) noexcept;

		~Click();

		[[nodiscard]] Click derive(ClickType type) const noexcept;
		[[nodiscard]] Click deriveAuto() const noexcept;

		// Runs func() immediately if already on a script thread, or via
		// FiberPool::queueJob() otherwise - same shape as real Stand's own
		// (FiberPool::queueJob is already the same name/signature in this
		// project). ensureWorkerContext() also goes through FiberPool: this
		// project has no separate background-Worker thread the way Stand
		// does, so a script-thread job is the substitute.
		void ensureYieldableScriptThread(std::function<void()>&& func) const;
		void ensureScriptThread(std::function<void()>&& func) const;
		void ensureScriptThread(std::function<void(Click&)>&& func);
		void ensureWorkerContext(std::function<void()>&& func) const;

		[[nodiscard]] bool isMenu() const noexcept;
		[[nodiscard]] bool isAuto() const noexcept;
		[[nodiscard]] bool isStand() const noexcept;
		[[nodiscard]] bool isBulk() const noexcept;

		[[nodiscard]] bool hasResponse() const noexcept;
		[[nodiscard]] bool canHaveResponse() const noexcept;
		[[nodiscard]] bool canHaveGenericResponse() const noexcept;
		[[nodiscard]] bool canHaveGenericErrorResponse() const noexcept;
		void setResponse(Label&& response) noexcept;
		void setResponse(Label&& response) const;
		void setGenericResponse(Label&& response);
		void setNoResponse() noexcept;

		void ensureResponse();
		[[nodiscard]] Label getResponse();
		void forgetResponse() noexcept;
		void respond();
	};
}
