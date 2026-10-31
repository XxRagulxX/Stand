#include "Rendering/StandPort/GridItemNotify.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/StandPort/NotifyGrid.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"
#include "Rendering/Theme.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/StringUtils.hpp"

#include <algorithm>

namespace Stand::Rendering
{
    static constexpr int16_t kBorderWidth = 3;

    std::vector<GridItemNotify*> GridItemNotify::s_Instances;

    GridItemNotify::GridItemNotify(std::string&& text, time_t show_until, time_t flash_until, std::function<void()> on_expire)
        : GridItem(
              GRIDITEM_INDIFFERENT,
              Stand::g_notify_grid.notify_width,
              0,
              0,
              (Stand::g_notify_grid.invert_flow && !Stand::g_notify_grid.next_to_map)
                  ? ALIGN_BOTTOM_LEFT
                  : ALIGN_TOP_CENTRE),
          show_until(show_until),
          flash_until(flash_until),
          frozen(Stand::g_notify_grid.isFrozen()),
          flashing(!IS_DEADLINE_REACHED(flash_until)),
          text(GridRenderer::WrapText(std::move(text), Theme::kSmallTextScale, float(Stand::g_notify_grid.notify_width - 10))),
          m_OnExpire(std::move(on_expire))
    {
        height = int16_t(GridRenderer::MeasureText(this->text.c_str(), Theme::kSmallTextScale).y + 2.f * Stand::g_notify_grid.notify_padding);
        s_Instances.push_back(this);
    }

    GridItemNotify::GridItemNotify(const std::wstring& text, time_t show_until, time_t flash_until)
        : GridItemNotify(StringUtils::utf16_to_utf8(text), show_until, flash_until, nullptr)
    {
    }

    GridItemNotify::~GridItemNotify()
    {
        s_Instances.erase(std::remove(s_Instances.begin(), s_Instances.end(), this), s_Instances.end());
    }

    void GridItemNotify::draw()
    {
        if (show_until != 0 && !frozen && IS_DEADLINE_REACHED(show_until))
        {
            if (m_OnExpire)
                m_OnExpire();
        }

        GridRenderer::DrawRect(float(x - kBorderWidth), float(y), float(kBorderWidth), float(height), g_renderer.notifyBorderColour);

        if (!frozen)
            flashing = !IS_DEADLINE_REACHED(flash_until);

        GridRenderer::DrawRect(float(x), float(y), float(width), float(height),
            flashing ? g_renderer.notifyFlashColour : g_renderer.notifyBgColour);
    }

    void GridItemNotify::drawText()
    {
        GridRenderer::DrawText(
            float(x) + 5.f + Stand::g_notify_grid.notify_padding,
            float(y) + Stand::g_notify_grid.notify_padding,
            text.c_str(),
            Theme::kUnfocusedText,
            Theme::kSmallTextScale);
    }
}
