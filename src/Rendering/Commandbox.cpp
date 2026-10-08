#include "Rendering/Commandbox.hpp"

namespace Stand
{
    bool Commandbox::shouldBlockGameInputs() const noexcept { return active; }
    bool Commandbox::isColourSelectorActive() const noexcept { return colour_selector_active; }

    void Commandbox::resetSecondaryStates()
    {
        Textbox::resetSecondaryStates();
        colour_selector_active = false;
        colour_selector_cursor = 0;
        colour_selector_cursor_pre_transpose = 0;
    }

    void Commandbox::onUpdate() {}
    void Commandbox::onLeft() { Textbox::onLeft(); }
    void Commandbox::onRight() { Textbox::onRight(); }
    void Commandbox::onUp() { Textbox::onUp(); }
    void Commandbox::onDown() { Textbox::onDown(); }
    void Commandbox::onEnter() {}
    void Commandbox::onEscape() { Textbox::onEscape(); active = false; }
    void Commandbox::onCtrlU() { resetInput(); }

    uint8_t Commandbox::transposeCursorDown(uint8_t cursor) { return cursor; }
    void Commandbox::toggleColourSelectorRow() {}
}
