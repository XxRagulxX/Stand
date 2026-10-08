#include "Rendering/Textbox.hpp"

#include "lib/soup/unicode.hpp"
#include "lib/soup/os.hpp"

namespace Stand
{
    void Textbox::setText(const std::wstring& text)
    {
        resetSecondaryStates();
        setTextKeepSecondaryStates(text);
    }

    void Textbox::setText(std::wstring&& text)
    {
        resetSecondaryStates();
        setTextKeepSecondaryStates(std::move(text));
    }

    void Textbox::setTextKeepSecondaryStates(const std::wstring& text)
    {
        this->text = text;
        onTextAddedNoCursorAdjustment();
        this->cursor = this->text.size();
        onUpdate();
    }

    void Textbox::setTextKeepSecondaryStates(std::wstring&& text)
    {
        this->text = std::move(text);
        onTextAddedNoCursorAdjustment();
        this->cursor = this->text.size();
        onUpdate();
    }

    void Textbox::appendText(const std::wstring& text)
    {
        this->text.insert(cursor, text);
        cursor += text.size();
        onUpdate();
    }

    void Textbox::eraseTextBack(size_t len)
    {
        size_t count = 0;
        while (len != 0 && cursor != 0)
        {
            --len;
            --cursor;
            ++count;
        }
        text.erase(cursor, count);
        onUpdate();
    }

    void Textbox::onTextAdded()
    {
        const auto prev_len = this->text.length();
        onTextAddedNoCursorAdjustment();
        this->cursor += (this->text.length() - prev_len);
    }

    void Textbox::onTextAddedNoCursorAdjustment()
    {
    }

    void Textbox::resetInput()
    {
        cursor = 0;
        text.clear();
        resetSecondaryStates();
        onUpdate();
    }

    void Textbox::resetSecondaryStates()
    {
        history_i = 0;
        text_before_history_navigation.clear();
    }

    void Textbox::onFocus() {}
    void Textbox::onTick() {}
    void Textbox::onBlur() {}

    void Textbox::processKeydown(unsigned int vk)
    {
        switch (vk)
        {
        case VK_LEFT:
            onLeft();
            break;

        case VK_RIGHT:
            onRight();
            break;

        case VK_HOME:
            if (cursor != 0)
            {
                cursor = 0;
                onUpdate();
            }
            break;

        case VK_END:
            if (cursor != text.size())
            {
                cursor = text.size();
                onUpdate();
            }
            break;

        case VK_DELETE:
            if (cursor != text.size())
            {
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    while (cursor < text.size() && isCursorStopper(text[cursor]))
                    {
                        text.erase(cursor, 1);
                    }
                    while (cursor < text.size() && !isCursorStopper(text[cursor]))
                    {
                        text.erase(cursor, 1);
                    }
                }
                else
                {
                    size_t count = 1;
                    SOUP_IF_UNLIKELY (UTF16_IS_HIGH_SURROGATE(text[cursor]))
                    {
                        SOUP_IF_LIKELY (cursor + 1 != text.size())
                        {
                            ++count;
                        }
                    }
                    text.erase(cursor, count);
                }
                onUpdate();
            }
            break;

        case VK_UP:
            onUp();
            break;

        case VK_DOWN:
            onDown();
            break;
        }
    }

    void Textbox::processChar(wchar_t c)
    {
        switch (c)
        {
        default:
            text.insert(cursor++, 1, c);
            onTextAdded();
            onUpdate();
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 10:
        case 11:
        case 12:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 23:
        case 24:
        case 25:
        case 26:
            break;

        case 21:
            onCtrlU();
            break;

        case 8:
            if (cursor != 0)
            {
                --cursor;
                size_t count = 1;
                SOUP_IF_UNLIKELY (UTF16_IS_LOW_SURROGATE(text[cursor]))
                {
                    SOUP_IF_LIKELY (cursor != 0)
                    {
                        --cursor;
                        ++count;
                    }
                }
                text.erase(cursor, count);
                onUpdate();
            }
            break;

        case 127:
            if (cursor != 0)
            {
                do
                {
                    --cursor;
                    if (!isCursorStopper(text[cursor]))
                    {
                        ++cursor;
                        break;
                    }
                    text.erase(cursor, 1);
                } while (cursor != 0);
                while (cursor != 0 && !isCursorStopper(text[cursor - 1]))
                {
                    text.erase(--cursor, 1);
                }
                onUpdate();
            }
            break;

        case 13:
            onEnter();
            break;

        case 22:
            {
                auto clipboard = soup::os::getClipboardTextUtf16();
                for (size_t pos = 0; (pos = clipboard.find(L'\r', pos)) != std::wstring::npos; )
                    clipboard.erase(pos, 1);
                for (size_t pos = 0; (pos = clipboard.find(L'\n', pos)) != std::wstring::npos; )
                    clipboard.replace(pos, 1, L"; ");
                appendText(clipboard);
            }
            break;

        case 27:
            onEscape();
            break;
        }
    }

    void Textbox::onUpdate()
    {
    }

    void Textbox::onLeft()
    {
        if (cursor != 0)
        {
            --cursor;
            if (GetKeyState(VK_CONTROL) & 0x8000)
            {
                do
                {
                    --cursor;
                    if (!isCursorStopper(text[cursor]))
                    {
                        ++cursor;
                        break;
                    }
                } while (cursor != 0);
                while (cursor != 0 && !isCursorStopper(text[cursor - 1]))
                {
                    --cursor;
                }
            }
            else
            {
                SOUP_IF_UNLIKELY (UTF16_IS_LOW_SURROGATE(text[cursor]))
                {
                    SOUP_IF_LIKELY (cursor != 0)
                    {
                        --cursor;
                    }
                }
            }
            onUpdate();
        }
    }

    void Textbox::onRight()
    {
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            while (cursor < text.size() && isCursorStopper(text[++cursor]));
            while (cursor < text.size() && !isCursorStopper(text[++cursor]));
        }
        else
        {
            if (cursor != text.size())
            {
                ++cursor;
                SOUP_IF_UNLIKELY (UTF16_IS_HIGH_SURROGATE(text[cursor - 1]))
                {
                    SOUP_IF_LIKELY (cursor != text.size())
                    {
                        ++cursor;
                    }
                }
            }
        }
        onUpdate();
    }

    void Textbox::onUp()
    {
        auto new_history_i = (history_i + 1);
        auto i = history.size() - new_history_i;
        if (i < history.size())
        {
            if (history_i == 0)
            {
                text_before_history_navigation = std::move(text);
            }
            history_i = new_history_i;
            setTextKeepSecondaryStates(history.at(i));
        }
    }

    void Textbox::onDown()
    {
        if (!history.empty() && history_i != 0)
        {
            if (--history_i == 0)
            {
                setTextKeepSecondaryStates(std::move(text_before_history_navigation));
            }
            else
            {
                setTextKeepSecondaryStates(history.at(history.size() - history_i));
            }
        }
    }

    void Textbox::onEnter()
    {
    }

    void Textbox::onEscape()
    {
        if (!keep_content_on_close)
        {
            resetInput();
        }
    }

    void Textbox::onCtrlU()
    {
    }

    bool Textbox::isCursorStopper(wchar_t c) noexcept
    {
        return c == L' '
            || c == L'/'
            || c == L'\\'
            ;
    }
}
