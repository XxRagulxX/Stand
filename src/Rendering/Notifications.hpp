#pragma once

#include "Rendering/NotifySettings.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

namespace Stand
{
	// H-space units (Stand's own virtual 1920x1080 HUD canvas - see
	// GridRenderer.hpp's own class comment for what that means), not
	// raw client pixels the way this was read before this ported to
	// DirectXTK12/GridRenderer's own primitives.
	//
	// Neither card width nor height is a fixed constant here any more -
	// real Stand's own notification card (Menu/GridItemNotify.cpp on
	// origin/stand-reference) has no fixed size at all: width is the
	// user's own Width setting (NotifySettings::kWidth, replacing this
	// project's previous hardcoded 350 - an arbitrary leftover from the
	// pre-port ImGui window size, not anything real Stand ever used),
	// and height is computed per-notification from its own wrapped
	// message text (GridItemText's own ctor: "height += getTextHeight(
	// text, small_text.scale) + 5 + extra_padding", extra_padding being
	// Stand's own Padding setting) - see Notifications.cpp's own
	// ComputeContentMetrics() for this project's equivalent (title +
	// separator + wrapped message + optional context line, since this
	// project's own notification design - predating this port, unlike
	// Stand's own plain single text block - already had those extra
	// parts; only the "size to content instead of a fixed box" principle
	// is what's actually being matched here).
	static inline float m_CardAnimationSpeed = 50.f;

	enum class NotificationType
	{
		Info,
		Success,
		Warning,
		Error
	};

	struct Notification
	{
		NotificationType m_Type;
		std::string m_Title;
		std::string m_Message;
		std::chrono::time_point<std::chrono::system_clock> m_CreatedOn;
		int m_Duration;
		std::function<void()> m_ContextFunc;
		std::string m_ContextFuncName;
		// Starts fully off-screen at whatever Width currently is (read
		// live at construction time, not a fixed constant) - see
		// DrawImpl()'s own slide-out erase check for the matching other
		// end of this animation.
		float m_AnimationOffset = -Rendering::NotifySettings::kWidth;
		bool m_Erasing = false;
		std::uint32_t m_Identifier;

		// Real Stand's own per-notification flash_time (Menu/GridToaster.cpp) -
		// a std::chrono::steady_clock deadline (not tied to m_CreatedOn,
		// since re-triggering an already-live notification restarts the
		// flash without restarting m_CreatedOn's own countdown) until
		// which this card's border shows NotifySettings::kFlashColour
		// instead of kBorderColour - see Notifications.cpp's own
		// DrawNotificationRect().
		std::chrono::time_point<std::chrono::steady_clock> m_FlashUntil{};

		// This frame's own computed content height and cumulative Y
		// stacking offset from the anchor - set once per frame by
		// DrawImpl() (the rect pass, which owns the running stack
		// accumulator across every live notification/the preview toast)
		// and read back as-is by DrawTextImpl() (the text pass) right
		// after, rather than each pass recomputing its own independent
		// running total - see Notifications.cpp's own DrawImpl() for why:
		// with a fixed per-card height, an index-based position counter
		// was enough for either pass to agree independently; a variable,
		// content-dependent height means a card's own Y depends on the
		// summed heights of every card stacked before it, which only
		// DrawImpl() (running first each frame, per this class's own
		// comment above) can safely compute once and hand off - if
		// DrawTextImpl() also recomputed it independently, a notification
		// erased between the two passes in the same frame (rare, but
		// possible - see DrawImpl()'s own erase handling) would leave the
		// two passes disagreeing on every later card's own Y for that one
		// frame.
		float m_CachedHeight = 0.f;
		float m_StackOffset = 0.f;
	};

	// Toast notification stack, drawn via GridRenderer's own DirectXTK12
	// primitives (DrawRect/DrawText/MeasureText) instead of the classic
	// menu's ImGui windows this used to open one of per card (see
	// Notifications.cpp's own history for that version) - the first of
	// this project's non-menu ImGui-drawing subsystems to move over (see
	// GridRenderer.hpp's own class comment on what else is still ImGui-
	// based). Unlike every Grid/GridItem-based page in this system, this
	// isn't a Grid at all - it's a free-standing overlay always drawn on
	// top of everything else, the same shape MenuPopup already is, except
	// unconditionally every frame rather than only while the Stand-style
	// menu itself is open (a toast needs to show up whether or not any
	// menu is even open) - see GridRenderer::DrawImpl's own call site for
	// where that split lives now.
	//
	// Two-pass draw, split along the same rect/text boundary every other
	// DirectXTK12 consumer in this system already has to (PrimitiveBatch
	// vs SpriteBatch - see GridRenderer.hpp's own DrawRect/DrawText doc
	// comments for why): Draw() MUST run first each frame - besides
	// drawing each card's background/progress bar rects, it's also what
	// advances each card's slide animation and expires/erases finished
	// ones, exactly like the pre-port DrawImpl() already did. DrawText()
	// only reads whatever state Draw() left this frame (title/message/
	// context-function text) - it doesn't re-advance anything, so calling
	// it more than once (or before Draw()) per frame would show stale or
	// duplicate-advanced state. GridRenderer::DrawImpl calls them in that
	// order, once each, every frame - nothing else should call these.
	//
	// The classic menu's own "context function" (a clickable follow-up
	// action shown under the message - e.g. LuaNotify's own callers never
	// actually use this today, so it's untested here beyond mirroring the
	// data model) has no click to bind to any more (this whole overlay is
	// mouseless, same as the rest of the new renderer) - its name still
	// draws as an informational text line rather than being silently
	// dropped.
	class Notifications
	{
	private:
		std::unordered_map<std::string, Notification> m_Notifications = {};
		std::mutex m_mutex;

		// Real Stand's own persistent toast (CommandListNotifySettings::
		// showPersistentToast()/hidePersistentToast(), Commands/Online/
		// CommandListNotifySettings.cpp on origin/stand-reference) -
		// always shown first/topmost, ahead of every real notification in
		// m_Notifications above, while SetPreviewActiveImpl(true) is in
		// effect (see GridRenderer.cpp's own call site: live while
		// Settings > Notifications or its Custom Position sub-page is the
		// one currently open). Kept as its own separate Notification
		// rather than a synthetic entry in m_Notifications - it has no
		// identifier to de-duplicate against, never expires on its own,
		// and needs to always sort first regardless of m_Notifications'
		// own unordered_map iteration order.
		bool m_PreviewActive = false;
		Notification m_Preview{};

		// duration is in milliseconds
		Notification ShowImpl(std::string title, std::string message, NotificationType type, int duration, std::function<void()> context_function, std::string context_function_name);
		void SetPreviewActiveImpl(bool active);
		void FlashPreviewImpl();
		void DrawImpl();
		void DrawTextImpl();
		bool EraseImpl(Notification notification);

		static Notifications& GetInstance()
		{
			static Notifications instance;
			return instance;
		}

	public:
		static void ShowInGame(const std::string& title, const std::string& message, const std::string& icon, const std::string& color);
		static Notification Show(std::string title, std::string message, NotificationType type = NotificationType::Info, int duration = 5000, std::function<void()> context_function = nullptr, std::string context_function_name = "")
		{
			return GetInstance().ShowImpl(title, message, type, duration, context_function, context_function_name);
		}

		// Rect pass (background/progress bar) - also advances animation
		// and expires finished cards. Must run before DrawText() each
		// frame - see this class's own comment above.
		static void Draw()
		{
			GetInstance().DrawImpl();
		}

		// Text pass (title/message/context-function line). Must run
		// after Draw() each frame - see this class's own comment above.
		static void DrawText()
		{
			GetInstance().DrawTextImpl();
		}

		static bool Erase(Notification notification)
		{
			return GetInstance().EraseImpl(notification);
		}

		// See m_Preview's own comment above. Turning it on flashes it
		// once immediately (same as any notification's own first
		// appearance/re-trigger - see ShowImpl()) - "blinking" to show
		// where notifications will appear the moment you open/focus the
		// page that controls where/how they look. A no-op both ways if
		// already in the requested state, so GridRenderer.cpp's own
		// per-frame call (there's no Grid-level "just entered/left this
		// page" hook to call this from instead - see that call site's own
		// comment) doesn't re-flash it every single frame while the page
		// stays open.
		static void SetPreviewActive(bool active)
		{
			GetInstance().SetPreviewActiveImpl(active);
		}

		// Ported from real Stand's own GridToaster::flashPersistentToast()
		// (Menu/GridToaster.cpp on origin/stand-reference), which is what
		// its own "Flash Notification" button actually calls - re-flashes
		// the SAME persistent preview toast (see m_Preview's own comment
		// above) rather than creating a separate, second notification.
		// A no-op while the preview isn't active - matches real Stand's
		// own guard on that button ("if (g_toaster == &g_grid_toaster)"),
		// since there's nothing to flash if the preview isn't currently
		// showing at all.
		static void FlashPreview()
		{
			GetInstance().FlashPreviewImpl();
		}
	};

}
