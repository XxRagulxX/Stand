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
		float m_AnimationOffset = -Rendering::NotifySettings::kWidth;
		bool m_Erasing = false;
		std::uint32_t m_Identifier;
		std::chrono::time_point<std::chrono::steady_clock> m_FlashUntil{};
		float m_CachedHeight = 0.f;
		float m_StackOffset = 0.f;
	};

	class Notifications
	{
	private:
		std::unordered_map<std::string, Notification> m_Notifications = {};
		std::mutex m_mutex;
		bool m_PreviewActive = false;
		Notification m_Preview{};

		// duration is in milliseconds
		Notification ShowImpl(std::string title, std::string message, NotificationType type, int duration, std::function<void()> context_function, std::string context_function_name);
		void SetPreviewActiveImpl(bool active);
		void FlashPreviewImpl();
		void DrawImpl();
		void DrawTextImpl();
		bool EraseImpl(Notification notification);
		void ClearImpl();

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

		static void Draw()
		{
			GetInstance().DrawImpl();
		}

		static void DrawText()
		{
			GetInstance().DrawTextImpl();
		}

		static bool Erase(Notification notification)
		{
			return GetInstance().EraseImpl(notification);
		}

		static void Clear()
		{
			GetInstance().ClearImpl();
		}

		static void SetPreviewActive(bool active)
		{
			GetInstance().SetPreviewActiveImpl(active);
		}
		
		static void FlashPreview()
		{
			GetInstance().FlashPreviewImpl();
		}
	};

}
