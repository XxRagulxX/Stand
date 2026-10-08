#include "Rendering/StandPort/GridItemPrimaryText.hpp"

#include "Rendering/StandRendererCompat.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
    GridItemPrimaryText::GridItemPrimaryText(int16_t width, int16_t height, std::wstring text)
        : GridItemText(width, height, {}, Theme::kText), text(std::move(text))
    {
    }

    void GridItemPrimaryText::draw()
    {
        draw(g_renderer.small_text);
    }

    void GridItemPrimaryText::draw(const TextSettings& settings)
    {
        drawBackground(g_renderer.getFocusRectColour());
        g_renderer.drawTextH(float(x + 5), float(y), text, g_renderer.getFocusTextColour(), settings);
    }
}
