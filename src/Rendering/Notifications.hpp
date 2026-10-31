#pragma once

#include <string>

namespace Stand
{
    enum class NotificationType
    {
        Info,
        Success,
        Warning,
        Error
    };

    class Notifications
    {
    public:
        static void Show(const std::string& title, const std::string& message, NotificationType type = NotificationType::Info, int duration = 5000, void* = nullptr, const std::string& = "");
        static void Clear();
        static void Draw() {}
        static void DrawText() {}
        static void SetPreviewActive(bool) {}
        static void FlashPreview() {}
    };
}
