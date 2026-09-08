#include "Menu/Click.hpp"

#include "Scripting/FiberPool.hpp"
#include "Rendering/Notifications.hpp"

namespace Stand
{
	Click::Click(ClickType type) noexcept :
	    Click(type, TC_OTHER)
	{
	}

	Click::Click(ClickType type, ThreadContext thread_context) noexcept :
	    type(type),
	    thread_context(thread_context)
	{
	}

	Click::Click(const Click& b) noexcept :
	    type(b.type),
	    thread_context(b.thread_context),
	    sound_feedback(b.sound_feedback),
	    response(b.response),
	    response_type(b.response_type),
	    response_acknowledged(b.response_acknowledged)
	{
	}

	Click::Click(Click&& b) noexcept :
	    type(b.type),
	    thread_context(b.thread_context),
	    sound_feedback(b.sound_feedback),
	    response(std::move(b.response)),
	    response_type(b.response_type),
	    response_acknowledged(b.response_acknowledged)
	{
		b.forgetResponse();
	}

	Click::~Click()
	{
		if (!response_acknowledged && response_type == RESPONSE_CUSTOM)
			respond();
	}

	Click Click::derive(ClickType type) const noexcept
	{
		return Click{type, thread_context};
	}

	Click Click::deriveAuto() const noexcept
	{
		return derive(CLICK_AUTO);
	}

	void Click::ensureYieldableScriptThread(std::function<void()>&& func) const
	{
		if (thread_context == TC_SCRIPT_YIELDABLE)
			func();
		else
			Stand::FiberPool::queueJob(std::move(func));
	}

	void Click::ensureScriptThread(std::function<void()>&& func) const
	{
		if (thread_context_is_script(thread_context))
			func();
		else
			Stand::FiberPool::queueJob(std::move(func));
	}

	void Click::ensureScriptThread(std::function<void(Click&)>&& func)
	{
		if (thread_context_is_script(thread_context))
		{
			func(*this);
		}
		else
		{
			Stand::FiberPool::queueJob([click{*this}, func{std::move(func)}]() mutable {
				click.thread_context = TC_SCRIPT_YIELDABLE;
				func(click);
			});
		}
	}

	void Click::ensureWorkerContext(std::function<void()>&& func) const
	{
		Stand::FiberPool::queueJob(std::move(func));
	}

	bool Click::isMenu() const noexcept
	{
		return type == CLICK_MENU;
	}

	bool Click::isAuto() const noexcept
	{
		return (type & CLICK_FLAG_AUTO) != 0;
	}

	bool Click::isStand() const noexcept
	{
		switch (type)
		{
		case CLICK_BULK:
		case CLICK_AUTO:
			return true;
		}
		return false;
	}

	bool Click::isBulk() const noexcept
	{
		return type == CLICK_BULK;
	}

	bool Click::hasResponse() const noexcept
	{
		return !response.empty() || response_type == RESPONSE_NONE;
	}

	bool Click::canHaveResponse() const noexcept
	{
		return type != CLICK_BULK;
	}

	bool Click::canHaveGenericResponse() const noexcept
	{
		return !isAuto();
	}

	bool Click::canHaveGenericErrorResponse() const noexcept
	{
		return canHaveGenericResponse();
	}

	void Click::setResponse(Label&& response) noexcept
	{
		this->response = std::move(response);
		this->response_type = RESPONSE_CUSTOM;
	}

	void Click::setResponse(Label&& response) const
	{
		Click(*this).setResponse(std::move(response));
	}

	void Click::setGenericResponse(Label&& response)
	{
		if (!hasResponse())
		{
			this->response = std::move(response);
			this->response_type = RESPONSE_GENERIC;
		}
	}

	void Click::setNoResponse() noexcept
	{
		response.reset();
		response_type = RESPONSE_NONE;
	}

	void Click::ensureResponse()
	{
		if (!hasResponse() && canHaveGenericResponse())
			setGenericResponse(LIT("Done"));
	}

	Label Click::getResponse()
	{
		response_acknowledged = true;
		return response;
	}

	void Click::forgetResponse() noexcept
	{
		response.reset();
		response_type = RESPONSE_GENERIC;
		response_acknowledged = false;
	}

	void Click::respond()
	{
		if (canHaveResponse() && !response.empty())
		{
			response_acknowledged = true;

			// No title - matches real Stand's own Util::toast(response, ...)
			// exactly (confirmed against origin/stand-reference's own
			// Click::respond()): its toasts have no separate title
			// concept at all, just the response text itself. The real
			// bug behind "no toast ever shows up" for every Click-driven
			// response (menu click, hotkey, command console alike - none
			// of it was actually broken by any of those, this was) turned
			// out to be Notifications::ShowImpl() (Rendering/
			// Notifications.cpp) requiring BOTH title and message
			// non-empty, when an empty title was already a fully
			// supported, deliberate look downstream (see that function's
			// own comment) - fixed there instead of by giving every
			// Click-driven toast a generic "StandEnhanced" title it never
			// needed (a previous pass here's own workaround, undone now
			// that the real cause is fixed at its actual source).
			Stand::Notifications::Show("", response.getLocalisedUtf8(), Stand::NotificationType::Info);
		}
	}
}
