#pragma once

namespace Stand
{
	struct tbScreenshotMode
	{
		bool allow_centred_text = false;

		[[nodiscard]] bool isEnabled() const noexcept { return false; }
	};

	inline tbScreenshotMode g_tb_screenshot_mode{};
}
