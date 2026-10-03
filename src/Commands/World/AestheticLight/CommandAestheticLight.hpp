#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandColourRGB.hpp"
#include "Commands/World/AestheticLight/CommandAestheticLightPlacement.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <cmath>

namespace Stand
{
    class CommandAestheticLight : public CommandToggle
    {
    public:
        explicit CommandAestheticLight(CommandList* parent,
            CommandColourRGB* colour,
            CommandAestheticLightPlacement* placement,
            CommandSliderFloat* range,
            CommandSliderFloat* intensity,
            CommandSliderFloat* shadow)
            : CommandToggle(parent, LIT("Aesthetic Light")),
              m_colour(colour),
              m_placement(placement),
              m_range(range),
              m_intensity(intensity),
              m_shadow(shadow)
        {
        }

        void onEnable(Click& click) override
        {
            onChangeToggleScriptTickEventHandler(click, [this]()
            {
                if (m_on)
                {
                    const int ir = m_colour->m_r;
                    const int ig = m_colour->m_g;
                    const int ib = m_colour->m_b;
                    const float range_v     = m_range->value     / 100.0f;
                    const float intensity_v = m_intensity->value / 100.0f;
                    const float shadow_v    = m_shadow->value    / 100.0f;

                    const int ped = PLAYER::GET_PLAYER_PED(-1);

                    switch (m_placement->value)
                    {
                    case 0:
                    {
                        const Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                        drawLight(pos, ir, ig, ib, range_v, intensity_v, shadow_v);
                        break;
                    }
                    case 1:
                    {
                        const Vector3 pos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
                        drawLight(pos, ir, ig, ib, range_v, intensity_v, shadow_v);
                        break;
                    }
                    case 2:
                    {
                        const Vector3 ped_pos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                        Vector3 min_dim{}, max_dim{};
                        MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(ped), &min_dim, &max_dim);
                        const float scale = (max_dim.z - min_dim.z) * 0.5f;

                        for (float pitch = 0.0f; pitch < 360.0f; pitch += 60.0f)
                        {
                            const float sz = sinf(pitch * 3.14159265f / 180.0f) * scale;
                            const Vector3 pitch_pos = { ped_pos.x, ped_pos.y, ped_pos.z + sz };
                            for (float heading = 0.0f; heading < 360.0f; heading += 60.0f)
                            {
                                const float hrad = heading * 3.14159265f / 180.0f;
                                const Vector3 light_pos = {
                                    pitch_pos.x + (-sinf(hrad)) * (scale * 0.5f),
                                    pitch_pos.y +   cosf(hrad)  * (scale * 0.5f),
                                    pitch_pos.z
                                };
                                drawLight(light_pos, ir, ig, ib, range_v, intensity_v, shadow_v);
                            }
                        }
                        break;
                    }
                    }
                }
                return m_on;
            });
        }

    private:
        void drawLight(const Vector3& pos, int r, int g, int b, float range_v, float intensity_v, float shadow_v) const
        {
            GRAPHICS::DRAW_LIGHT_WITH_RANGEEX(pos.x, pos.y, pos.z, r, g, b, range_v, intensity_v, shadow_v);
        }

        CommandColourRGB* m_colour;
        CommandAestheticLightPlacement* m_placement;
        CommandSliderFloat* m_range;
        CommandSliderFloat* m_intensity;
        CommandSliderFloat* m_shadow;
    };
}
