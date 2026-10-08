#include "Rendering/StandPort/GridItemNotify.hpp"

#include "Rendering/NotifyGrid.hpp"
#include "Rendering/StandRendererCompat.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/StringUtils.hpp"

namespace Stand::Rendering
{
    GridItemNotify::GridItemNotify(const std::wstring& text, time_t show_until, time_t flash_until)
        : GridItemText(0, 0, StringUtils::utf16_to_utf8(text), {}),
          show_until(show_until), flash_until(flash_until),
          frozen(false), flashing(!IS_DEADLINE_REACHED(flash_until))
    {
    }

    void GridItemNotify::draw()
    {
        if (show_until != 0 && !frozen && IS_DEADLINE_REACHED(show_until))
        {
            g_notify_grid.update();
        }
        constexpr int16_t border_width = 3;
        g_renderer.drawRectH(float(x - border_width), float(y), float(border_width), float(height), g_renderer.notifyBorderColour);
        if (!frozen)
        {
            flashing = !IS_DEADLINE_REACHED(flash_until);
        }
        if (!flashing)
        {
            drawBackground(g_renderer.notifyBgColour);
        }
        else
        {
            drawBackground(g_renderer.notifyFlashColour);
        }
        drawText();
    }
}
