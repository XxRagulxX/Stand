#include "Commands/Widgets/CommandListSelect.hpp"

#include "Commands/Widgets/CommandStateSerializer.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    std::optional<Label> CommandListSelect::getLabelForValue(long long v) const
    {
        for (const auto& [val, lbl] : options)
        {
            if (val == v)
                return lbl;
        }
        return std::nullopt;
    }

    void CommandListSelect::updateValue(Click& click, long long new_val)
    {
        ensureScriptThread(click, [this, new_val](Click& click) mutable {
            const long long prev = this->value;
            this->value = new_val;
            updateState(click);
            onChange(click, prev);
        });
    }

    void CommandListSelect::updateState(const Click&)
    {
        CommandStateSerializer::MarkDirty();
    }

    bool CommandListSelect::onLeft(Click& click, bool holding)
    {
        std::optional<long long> prev;
        for (const auto& [val, lbl] : options)
        {
            if (val == value)
            {
                if (prev.has_value())
                {
                    updateValue(click, *prev);
                }
                else
                {
                    if (holding)
                        return false;
                    updateValue(click, options.back().first);
                }
                return true;
            }
            prev = val;
        }
        return true;
    }

    bool CommandListSelect::onRight(Click& click, bool holding)
    {
        bool next = false;
        for (const auto& [val, lbl] : options)
        {
            if (next)
            {
                updateValue(click, val);
                return true;
            }
            if (val == value)
                next = true;
        }
        if (next)
        {
            if (holding)
                return false;
            updateValue(click, options.front().first);
        }
        return true;
    }

    void CommandListSelect::setValue(Click& click, long long new_val)
    {
        if (this->value == new_val)
            return;
        for (const auto& [val, lbl] : options)
        {
            if (val == new_val)
            {
                const long long prev = this->value;
                this->value = new_val;
                updateState(click);
                onChange(click, prev);
                return;
            }
        }
    }

    std::string CommandListSelect::getState() const
    {
        if (auto lbl = getLabelForValue(value))
            return lbl->getEnglishUtf8();
        return {};
    }

    std::string CommandListSelect::getDefaultState() const
    {
        if (auto lbl = getLabelForValue(default_value))
            return lbl->getEnglishUtf8();
        return {};
    }

    void CommandListSelect::setState(Click& click, const std::string& state)
    {
        if (state.empty())
        {
            applyDefaultState();
            return;
        }
        for (const auto& [val, lbl] : options)
        {
            if (state == lbl.getEnglishUtf8())
            {
                setValue(click, val);
                return;
            }
        }
    }

    void CommandListSelect::applyDefaultState()
    {
        if (value != default_value)
        {
            const long long prev = value;
            value = default_value;
            Click click(CLICK_BULK, TC_APPLYDEFAULTSTATE);
            updateState(click);
            onChange(click, prev);
        }
    }
}
