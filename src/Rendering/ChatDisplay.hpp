#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include <vector>

namespace Stand
{
	struct Message
	{
		std::string m_Sender;
		std::string m_Message;
		ImColor m_Color;
	};

	// In-game chat overlay: mirrors the pre-port ImGui window (no
	// background, bottom-right-ish, fades out 15s after the last
	// message/MarkAccessed() call) via GridRenderer's own DirectXTK12
	// primitives instead - same shape as Overlay's own watermark (no
	// background of its own, always visible regardless of any menu) and
	// Notifications' own two-pass split.
	class ChatDisplay
	{
	private:
		std::chrono::system_clock::time_point m_LastAccessTime;
		float m_Opacity = 1.0f;
		std::vector<Message> m_Messages = {};
		std::mutex m_Mutex;

		void ShowImpl(std::string sender, std::string message, ImColor color);

		// Rect pass - this overlay has no background of its own, so
		// there's nothing to actually draw, but this still has to run
		// first each frame: it's what advances the idle fade-out (same
		// place that always lived, just no longer interleaved with
		// drawing) - DrawTextImpl reads m_Opacity/m_Messages straight
		// off this same instance afterward, no separate snapshot needed
		// the way Overlay's own free-function state needed one.
		void DrawImpl();

		// Text pass - draws every message at whatever m_Opacity DrawImpl
		// left this frame. Must run after DrawImpl(), same contract as
		// Notifications/Overlay/ESP.
		void DrawTextImpl();

		void ClearImpl();
		void MarkAccessedImpl();

		static ChatDisplay& GetInstance()
		{
			static ChatDisplay instance;
			return instance;
		}

	public:
		static void Show(std::string sender, std::string message, ImColor color)
		{
			return GetInstance().ShowImpl(sender, message, color);
		}

		static void Draw()
		{
			GetInstance().DrawImpl();
		}

		static void DrawText()
		{
			GetInstance().DrawTextImpl();
		}

		static void Clear()
		{
			GetInstance().ClearImpl();
		}

		static void MarkAccessed()
		{
			GetInstance().MarkAccessedImpl();
		}
	};

}
