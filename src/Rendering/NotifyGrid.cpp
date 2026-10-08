#include "Rendering/NotifyGrid.hpp"

#include "Core/ExceptionHandler.hpp"
#include "Rendering/StandRendererCompat.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Util.hpp"

namespace Stand
{
    NotifyGrid::NotifyGrid()
        : Rendering::Grid(320, 1060, 3)
    {
    }

    void NotifyGrid::onTick()
    {
        const bool minimap_visible = (!HUD::IS_WARNING_MESSAGE_ACTIVE() && !HUD::IS_PAUSE_MENU_ACTIVE() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS());
        const bool bigmap = Util::is_bigmap_active();
        const bool new_invites_indicator = (ScriptGlobal(GLOBAL_NEW_INVITES).get<int>() > 0);
        const float safe_zone = GRAPHICS::GET_SAFE_ZONE_SIZE();
        const float client_size_y = g_renderer.client_size.y;
        if (this->minimap_visible != minimap_visible
            || this->bigmap != bigmap
            || this->new_invites_indicator != new_invites_indicator
            || this->safe_zone != safe_zone
            || this->client_size_y != client_size_y
            )
        {
            this->minimap_visible = minimap_visible;
            this->bigmap = bigmap;
            this->new_invites_indicator = new_invites_indicator;
            this->safe_zone = safe_zone;
            this->client_size_y = client_size_y;
            if (next_to_map)
            {
                setOriginNextToMap();
                if (items)
                {
                    setPositions(*items, 0);
                }
            }
        }
    }

    void NotifyGrid::setOriginNextToMap()
    {
        const float safezone_scale = (1.0f - ((safe_zone * 10.0f) - 9.0f));
        const float safezone_width = (96.0f * safezone_scale);
        const float safezone_height = (54.0f * safezone_scale);
        origin.x = int16_t(0.2f + safezone_width);
        origin.y = int16_t(g_renderer.posC2H(0.0f, client_size_y - 3.8f).y - safezone_height);
        if (minimap_visible)
        {
            origin.x += 291;
            if (bigmap)
            {
                origin.x += 158;
                if (new_invites_indicator)
                {
                    origin.x += 60;
                }
            }
            else
            {
                if (new_invites_indicator)
                {
                    origin.x += 39;
                }
            }
        }
    }

    void NotifyGrid::populate(std::vector<std::unique_ptr<Rendering::GridItem>>& items_draft)
    {
        if (!persistent_notify.empty())
        {
            items_draft.emplace_back(std::make_unique<Rendering::GridItemNotify>(persistent_notify, 0, persistent_notify_flash_until));
        }
        EXCEPTIONAL_LOCK(notifies_mtx)
        const auto time = get_current_time_millis();
        uint8_t live_notifies = 0;
        for (auto i = notifies.begin(); i != notifies.end(); )
        {
            if (!i->live)
            {
                if (live_notifies > 10)
                {
                    ++i;
                    continue;
                }
                i->flash_time += time;
                i->live = true;
            }
            if (i->expiry_time != 0)
            {
                if (isFrozen() ? wasDeadlineReached(i->expiry_time) : IS_DEADLINE_REACHED(i->expiry_time))
                {
                    i = notifies.erase(i);
                    continue;
                }
            }
            else if (live_notifies == 0)
            {
                i->expiry_time = get_current_time_millis() + (i->display_time / i->expiry_divisor);
            }
            else if (i->expiry_divisor == 1)
            {
                i->expiry_divisor = live_notifies + 1;
            }
            items_draft.emplace_back(std::make_unique<Rendering::GridItemNotify>(i->text, i->expiry_time, i->flash_time));
            ++live_notifies;
            ++i;
        }
        EXCEPTIONAL_UNLOCK(notifies_mtx)
    }

    void NotifyGrid::clear()
    {
        EXCEPTIONAL_LOCK(notifies_mtx)
        notifies.clear();
        EXCEPTIONAL_UNLOCK(notifies_mtx)
        update();
    }

    bool NotifyGrid::isFrozen() const noexcept
    {
        return frozen_since != 0;
    }

    bool NotifyGrid::wasDeadlineReached(time_t deadline) const noexcept
    {
        return frozen_since >= deadline;
    }

    void NotifyGrid::freeze()
    {
        frozen_since = get_current_time_millis();
        auto items_capture{ items };
        for (const auto& item : *items_capture)
        {
            static_cast<Rendering::GridItemNotify*>(item.get())->frozen = true;
        }
    }

    void NotifyGrid::unfreeze()
    {
        const auto delta = (get_current_time_millis() - frozen_since);
        EXCEPTIONAL_LOCK(notifies_mtx)
        for (auto i = notifies.begin(); i != notifies.end(); ++i)
        {
            if (i->live)
            {
                i->flash_time += delta;
            }
            if (i->expiry_time != 0)
            {
                i->expiry_time += delta;
            }
        }
        EXCEPTIONAL_UNLOCK(notifies_mtx)
        frozen_since = 0;
        update();
    }
}
