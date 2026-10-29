#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <ctime>
#include <functional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
    class GridItemNotify : public GridItem
    {
    public:
        const time_t show_until;
        time_t flash_until;
        bool frozen;
        bool flashing;

        explicit GridItemNotify(std::string&& text, time_t show_until, time_t flash_until, std::function<void()> on_expire);
        explicit GridItemNotify(const std::wstring& text, time_t show_until, time_t flash_until);
        ~GridItemNotify();

        void draw() final;

    protected:
        void drawText();

    private:
        const std::string text;
        const std::function<void()> m_OnExpire;

        static std::vector<GridItemNotify*> s_Instances;
    };
}
