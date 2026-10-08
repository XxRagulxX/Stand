#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>
#include <SpriteFont.h>
#include <string>

#include <soup/SharedPtr.hpp>

#include "Game/gta_fwddecl.hpp"
#include "Game/typedecl.hpp"
#include "Rendering/StandPort/Alignment.hpp"
#include "Rendering/StandPort/Direction.hpp"
#include "Rendering/StandPort/TextSettings.hpp"

namespace Stand
{
    struct StandRendererCompat
    {
        Direction tabs_pos = LEFT;
        int16_t tabs_width = 112;

        DirectX::SimpleMath::Color focusRectColour    { 1.0f, 0.0f, 1.0f, 1.0f };
        DirectX::SimpleMath::Color bgRectColour       { 0.0f, 0.0f, 0.0f, 0.3019f };
        DirectX::SimpleMath::Color focusTextColour    { 1.0f, 1.0f, 1.0f, 1.0f };
        DirectX::SimpleMath::Color bgTextColour       { 1.0f, 1.0f, 1.0f, 1.0f };
        DirectX::SimpleMath::Color notifyBorderColour { 1.0f, 0.0f, 1.0f, 1.0f };
        DirectX::SimpleMath::Color notifyFlashColour  { 0.6196f, 0.0f, 0.6196f, 1.0f };
        DirectX::SimpleMath::Color notifyBgColour     { 0.0f, 0.0f, 0.0f, 0.3019f };

        TextSettings small_text{ 12.0f * 2.0f, -2.0f, 2.0f };

        struct Size2d { float x = 1920.0f, y = 1080.0f; } client_size;
        struct Point2d { float x = 0.0f, y = 0.0f; };
        [[nodiscard]] Point2d posC2H(float x, float y) const noexcept { return {x, y}; }

        [[nodiscard]] const DirectX::SimpleMath::Color& getFocusRectColour() const noexcept { return focusRectColour; }
        [[nodiscard]] const DirectX::SimpleMath::Color& getBgRectColour() const noexcept { return bgRectColour; }
        [[nodiscard]] const DirectX::SimpleMath::Color& getFocusTextColour() const noexcept { return focusTextColour; }
        [[nodiscard]] const DirectX::SimpleMath::Color& getBgTextColour() const noexcept { return bgTextColour; }

        enum GameplayState : uint8_t { LOADING, PLAYING, MENUING };
        GameplayState gameplayState = PLAYING;

        [[nodiscard]] bool doesGameplayStateAllowScriptExecution() const noexcept { return true; }
        [[nodiscard]] bool areTabsEnabled() const noexcept { return true; }

        void drawRectH(float, float, float, float, const DirectX::SimpleMath::Color&) const noexcept {}
        void drawTextH(float, float, const std::wstring&, const DirectX::SimpleMath::Color&, const TextSettings&) const noexcept {}
        void drawTextH(float, float, std::wstring&&, const DirectX::SimpleMath::Color&, const TextSettings&) const noexcept {}

        DirectX::SimpleMath::Color arColour = { 1.0f, 0.0f, 1.0f, 0.78431f };
        float resolution_text_scale = 1.0f;
        soup::SharedPtr<DirectX::SpriteFont> m_font_user{};

        void drawLineCP(const rage::Vector2&, const rage::Vector2&, const DirectX::SimpleMath::Color&) noexcept {}
        void drawLineThisTickCP(const rage::Vector2&, const rage::Vector2&, const DirectX::SimpleMath::Color&) noexcept {}
        [[nodiscard]] DirectX::SimpleMath::Vector2 CP2C(float, float) const noexcept { return {}; }
        void drawTextC(float, float, std::wstring&&, Rendering::Alignment, float, const DirectX::SimpleMath::Color&, soup::SharedPtr<DirectX::SpriteFont>, bool) noexcept {}
        void drawUnscaled3dTextThisTickH(const v3&, std::wstring&&, float, DirectX::SimpleMath::Color&) noexcept {}
    };

    inline StandRendererCompat g_renderer{};
}
