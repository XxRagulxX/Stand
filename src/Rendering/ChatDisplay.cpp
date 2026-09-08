#include "Rendering/ChatDisplay.hpp"
#include "Menu/Menu.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/TextWrap.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	namespace
	{
		// Original position/width (x_pos = ScreenResX - 470, width 460,
		// y_pos = 200) were real client pixels - now H-space units (see
		// GridRenderer.hpp's own class comment for what that means),
		// same reasoning as every other overlay ported this way this
		// session: since Stand's virtual canvas is also 1920x1080, the
		// same numbers still mean the same thing at 1080p and now
		// actually scale on every other resolution too.
		constexpr float kX = Rendering::Theme::kHudWidth - 470.f;
		constexpr float kMaxWidth = 460.f;
		constexpr float kY = 200.f;
		constexpr float kLineSpacing = 2.f;
		constexpr float kEntryGap = 6.f;
		constexpr float kScale = Rendering::Theme::kSmallTextScale;
	}

	void ChatDisplay::ShowImpl(std::string sender, std::string message, ImColor color)
	{
		if (sender.empty() || message.empty())
			return;

		Message notification{};
		notification.m_Sender = sender;
		notification.m_Message = message;
		notification.m_Color = color;

		static const bool isBigScreen = *Pointers.ScreenResX > 1600 && *Pointers.ScreenResY > 900;
		static const int maxMessages = isBigScreen ? 17 : 7;

		std::lock_guard<std::mutex> lock(m_Mutex);

		if (m_Messages.size() >= maxMessages)
		{
			m_Messages.erase(m_Messages.begin());
		}

		m_Messages.push_back(notification);
		MarkAccessed();
	}

	void ChatDisplay::DrawImpl()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		// TODO: unlink fade speed with FPS
		if (std::chrono::system_clock::now() - m_LastAccessTime > 15s && m_Opacity != 0.0f)
		{
			m_Opacity -= 0.01f;
			if (m_Opacity <= 0.011f)
				m_Opacity = 0.0f;
		}
	}

	void ChatDisplay::DrawTextImpl()
	{
		using Rendering::GridRenderer;

		std::lock_guard<std::mutex> lock(m_Mutex);

		if (m_Messages.empty() || m_Opacity < 0.011f)
			return;

		float y = kY;
		for (auto& message : m_Messages)
		{
			const DirectX::XMFLOAT4 senderColour{message.m_Color.Value.x, message.m_Color.Value.y, message.m_Color.Value.z, m_Opacity};
			const DirectX::XMFLOAT4 messageColour{1.f, 1.f, 1.f, m_Opacity};

			// Sender and message draw as their own separate wrapped
			// blocks rather than continuing on the same line the way
			// the original's own ImGui::SameLine() did - a deliberate
			// simplification (see Rendering/TextWrap.hpp's own comment
			// on why this system's own wrapping is greedy-and-plain, no
			// real text-layout engine), not an attempt to reproduce
			// that exact continuation.
			const std::string senderLine = message.m_Sender + ":";
			GridRenderer::DrawText(kX, y, senderLine.c_str(), senderColour, kScale);
			y += GridRenderer::MeasureText(senderLine.c_str(), kScale).y + kLineSpacing;

			for (auto& line : Rendering::WrapText(message.m_Message, kMaxWidth, kScale))
			{
				GridRenderer::DrawText(kX, y, line.c_str(), messageColour, kScale);
				y += GridRenderer::MeasureText(line.c_str(), kScale).y + kLineSpacing;
			}

			y += kEntryGap;
		}
	}

	void ChatDisplay::ClearImpl()
	{
		m_Messages.clear();
	}

	void ChatDisplay::MarkAccessedImpl()
	{
		m_Opacity = 1.0f;
		m_LastAccessTime = std::chrono::system_clock::now();
	}
}
