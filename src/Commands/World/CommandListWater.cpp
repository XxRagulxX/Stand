#include "Commands/World/CommandListWater.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/WaterQuad.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

namespace Stand
{
    class CommandWaterWaveBehaviour : public CommandListSelect
    {
    public:
        explicit CommandWaterWaveBehaviour(CommandList* parent)
            : CommandListSelect(parent, LIT("Wave Behaviour"), CMDNAMES("wavebehaviour", "wavebehavior"), NOLABEL,
                {
                    {0, LIT("Normal")},
                    {1, LIT("Smooth")},
                    {2, LIT("Magnet")},
                },
                0)
        {
        }

        void onChange(Click& click, long long prev_value) override
        {
            if (m_tickHandler)
            {
                m_tickHandler = nullptr;
                CommandTickDispatch::RemoveCommand(this);
            }
            if (prev_value == 1)
                WATER::RESET_DEEP_OCEAN_SCALER();

            if (value == 1)
            {
                m_tickHandler = [this]() -> bool {
                    if (value != 1)
                    {
                        WATER::RESET_DEEP_OCEAN_SCALER();
                        return false;
                    }
                    WATER::SET_DEEP_OCEAN_SCALER(0.0f);
                    return true;
                };
                CommandTickDispatch::AddCommand(this);
            }
            else if (value == 2)
            {
                m_tickHandler = [this]() -> bool {
                    if (value != 2)
                        return false;
                    const int ped = PLAYER::GET_PLAYER_PED(-1);
                    const Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                    for (float x = pos.x - 10.0f; x <= (pos.x + 10.0f); x += 1.0f)
                    {
                        for (float y = pos.y - 10.0f; y <= (pos.y + 10.0f); y += 1.0f)
                        {
                            if (MISC::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, 0, x, y, 0, FALSE) <= 5.0f)
                                WATER::MODIFY_WATER(x, y, 1.0f, 1.0f);
                        }
                    }
                    return true;
                };
                CommandTickDispatch::AddCommand(this);
            }
        }
    };

    class CommandWaterHeight : public CommandSliderFloat
    {
        std::unordered_map<uint16_t, float> og_heights;

    public:
        CommandToggleNoCorrelation* local = nullptr;

        explicit CommandWaterHeight(CommandList* parent)
            : CommandSliderFloat(parent, LIT("Z Differential"), CMDNAMES("waterheight", "waterzdifferential"), NOLABEL,
                -1000000, 1000000, 0, 100)
        {
        }

    private:
        void setWaterQuadHeightDifferential(uint16_t id, float height_diff)
        {
            WaterQuad* const quad = WaterQuad::get(id);
            if (!quad)
                return;
            auto entry = og_heights.find(id);
            if (entry == og_heights.end())
            {
                og_heights.emplace(id, quad->z);
                quad->z += height_diff;
            }
            else
            {
                quad->z = entry->second + height_diff;
            }
        }

        static void processPossibleWaterQuad(
            std::unordered_map<uint16_t, float>& og_heights,
            std::unordered_set<uint16_t>& ids,
            float x, float y, float z)
        {
            auto res = WaterQuad::idsAt(x, y);
            for (auto id : res)
            {
                float water_z;
                auto entry = og_heights.find(id);
                if (entry == og_heights.end())
                {
                    WaterQuad* quad = WaterQuad::get(id);
                    if (!quad)
                        continue;
                    water_z = quad->z;
                }
                else
                {
                    water_z = entry->second;
                }
                if (water_z + 10.0f >= z)
                    ids.emplace(id);
            }
        }

        static void processPossibleWaterQuadsInCircle(
            std::unordered_map<uint16_t, float>& og_heights,
            std::unordered_set<uint16_t>& ids,
            float posX, float posY, float posZ, float dist)
        {
            for (int i = 0; i < 8; i++)
            {
                const float rad = static_cast<float>(i) * (6.28318530f / 8.0f);
                const float dx = sinf(rad) * dist;
                const float dy = cosf(rad) * dist;
                processPossibleWaterQuad(og_heights, ids, posX + dx, posY + dy, posZ);
            }
        }

    public:
        void onChange(Click& click, int prev_value) override
        {
            m_tickHandler = [this]() -> bool {
                const bool still_active = (value != 0);
                if (local && local->m_on)
                {
                    std::unordered_set<uint16_t> ids;
                    if (still_active)
                    {
                        const int ped = PLAYER::GET_PLAYER_PED(-1);
                        const Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                        processPossibleWaterQuad(og_heights, ids, pos.x, pos.y, pos.z);
                        processPossibleWaterQuadsInCircle(og_heights, ids, pos.x, pos.y, pos.z, 3.0f);
                        processPossibleWaterQuadsInCircle(og_heights, ids, pos.x, pos.y, pos.z, 10.0f);
                    }
                    for (auto it = og_heights.begin(); it != og_heights.end(); )
                    {
                        if (ids.count(it->first) == 0)
                        {
                            WaterQuad* quad = WaterQuad::get(it->first);
                            if (quad)
                                quad->z = it->second;
                            it = og_heights.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    for (auto id : ids)
                        setWaterQuadHeightDifferential(id, getFloatValue());
                }
                else
                {
                    if (still_active)
                    {
                        for (uint16_t i = 0; i != WaterQuad::size(); ++i)
                            setWaterQuadHeightDifferential(i, getFloatValue());
                    }
                    else
                    {
                        for (auto& entry : og_heights)
                        {
                            WaterQuad* quad = WaterQuad::get(entry.first);
                            if (quad)
                                quad->z = entry.second;
                        }
                        og_heights.clear();
                    }
                }
                return still_active;
            };
            CommandTickDispatch::AddCommand(this);
        }
    };

    class CommandWaterStrengthOverride : public CommandSliderFloat
    {
    public:
        explicit CommandWaterStrengthOverride(CommandList* parent)
            : CommandSliderFloat(parent, LIT("Strength Override"), CMDNAMES("waterstrength"), NOLABEL,
                0, 100000, 0, 10)
        {
        }

        void onChange(Click& click, int prev_value) override
        {
            if (m_tickHandler)
            {
                m_tickHandler = nullptr;
                CommandTickDispatch::RemoveCommand(this);
            }
            if (value != 0)
            {
                m_tickHandler = [this]() -> bool {
                    if (value == 0)
                        return false;
                    MISC::WATER_OVERRIDE_SET_STRENGTH(getFloatValue());
                    return true;
                };
                CommandTickDispatch::AddCommand(this);
            }
        }
    };

    class CommandWaterOpacityDifferential : public CommandSlider
    {
        std::vector<WaterOpacityData> og_opacity;

    public:
        explicit CommandWaterOpacityDifferential(CommandList* parent)
            : CommandSlider(parent, LIT("Opacity Differential"), CMDNAMES("wateropacity"), NOLABEL,
                0 - 80, 255 - 22, 0)
        {
        }

        void onChange(Click& click, int prev_value) override
        {
            if (m_tickHandler)
            {
                m_tickHandler = nullptr;
                CommandTickDispatch::RemoveCommand(this);
            }

            ensureScriptThread(click, [this]() {
                const uint16_t sz = WaterQuad::size();
                if (sz > 0 && sz == static_cast<uint16_t>(og_opacity.size()))
                {
                    WaterQuad* quads = WaterQuad::get(0);
                    if (quads)
                    {
                        for (uint16_t i = 0; i < sz; i++)
                            quads[i].opacity = og_opacity[i];
                    }
                }
                og_opacity.clear();

                if (value != 0)
                {
                    m_tickHandler = [this]() -> bool {
                        if (value == 0)
                            return false;
                        const uint16_t sz = WaterQuad::size();
                        if (sz == 0)
                            return true;
                        if (sz != static_cast<uint16_t>(og_opacity.size()))
                        {
                            WaterQuad* quads = WaterQuad::get(0);
                            if (!quads)
                                return true;
                            og_opacity.clear();
                            og_opacity.reserve(sz);
                            const int v = value;
                            for (uint16_t i = 0; i < sz; i++)
                            {
                                og_opacity.push_back(quads[i].opacity);
                                quads[i].opacity.a1 = static_cast<uint8_t>(std::clamp(static_cast<int>(quads[i].opacity.a1) + v, 0, 255));
                                quads[i].opacity.a2 = static_cast<uint8_t>(std::clamp(static_cast<int>(quads[i].opacity.a2) + v, 0, 255));
                                quads[i].opacity.a3 = static_cast<uint8_t>(std::clamp(static_cast<int>(quads[i].opacity.a3) + v, 0, 255));
                                quads[i].opacity.a4 = static_cast<uint8_t>(std::clamp(static_cast<int>(quads[i].opacity.a4) + v, 0, 255));
                            }
                        }
                        return true;
                    };
                    CommandTickDispatch::AddCommand(this);
                }
            });
        }
    };

    CommandListWater::CommandListWater(CommandList* parent)
        : CommandList(parent, LIT("Water"))
    {
        createChild<CommandWaterWaveBehaviour>();
        auto* height = createChild<CommandWaterHeight>();
        height->local = createChild<CommandToggle>(LIT("Only Apply Z Differential To Nearby Water"), CMDNAMES_0());
        createChild<CommandWaterStrengthOverride>();
        createChild<CommandWaterOpacityDifferential>();
    }
}
