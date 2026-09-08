#include "Rendering/Notifications.hpp"

#include "Core/Pointers.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Rendering/TextWrap.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"

#include <algorithm>
#include <limits>
#include <mutex>

namespace Stand
{
	namespace
	{
		// Internal card padding (text inset from the card's own edges) -
		// a project-specific constant, distinct from
		// Rendering::NotifySettings::kPadding (real Stand's own
		// user-facing "Padding" setting, which controls the gap between
		// the anchor point/stacked cards instead - see DrawImpl()'s own
		// stackOffset accumulator below). Real Stand doesn't expose this
		// inner inset separately.
		constexpr float kTextPadding = 8.f;
		// Real Stand's own GridItemNotify::draw() border_width
		// (Menu/GridItemNotify.cpp on origin/stand-reference) - a
		// static stripe down the card's own left edge, not an
		// animated "remaining time" bar - see DrawNotificationRect()
		// below for where this project's previous top-shrinking-
		// width bar (never actually real Stand's own behaviour, a
		// pre-existing addition from before this port) is replaced
		// by this instead.
		constexpr float kBorderWidth = 3.f;
		constexpr float kTitleScale = Rendering::Theme::kTextScale;
		constexpr float kMessageScale = Rendering::Theme::kSmallTextScale;

		// Not part of Theme.hpp's own six customizable colours (see that
		// header's own comment on why only those six are) - Success/
		// Warning have no existing slot there, same as the original's own
		// hardcoded ImVec4s for them.
		constexpr DirectX::XMFLOAT4 kSuccessColour{0.f, 1.f, 0.f, 1.f};
		constexpr DirectX::XMFLOAT4 kWarningColour{1.f, 0.5f, 0.f, 1.f};

		const DirectX::XMFLOAT4& GetTypeColour(NotificationType type)
		{
			switch (type)
			{
			case NotificationType::Success:
				return kSuccessColour;
			case NotificationType::Warning:
				return kWarningColour;
			case NotificationType::Error:
				return Rendering::Theme::kError;
			case NotificationType::Info:
			default:
				return Rendering::Theme::kText;
			}
		}

		// Same per-axis H-space/client-pixel scale Commands/Widgets/
		// Position2dCommands.hpp's own Detail::GetHSpaceScale() already
		// is - duplicated here rather than shared, same call this
		// project already made for that function's own precedent
		// (CommandMenuPosition.cpp's original version): this is the only
		// other place outside that file needing one, and it's a tiny,
		// self-contained, pure function - not worth a Rendering ->
		// Commands include just to share three lines.
		float GetHSpaceScale()
		{
			const auto resX = static_cast<float>(*Pointers.ScreenResX);
			const auto resY = static_cast<float>(*Pointers.ScreenResY);
			if (resX <= 0.f || resY <= 0.f)
				return 1.f;

			return std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight);
		}

		// Ported from real Stand's own NotifyGrid::setOriginNextToMap()
		// (Menu/NotifyGrid.cpp on origin/stand-reference) - positions the
		// anchor just above the safe zone at the bottom-left of the
		// screen, shifted right when the minimap is actually on screen
		// so this sits BESIDE it instead of overlapping it. Ported:
		// the safe-zone-based base position (GRAPHICS::GET_SAFE_ZONE_SIZE(),
		// same formula) and the minimap-visible shift (the same three
		// HUD-state natives Stand's own minimap_visible check uses, all
		// already used elsewhere in this project). NOT ported: Stand's
		// own further +158/+39/+60 shifts for an expanded "big map" or a
		// pending Social Club invite indicator - those read
		// Util::is_bigmap_active() (a raw ScriptGlobal(GLOBAL_BIGMAP)
		// read) and a hardcoded GLOBAL_NEW_INVITES script global index,
		// both Stand-specific and - unlike the natives used below - a
		// real portability risk to hardcode here (global indices shift
		// between game builds); a disclosed gap for the common case
		// (normal minimap, no pending invite) rather than every case.
		void GetNextToMapAnchor(float& anchorX, float& anchorY)
		{
			const float safeZone = GRAPHICS::GET_SAFE_ZONE_SIZE();
			const float safezoneScale = 1.f - ((safeZone * 10.f) - 9.f);
			const float safezoneWidthH = 96.f * safezoneScale;
			const float safezoneHeightH = 54.f * safezoneScale;

			// Real Stand's own posC2H(0, client_size_y - 3.8f).y -
			// "3.8 client pixels above the bottom edge of the screen"
			// converted to H-space. GridRenderer's own equivalent
			// (PosC2H) was removed once real mouse support was dropped
			// (see GridRenderer::WndProcImpl's own comment) - re-adding
			// it just for this one read isn't worth it, so this
			// converts the same way GetHSpaceScale() above already lets
			// Position2dCommands.hpp's Move With Mouse do: kHudHeight is
			// always the H-space canvas's own bottom edge by definition
			// (Stand's own convention - see GridRenderer.hpp's own class
			// comment), so "a few client pixels above that" is just
			// kHudHeight minus those pixels converted through the same
			// scale.
			const auto scale = GetHSpaceScale();
			const float bottomEdgeH = Rendering::Theme::kHudHeight - (3.8f / scale);

			anchorX = 0.2f + safezoneWidthH;
			anchorY = bottomEdgeH - safezoneHeightH;

			const bool minimapVisible = !HUD::IS_WARNING_MESSAGE_ACTIVE() && !HUD::IS_PAUSE_MENU_ACTIVE() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();
			if (minimapVisible)
				anchorX += 291.f;
		}

		// Where notification 0 (the newest/first-position one - see
		// Draw()'s own position counter) anchors to, depending on
		// Rendering::NotifySettings::kType - real Stand's own
		// CommandNotifyType switches between exactly these two Stand-
		// drawn positions (Game routes through ShowInGame() instead and
		// never reaches this at all - see ShowImpl()'s own early-out).
		void GetAnchor(float& anchorX, float& anchorY)
		{
			if (Rendering::NotifySettings::kType == Rendering::NotifySettings::Type::StandCustomPosition)
			{
				anchorX = static_cast<float>(Rendering::NotifySettings::kCustomPositionX);
				anchorY = static_cast<float>(Rendering::NotifySettings::kCustomPositionY);
			}
			else
			{
				GetNextToMapAnchor(anchorX, anchorY);
			}
		}

		// Everything about a notification's own on-screen size, computed
		// purely from its own (immutable, once created) title/message
		// text plus whatever Width/Padding currently are - independent of
		// its position in the stack, so this can be computed for one
		// notification without needing to know about any other. Real
		// Stand's own equivalent is GridItemText's ctor (Menu/
		// GridItemText.cpp on origin/stand-reference): "height +=
		// getTextHeight(text, small_text.scale) + 5 + extra_padding" -
		// there's no separate title/separator/context line there (a
		// notification is just one wrapped block of text with a border
		// stripe down its left edge), so this reproduces the same "size
		// to content, not a fixed box" principle against this project's
		// own richer title+message+context layout instead, rather than
		// literally collapsing that back down to Stand's plainer shape.
		struct ContentMetrics
		{
			std::vector<std::string> messageLines;
			float lineHeight;
			float titleHeight;
			float cardHeight;
		};

		ContentMetrics ComputeContentMetrics(const Notification& notification)
		{
			using Rendering::NotifySettings::kWidth;

			// Real Stand's own persistent preview toast has no title at
			// all (LANG_GET_W("IPSUM") is the whole message, same as
			// every other GridToaster notification - see
			// Notifications.hpp's own class comment on why this
			// project's own title/message split exists at all) - an
			// empty title here skips the title row entirely rather than
			// drawing an empty line, so Notifications::SetPreviewActive()
			// can build one with just a message.
			const bool hasTitle = !notification.m_Title.empty();

			ContentMetrics metrics{};
			metrics.titleHeight = hasTitle ? Rendering::GridRenderer::MeasureText(notification.m_Title.c_str(), kTitleScale).y : 0.f;
			metrics.messageLines = Rendering::WrapText(notification.m_Message, kWidth - kTextPadding * 2.f, kMessageScale);
			metrics.lineHeight = Rendering::GridRenderer::MeasureText("Ag", kMessageScale).y;

			// Same running-Y shape ComputeLayout() below builds from -
			// top inset, optional title, message lines, optional context
			// line, bottom inset - kept in sync with that function's own
			// offsets rather than measuring the drawn rect after the
			// fact. No separator line and no top-row reservation for a
			// progress bar any more - see DrawNotificationRect() below
			// for why (real Stand has neither), which is also most of
			// what makes this smaller than before.
			float height = kTextPadding;
			if (hasTitle)
				height += metrics.titleHeight + kTextPadding * 0.5f;
			height += metrics.messageLines.size() * metrics.lineHeight;
			if (notification.m_ContextFunc)
				height += kTextPadding * 0.5f + Rendering::GridRenderer::MeasureText(notification.m_ContextFuncName.c_str(), kMessageScale).y;
			height += kTextPadding;

			metrics.cardHeight = height;
			return metrics;
		}

		struct Layout
		{
			float cardX, cardY;
			float titleY;
			float messageStartY;
			float contextY;
		};

		// stackOffset is this notification's own cumulative Y offset
		// from the anchor (see Notification::m_StackOffset's own
		// comment) - a running pixel total rather than an index*fixed-
		// height multiple, since every card can now be a different
		// height.
		Layout ComputeLayout(const Notification& notification, float stackOffset, const ContentMetrics& metrics)
		{
			using Rendering::NotifySettings::kInvertFlow;
			using Rendering::NotifySettings::kType;
			using Rendering::NotifySettings::Type;

			float anchorX, anchorY;
			GetAnchor(anchorX, anchorY);

			// Real Stand's own GridItemNotify/Grid::setPositions()
			// (Menu/GridItemNotify.cpp + Menu/Grid.cpp on origin/
			// stand-reference) - "Stand, Next To Map" ALWAYS grows
			// upward from anchorY regardless of Invert Flow (that
			// setting "only takes effect with type set to Stand, Custom
			// Position" - see CommandNotifyInvertFlow.cpp's own
			// description), because real Stand's own GridItemNotify ctor
			// picks ALIGN_TOP_CENTRE unconditionally whenever next_to_map
			// is true, and only respects invert_flow's own
			// ALIGN_BOTTOM_LEFT while it's false. "Stand, Custom
			// Position" instead grows downward by default (Invert Flow
			// off) and only grows upward when the user turns it on.
			const bool growUpward = (kType == Type::StandNextToMap) || kInvertFlow;

			Layout layout{};
			layout.cardX = anchorX + notification.m_AnimationOffset;
			// Traced against Grid::setPositions()'s own ALIGN_TOP_CENTRE
			// arithmetic: for the first item (stackOffset == 0), "y -=
			// item->height" makes anchorY the item's own BOTTOM edge, not
			// its top - i.e. growing upward means this card's top edge is
			// anchorY minus its OWN height too, not just minus the
			// stack's running total. Growing downward has no such
			// asymmetry (anchorY is already the first item's own top
			// edge, same as the accumulator itself). Getting this wrong
			// for the "Next To Map" default previously placed the first
			// card almost entirely off the bottom of the screen (anchorY
			// there sits only ~4px above the screen's own bottom edge, on
			// purpose - it's meant to be a floor cards sit ON TOP OF, not
			// a ceiling they hang below).
			layout.cardY = growUpward ? anchorY - stackOffset - metrics.cardHeight : anchorY + stackOffset;

			layout.titleY = layout.cardY + kTextPadding;
			layout.messageStartY = notification.m_Title.empty() ? layout.titleY : layout.titleY + metrics.titleHeight + kTextPadding * 0.5f;
			layout.contextY = layout.messageStartY + metrics.messageLines.size() * metrics.lineHeight + kTextPadding * 0.5f;

			return layout;
		}

		void DrawNotificationRect(const Notification& notification, const ContentMetrics& metrics)
		{
			using Rendering::GridRenderer;
			using Rendering::NotifySettings::kWidth;

			const auto layout = ComputeLayout(notification, notification.m_StackOffset, metrics);

			// Real Stand's own Border Colour, flashing to Flash Colour
			// for a short window after this notification first appears
			// (or re-triggers - see ShowImpl()) - drawn as a static
			// stripe down the card's own LEFT edge (GridItemNotify::
			// draw()'s own drawRectH(x - border_width, y, border_width,
			// height, ...)), not the shrinking-width "time remaining"
			// bar this project drew across the top before - that was
			// never real Stand's own behaviour (a pre-existing addition
			// predating this port), so the visual "remaining time" cue
			// it gave is a real, disclosed loss here in favour of
			// matching Stand's real look (and further shrinking every
			// card - it no longer reserves its own top row at all).
			const auto& borderColour = (std::chrono::steady_clock::now() < notification.m_FlashUntil) ? Rendering::NotifySettings::kFlashColour : Rendering::NotifySettings::kBorderColour;

			GridRenderer::DrawRect(layout.cardX, layout.cardY, kWidth, metrics.cardHeight, Rendering::NotifySettings::kBackgroundColour);
			GridRenderer::DrawRect(layout.cardX - kBorderWidth, layout.cardY, kBorderWidth, metrics.cardHeight, borderColour);
		}

		void DrawNotificationText(const Notification& notification, const ContentMetrics& metrics)
		{
			using Rendering::GridRenderer;

			const auto layout = ComputeLayout(notification, notification.m_StackOffset, metrics);
			const float textX = layout.cardX + kTextPadding;

			if (!notification.m_Title.empty())
				GridRenderer::DrawText(textX, layout.titleY, notification.m_Title.c_str(), GetTypeColour(notification.m_Type), kTitleScale);

			float y = layout.messageStartY;
			for (auto& line : metrics.messageLines)
			{
				GridRenderer::DrawText(textX, y, line.c_str(), Rendering::Theme::kText, kMessageScale);
				y += metrics.lineHeight;
			}

			// No click to bind this to any more (this overlay is
			// mouseless, same as the rest of the new renderer) - see
			// this class's own header comment - so this is informational
			// only.
			if (notification.m_ContextFunc)
				GridRenderer::DrawText(textX, layout.contextY, notification.m_ContextFuncName.c_str(), Rendering::Theme::kPlaceholderText, kMessageScale);
		}

		// Real Stand's own estimate_reading_time() (Menu/GridToaster.cpp) -
		// counts words by space character (real Stand's own version also
		// counts 0.5 words per non-ASCII wide character, for CJK text;
		// this project's own Notification::m_Message is a narrow/UTF-8
		// std::string rather than Stand's own std::wstring, so that half
		// of the formula is left out here - a real, disclosed
		// simplification rather than an attempt at UTF-8-aware CJK
		// detection). Result is NOT clamped yet - ShowImpl() clamps to
		// [kMinDurationMs, kMaxDurationMs] itself, matching real Stand's
		// own std::clamp call site.
		int EstimateReadingTimeMs(const std::string& message)
		{
			double numWords = 0.0;
			for (char c : message)
			{
				if (c == ' ')
					numWords += 1.0;
			}

			const auto wpm = std::max<std::uint16_t>(1, Rendering::NotifySettings::kReadingSpeedWpm);
			const double secondsPerWord = 60.0 / static_cast<double>(wpm);
			return static_cast<int>(numWords * secondsPerWord * 1000.0) + Rendering::NotifySettings::kReadingStartDelayMs;
		}
	}

	Notification Notifications::ShowImpl(std::string title, std::string message, NotificationType type, int duration, std::function<void()> context_function, std::string context_function_name)
	{
		// title alone being empty is fine and already fully supported
		// downstream - ComputeContentMetrics()/DrawTextImpl() both skip
		// the title row entirely when it's empty (see their own
		// comments), the same title-less look every one of real Stand's
		// own toasts actually has (m_Preview above builds one exactly
		// this way, bypassing this function). Requiring BOTH here was an
		// unrelated bug that surfaced when Click::respond() (Menu/
		// Click.cpp) was fixed to stop passing an empty title as its own
		// workaround for a message that never showed up at all - the
		// real fix was this line, not giving every Click-driven toast a
		// generic "StandEnhanced" title it never needed. message is still
		// required - there's nothing to show at all without it.
		if (message.empty())
			return {};

		// Real Stand's own CommandNotifyType, "Game" option - routes
		// straight through the native GTA5 feed instead of this
		// project's own DirectXTK12 overlay, same as real Stand
		// switching its own g_toaster between GameToaster/GridToaster.
		// Nothing gets added to m_Notifications below in this case - the
		// overlay simply never shows anything while this is selected.
		if (Rendering::NotifySettings::kType == Rendering::NotifySettings::Type::Game)
		{
			ShowInGame(title, message, "", "");
			return {};
		}

		auto message_id = Joaat(title + message);

		auto exists = std::find_if(m_Notifications.begin(), m_Notifications.end(), [&](auto& notification) {
			return notification.second.m_Identifier == message_id;
		});

		if (exists != m_Notifications.end())
		{
			exists->second.m_CreatedOn = std::chrono::system_clock::now();
			// Real Stand's own re-trigger flash (Menu/GridToaster.cpp:
			// "if (notify.live) flash_time += get_current_time_millis();") -
			// an already-live notification showing again restarts its
			// own flash window without restarting m_CreatedOn's own
			// countdown above.
			exists->second.m_FlashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(Rendering::NotifySettings::kFlashMs);
			return {};
		}

		Notification notification{};
		notification.m_Title = title;
		notification.m_Message = message;
		notification.m_Type = type;
		notification.m_CreatedOn = std::chrono::system_clock::now();
		// Real Stand's own toast() always computes this from the
		// message itself (EstimateReadingTimeMs() above) rather than
		// trusting a duration passed in per call - the duration
		// parameter is kept only for source compatibility with every
		// existing call site (deliberately unused otherwise).
		notification.m_Duration = std::clamp(EstimateReadingTimeMs(message), Rendering::NotifySettings::kMinDurationMs, Rendering::NotifySettings::kMaxDurationMs);
		notification.m_Identifier = message_id;
		notification.m_FlashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(Rendering::NotifySettings::kFlashMs);

		if (context_function)
		{
			notification.m_ContextFunc = context_function;
			notification.m_ContextFuncName = context_function_name.empty() ? "Context Function" : context_function_name;
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		auto result = m_Notifications.insert(std::make_pair(title + message, notification));

		return notification;
	}

	bool Notifications::EraseImpl(Notification notification)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (auto& [id, n] : m_Notifications)
		{
			if (n.m_Identifier == notification.m_Identifier)
			{
				n.m_Erasing = true;
				return true;
			}
		}

		return false;
	}

	void Notifications::DrawImpl()
	{
		std::vector<std::string> keys_to_erase;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			float stackOffset = 0.f;

			// Real Stand's own addPersistentNotifyItem() (Menu/
			// NotifyGrid.cpp) - always drawn first/topmost, ahead of
			// every real notification below. Never drawn while Type is
			// "Game" - see SetPreviewActiveImpl()'s own comment for why
			// that routes through the native feed instead, one-shot,
			// rather than this overlay.
			if (m_PreviewActive && Rendering::NotifySettings::kType != Rendering::NotifySettings::Type::Game)
			{
				const auto metrics = ComputeContentMetrics(m_Preview);
				m_Preview.m_StackOffset = stackOffset;
				m_Preview.m_CachedHeight = metrics.cardHeight;
				DrawNotificationRect(m_Preview, metrics);
				stackOffset += metrics.cardHeight + Rendering::NotifySettings::kPadding;
			}

			for (auto& [id, notification] : m_Notifications)
			{
				const auto metrics = ComputeContentMetrics(notification);
				// Cached for DrawTextImpl()'s own pass right after - see
				// Notification::m_StackOffset/m_CachedHeight's own
				// comment for why that pass reads these back instead of
				// recomputing its own running total independently.
				notification.m_StackOffset = stackOffset;
				notification.m_CachedHeight = metrics.cardHeight;

				DrawNotificationRect(notification, metrics);

				if (!notification.m_Erasing)
				{
					if (notification.m_AnimationOffset < 0)
						notification.m_AnimationOffset += m_CardAnimationSpeed;

					// Need this to account for changes in card size (x dimension), custom increments might result in odd numbers
					if (notification.m_AnimationOffset > 0)
						notification.m_AnimationOffset = 0.f;
				}
				else
				{
					notification.m_AnimationOffset -= m_CardAnimationSpeed;
					if (notification.m_AnimationOffset <= -Rendering::NotifySettings::kWidth)
						keys_to_erase.push_back(id);
				}


				if ((float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - notification.m_CreatedOn).count() >= notification.m_Duration)
					keys_to_erase.push_back(id);

				stackOffset += metrics.cardHeight + Rendering::NotifySettings::kPadding;
			}
		}
		std::lock_guard<std::mutex> lock(m_mutex);
		for (const auto& key : keys_to_erase)
		{
			m_Notifications.erase(key);
		}
	}

	void Notifications::DrawTextImpl()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_PreviewActive && Rendering::NotifySettings::kType != Rendering::NotifySettings::Type::Game)
			DrawNotificationText(m_Preview, ComputeContentMetrics(m_Preview));

		for (auto& [id, notification] : m_Notifications)
		{
			DrawNotificationText(notification, ComputeContentMetrics(notification));
		}
	}

	void Notifications::SetPreviewActiveImpl(bool active)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (active == m_PreviewActive)
			return;

		m_PreviewActive = active;
		if (!active)
			return;

		// Real Stand's own g_toaster->setPersistentToast(LANG_GET_W("IPSUM")) -
		// "IPSUM" being Stand's own placeholder string, "Here's some text
		// to feast your eyes upon" (its real English translation, not a
		// guess - this project has no localization table of its own, so
		// the plain string stands in directly). No title - same as every
		// other real Stand notification (see ComputeContentMetrics()'s
		// own comment on why an empty title skips that row entirely).
		// Never expires on its own (INT_MAX) - cleared only by
		// SetPreviewActive(false) - and starts fully on-screen (no
		// slide-in) since real Stand's own notifications don't slide at
		// all (see Notifications.hpp's own class comment on where that
		// animation actually comes from).
		//
		// Built here regardless of Type - even though DrawImpl()/
		// DrawTextImpl() only ever draw it while Type isn't "Game" (see
		// their own comments) - so switching Type away from "Game"
		// without leaving this page still has real content ready to
		// show immediately, rather than an empty/stale m_Preview left
		// over from whatever Type was active the last time this
		// activated.
		m_Preview = Notification{};
		m_Preview.m_Type = NotificationType::Info;
		m_Preview.m_Message = "Here's some text to feast your eyes upon";
		m_Preview.m_CreatedOn = std::chrono::system_clock::now();
		m_Preview.m_Duration = std::numeric_limits<int>::max();
		m_Preview.m_AnimationOffset = 0.f;
		// Flashes once immediately - the "blink to show where a
		// notification will appear" the moment this page is opened/
		// focused, same as any notification's own first appearance.
		m_Preview.m_FlashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(Rendering::NotifySettings::kFlashMs);

		// Real Stand's own persistent toast also reroutes through the
		// native feed (GameToaster) while Type is "Game"
		// (CommandListNotifySettings::setToaster()/updateGameColour()) -
		// Border/Flash/Background Colour have no equivalent on that feed
		// at all (see ShowInGame()'s own native calls), so there's no
		// live-colour-preview benefit to replicating Stand's own
		// repeated re-posting just to keep a feed message persistently
		// on screen. A single sample post the moment this page is
		// opened/focused is a fair, honest analogue instead.
		if (Rendering::NotifySettings::kType == Rendering::NotifySettings::Type::Game)
			ShowInGame(m_Preview.m_Title, m_Preview.m_Message, "", "");
	}

	void Notifications::FlashPreviewImpl()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_PreviewActive)
			return;

		m_Preview.m_FlashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(Rendering::NotifySettings::kFlashMs);
	}

	int GetNotificationColor(const std::string& color)
	{
		std::string c = color;
		std::transform(c.begin(), c.end(), c.begin(), ::tolower);

		if (c == "white")
			return 160;
		if (c == "black")
			return 140;
		if (c == "red")
			return 6;
		if (c == "orange")
			return 130;
		if (c == "yellow")
			return 180;
		if (c == "mint")
			return 200;
		if (c == "green")
			return 184;
		if (c == "light blue")
			return 40;

		return 140; // default black
	}

	void Notifications::ShowInGame(const std::string& title, const std::string& message, const std::string& icon, const std::string& color)
	{
		// title may be empty (SetPreviewActiveImpl()'s own preview has
		// none, matching real Stand's own notifications) - only the
		// message is actually required for the native feed post itself
		// to mean anything.
		if (message.empty())
			return;

		FiberPool::queueJob([=] {
			constexpr int TIMEOUT_FRAMES = 300;
			constexpr int FEED_ICON_TYPE = 1;

			int bgColor = GetNotificationColor(color);

			std::string finalIcon = icon.empty() ? "CHAR_DEFAULT" : icon;

			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(finalIcon.c_str(), true);

			int timeout = 0;
			while (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(finalIcon.c_str()))
			{
				BUILTIN::WAIT(0);

				if (++timeout > TIMEOUT_FRAMES)
				{
					// fallback icon if failed
					finalIcon = "CHAR_DEFAULT";
					GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(finalIcon.c_str(), true);
					break;
				}
			}

			HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message.c_str());

			HUD::THEFEED_SET_BACKGROUND_COLOR_FOR_NEXT_POST(bgColor);

			HUD::END_TEXT_COMMAND_THEFEED_POST_MESSAGETEXT(
			    finalIcon.c_str(),
			    finalIcon.c_str(),
			    true,
			    FEED_ICON_TYPE,
			    title.c_str(),
			    "~c~Notification");

			HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(true, false);

			GRAPHICS::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(finalIcon.c_str());
		});
	}
}
