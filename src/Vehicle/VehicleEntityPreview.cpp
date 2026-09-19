#include "Vehicle/VehicleEntityPreview.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Vehicle/VehicleSpawnBehaviour.hpp"

#include <algorithm>

namespace Stand::Rendering
{
    namespace
    {
        void DrawBoundingBox(int entHandle, const Vector3& mn, const Vector3& mx, int r, int g, int b, int a)
        {
            const rage::fvector3 c[8] = {
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mn.y, mn.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mn.y, mn.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mx.y, mn.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mx.y, mn.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mn.y, mx.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mn.y, mx.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mx.y, mx.z),
                ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mx.y, mx.z),
            };
            auto line = [&](int i, int j) {
                GRAPHICS::DRAW_LINE(c[i].x, c[i].y, c[i].z, c[j].x, c[j].y, c[j].z, r, g, b, a);
            };
            line(0, 1); line(0, 2); line(1, 3); line(2, 3);
            line(4, 5); line(4, 6); line(5, 7); line(6, 7);
            line(0, 4); line(1, 5); line(2, 6); line(3, 7);
        }
    }

    VehicleEntityPreview::VehicleEntityPreview(int16_t width, int16_t height, joaat_t hash)
        : GridItemEntityPreview(width, height), m_Hash(hash)
    {
    }

    void VehicleEntityPreview::EnsureDisplayName()
    {
        if (m_NameLoaded)
            return;
        m_NameLoaded = true;

        std::string gxt = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(m_Hash);
        std::string display = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(gxt.c_str());
        m_ModelName = display == "NULL" ? gxt : display;

        std::string maker = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(
            VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(m_Hash));
        m_MakerName = (maker == "NULL") ? "" : maker;
    }

    void VehicleEntityPreview::EnsureModelDims() const
    {
        if (m_DimsLoaded)
            return;
        m_DimsLoaded = true;
        MISC::GET_MODEL_DIMENSIONS(m_Hash, &m_ModelMin, &m_ModelMax);
    }

    std::string VehicleEntityPreview::CurrentDisplayName() const
    {
        if (!m_MakerName.empty() && Features::GetCommandTabSpawnSettings().manufacturerNames->m_on)
            return m_MakerName + " " + m_ModelName;
        return m_ModelName;
    }

    std::uint32_t VehicleEntityPreview::GetModelHash() const
    {
        return m_Hash;
    }

    Entity VehicleEntityPreview::CreateEntity(const rage::fvector3& pos) const
    {
        const auto camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
        const int h = VEHICLE::CREATE_VEHICLE(m_Hash, pos.x, pos.y, pos.z, camRot.z, false, false, false);
        if (!ENTITY::DOES_ENTITY_EXIST(h))
            return Entity(0);
        ENTITY::SET_ENTITY_AS_MISSION_ENTITY(h, true, true);
        return Entity(h);
    }

    float VehicleEntityPreview::GetAdditionalOffset() const
    {
        EnsureModelDims();
        const float dimMax = std::max({ m_ModelMax.x - m_ModelMin.x,
                                        m_ModelMax.y - m_ModelMin.y,
                                        m_ModelMax.z - m_ModelMin.z });
        return dimMax + Theme::kPreviewVehDist - 1.f;
    }

    void VehicleEntityPreview::OnPreviewTick(Entity& entity)
    {
        EnsureModelDims();
        const int h = entity.GetHandle();
        const int br = static_cast<int>(Theme::kAccent.x * 255.f);
        const int bg = static_cast<int>(Theme::kAccent.y * 255.f);
        const int bb = static_cast<int>(Theme::kAccent.z * 255.f);
        const int ba = static_cast<int>(Theme::kAccent.w * 200.f);
        DrawBoundingBox(h, m_ModelMin, m_ModelMax, br, bg, bb, ba);
    }

    void VehicleEntityPreview::draw()
    {
        if (isKeyboardFocused())
            GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
        GridItemEntityPreview::draw();
    }

    void VehicleEntityPreview::drawText()
    {
        EnsureDisplayName();
        const auto name  = CurrentDisplayName();
        const auto size  = GridRenderer::MeasureText(name.c_str());
        const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
        GridRenderer::DrawText(x + 5.f, textY, name.c_str(), Theme::kText);
    }

    void VehicleEntityPreview::onClick(int16_t, int16_t)
    {
        activate();
    }

    void VehicleEntityPreview::activate()
    {
        EnsureDisplayName();
        ClearPreview();
        const auto hash = m_Hash;
        const auto name = CurrentDisplayName();
        FiberPool::queueJob([hash, name] {
            Stand::SpawnVehicle(hash, name);
        });
    }
}
