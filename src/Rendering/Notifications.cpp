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
		constexpr float kTextPadding = 8.f;
		constexpr float kBorderWidth = 3.f;
		constexpr float kTitleScale = Rendering::Theme::kTextScale;
		constexpr float kMessageScale = Rendering::Theme::kSmallTextScale;
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

		float GetHSpaceScale()
		{
			const auto resX = static_cast<float>(*Pointers.ScreenResX);
			const auto resY = static_cast<float>(*Pointers.ScreenResY);
			if (resX <= 0.f || resY <= 0.f)
				return 1.f;

			return std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight);
		}

		void GetNextToMapAnchor(float& anchorX, float& anchorY)
		{
			const float safeZone = GRAPHICS::GET_SAFE_ZONE_SIZE();
			const float safezoneScale = 1.f - ((safeZone * 10.f) - 9.f);
			const float safezoneWidthH = 96.f * safezoneScale;
			const float safezoneHeightH = 54.f * safezoneScale;

			const auto scale = GetHSpaceScale();
			const float bottomEdgeH = Rendering::Theme::kHudHeight - (3.8f / scale);

			anchorX = 0.2f + safezoneWidthH;
			anchorY = bottomEdgeH - safezoneHeightH;

			const bool minimapVisible = !HUD::IS_WARNING_MESSAGE_ACTIVE() && !HUD::IS_PAUSE_MENU_ACTIVE() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();
			if (minimapVisible)
				anchorX += 291.f;
		}

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

			const bool hasTitle = !notification.m_Title.empty();

			ContentMetrics metrics{};
			metrics.titleHeight = hasTitle ? Rendering::GridRenderer::MeasureText(notification.m_Title.c_str(), kTitleScale).y : 0.f;
			metrics.messageLines = Rendering::WrapText(notification.m_Message, kWidth - kTextPadding * 2.f, kMessageScale);
			metrics.lineHeight = Rendering::GridRenderer::MeasureText("Ag", kMessageScale).y;

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

		Layout ComputeLayout(const Notification& notification, float stackOffset, const ContentMetrics& metrics)
		{
			using Rendering::NotifySettings::kInvertFlow;
			using Rendering::NotifySettings::kType;
			using Rendering::NotifySettings::Type;

			float anchorX, anchorY;
			GetAnchor(anchorX, anchorY);

			const bool growUpward = (kType == Type::StandNextToMap) || kInvertFlow;

			Layout layout{};
			layout.cardX = anchorX + notification.m_AnimationOffset;
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

			if (notification.m_ContextFunc)
				GridRenderer::DrawText(textX, layout.contextY, notification.m_ContextFuncName.c_str(), Rendering::Theme::kPlaceholderText, kMessageScale);
		}

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
		if (message.empty())
			return {};

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
			exists->second.m_FlashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(Rendering::NotifySettings::kFlashMs);
			return {};
		}

		Notification notification{};
		notification.m_Title = title;
		notification.m_Message = message;
		notification.m_Type = type;
		notification.m_CreatedOn = std::chrono::system_clock::now();
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

	void Notifications::ClearImpl()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_Notifications.clear();
	}

	void Notifications::DrawImpl()
	{
		std::vector<std::string> keys_to_erase;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			float stackOffset = 0.f;

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
