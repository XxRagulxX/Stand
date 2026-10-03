#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandColourRGB.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <cmath>

namespace Stand
{
    class CommandWorldBorder : public CommandSlider
    {
    public:
        explicit CommandWorldBorder(CommandList* parent, CommandColourRGB* colour)
            : CommandSlider(parent, LIT("World Border"), CMDNAMES("worldborder"),
                LIT("Shows the world border if you're within the specified amount of metres to it."),
                0, 12000, 0, 30)
            , m_colour(colour)
        {
        }

        ~CommandWorldBorder() override
        {
            if (m_ticking) CommandTickDispatch::RemoveCommand(this);
        }

        void onChange(Click& click, int) override
        {
            if (value != 0)
            {
                step_size = static_cast<float>(value);
                if (step_size <= 6.0f) step_size = 6.0f;
                step_size = step_size / 30.0f;

                if (!m_ticking)
                {
                    CommandTickDispatch::AddCommand(this);
                    m_ticking = true;
                }
            }
            else
            {
                if (m_ticking)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            }
        }

        void onTick() override
        {
            if (value == 0)
            {
                if (m_ticking)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
                return;
            }

            const Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), TRUE);
            processBorderX(pos, world_x_min);
            processBorderX(pos, world_x_max);
            processBorderY(pos, world_y_min);
            processBorderY(pos, world_y_max);
            processBorderZ(pos, world_z_min);
            processBorderZ(pos, world_z_max);
        }

    private:
        CommandColourRGB* m_colour;
        float step_size = 1.0f;
        bool m_ticking = false;

        inline static float world_x_min = -4000.0f;
        inline static float world_x_max =  4500.0f;
        inline static float world_y_min = -4000.0f;
        inline static float world_y_max =  8000.0f;
        static constexpr float world_z_min = -200.0f;
        static constexpr float world_z_max = 2700.0f;

        void processBorderX(const Vector3& ped_pos, float x)
        {
            const float dist = std::abs(ped_pos.x - x);
            if (dist > static_cast<float>(value)) return;
            const int a = 255 - static_cast<int>(dist / static_cast<float>(value) * 255.0f);
            const int r = m_colour->m_r, g = m_colour->m_g, b = m_colour->m_b;
            for (float y = world_y_min; y < world_y_max; y += step_size)
                GRAPHICS::DRAW_LINE(x, y, world_z_min, x, y, world_z_max, r, g, b, a);
            for (float z = world_z_min; z < world_z_max; z += step_size)
                GRAPHICS::DRAW_LINE(x, world_y_min, z, x, world_y_max, z, r, g, b, a);
        }

        void processBorderY(const Vector3& ped_pos, float y)
        {
            const float dist = std::abs(ped_pos.y - y);
            if (dist > static_cast<float>(value)) return;
            const int a = 255 - static_cast<int>(dist / static_cast<float>(value) * 255.0f);
            const int r = m_colour->m_r, g = m_colour->m_g, b = m_colour->m_b;
            for (float x = world_x_min; x < world_x_max; x += step_size)
                GRAPHICS::DRAW_LINE(x, y, world_z_min, x, y, world_z_max, r, g, b, a);
            for (float z = world_z_min; z < world_z_max; z += step_size)
                GRAPHICS::DRAW_LINE(world_x_min, y, z, world_x_max, y, z, r, g, b, a);
        }

        void processBorderZ(const Vector3& ped_pos, float z)
        {
            const float dist = std::abs(ped_pos.z - z);
            if (dist > static_cast<float>(value)) return;
            const int a = 255 - static_cast<int>(dist / static_cast<float>(value) * 255.0f);
            const int r = m_colour->m_r, g = m_colour->m_g, b = m_colour->m_b;
            for (float x = world_x_min; x < world_x_max; x += step_size)
                GRAPHICS::DRAW_LINE(x, world_y_min, z, x, world_y_max, z, r, g, b, a);
            for (float y = world_y_min; y < world_y_max; y += step_size)
                GRAPHICS::DRAW_LINE(world_x_min, y, z, world_x_max, y, z, r, g, b, a);
        }
    };
}
