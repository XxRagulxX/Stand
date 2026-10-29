#include "Rendering/Notifications.hpp"

#include "Rendering/StandPort/NotifyGrid.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Toaster.hpp"

namespace Stand
{
    void Notifications::Show(const std::string& title, const std::string& message, NotificationType type, int duration, void*, const std::string&)
    {
        if (message.empty())
            return;

        std::string text = title.empty() ? message : (title + ": " + message);
        g_toaster->toast(StringUtils::utf8_to_utf16(text));
    }

    void Notifications::Clear()
    {
        g_notify_grid.clear();
    }
}
