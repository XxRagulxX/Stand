#pragma once

namespace Stand
{
    struct tbScreenshotMode
    {
        [[nodiscard]] bool isEnabled() const noexcept { return false; }
    };

    inline tbScreenshotMode g_tb_screenshot_mode{};
}
