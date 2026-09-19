#include "Rendering/VehiclePreviewItem.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/Vehicle.hpp"
#include "Commands/Vehicle/CommandTabVehicle.hpp"
#include "Vehicle/VehicleSpawnBehaviour.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <cmath>

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

    VehiclePreviewItem::VehiclePreviewItem(int16_t width, int16_t height, joaat_t hash)
        : GridItem(GRIDITEM_INDIFFERENT, width, height), m_Hash(hash)
    {
    }

    VehiclePreviewItem::~VehiclePreviewItem()
    {
        StopPreview();
    }

    void VehiclePreviewItem::EnsureDisplayName()
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

    std::string VehiclePreviewItem::CurrentDisplayName() const
    {
        if (!m_MakerName.empty() && Features::GetCommandTabVehicle().manufacturerNames->m_on)
            return m_MakerName + " " + m_ModelName;
        return m_ModelName;
    }

    void VehiclePreviewItem::StartPreview()
    {
        StopPreview();
        m_Fiber = std::make_shared<PreviewFiber>();
        auto fiber = m_Fiber;
        const auto hash = m_Hash;

        FiberPool::queueJob([this, fiber, hash] {
            while (!STREAMING::HAS_MODEL_LOADED(hash))
            {
                if (!fiber->active.load(std::memory_order_relaxed))
                    return;
                STREAMING::REQUEST_MODEL(hash);
                Script::current()->yield();
            }
            if (!fiber->active.load(std::memory_order_relaxed))
                return;

            Vector3 mn, mx;
            MISC::GET_MODEL_DIMENSIONS(hash, &mn, &mx);
            const float dimMax = std::max({ mx.x - mn.x, mx.y - mn.y, mx.z - mn.z });
            const float totalOffset = dimMax + Theme::kPreviewVehDist + 2.f;

            const auto camPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
            const auto camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
            const float yawRad = camRot.z * (3.14159265f / 180.f);

            const rage::fvector3 spawnPos{
                camPos.x - std::sin(yawRad) * totalOffset,
                camPos.y + std::cos(yawRad) * totalOffset,
                camPos.z,
            };

            Vehicle veh = Vehicle::Create(hash, spawnPos, camRot.z, false);
            if (!veh.IsValid())
                return;

            const int vehHandle = veh.GetHandle();
            float rotDeg = camRot.z;
            rage::fvector3 rot{};
            rot.z = rotDeg;
            veh.SetRotation(rot);

            while (fiber->active.load(std::memory_order_relaxed) && isKeyboardFocused())
            {
                if (!veh.IsValid())
                    break;

                ENTITY::SET_ENTITY_HAS_GRAVITY(vehHandle, FALSE);
                ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(vehHandle, FALSE, FALSE);

                rotDeg += Theme::kPreviewRotationSpeed;
                rot.z = rotDeg;
                veh.SetRotation(rot);

                const auto curCamPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
                const auto curCamRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
                const float curYawRad = curCamRot.z * (3.14159265f / 180.f);
                const rage::fvector3 newPos{
                    curCamPos.x - std::sin(curYawRad) * totalOffset,
                    curCamPos.y + std::cos(curYawRad) * totalOffset,
                    curCamPos.z,
                };
                veh.SetPosition(newPos);

                int alpha = 255;
                if (!Theme::kPreviewOpaque)
                {
                    const float speedPerc = std::min(Self::GetPed().GetSpeed() / 80.f, 1.f);
                    alpha = static_cast<int>(200.f - speedPerc * 150.f);
                }
                veh.SetAlpha(alpha);

                const int br = static_cast<int>(Theme::kAccent.x * 255.f);
                const int bg = static_cast<int>(Theme::kAccent.y * 255.f);
                const int bb = static_cast<int>(Theme::kAccent.z * 255.f);
                const int ba = static_cast<int>(Theme::kAccent.w * 200.f);
                DrawBoundingBox(vehHandle, mn, mx, br, bg, bb, ba);

                Script::current()->yield();
            }

            if (veh.IsValid())
            {
                const auto model = veh.GetModel();
                veh.Delete();
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
            }
        });
    }

    void VehiclePreviewItem::StopPreview()
    {
        if (m_Fiber)
        {
            m_Fiber->active.store(false, std::memory_order_relaxed);
            m_Fiber.reset();
        }
    }

    void VehiclePreviewItem::draw()
    {
        if (isKeyboardFocused())
            GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

        if (Theme::kDisableEntityPreviews)
        {
            m_FocusTracker.Update(false);
            if (m_FocusTracker.JustBlurred())
                StopPreview();
            return;
        }

        const bool focused = isKeyboardFocused();
        const bool justFocused = m_FocusTracker.Update(focused);
        if (justFocused)
            StartPreview();
        else if (m_FocusTracker.JustBlurred())
            StopPreview();
    }

    void VehiclePreviewItem::drawText()
    {
        EnsureDisplayName();
        const auto name  = CurrentDisplayName();
        const auto size  = GridRenderer::MeasureText(name.c_str());
        const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
        GridRenderer::DrawText(x + 5.f, textY, name.c_str(), Theme::kText);
    }

    void VehiclePreviewItem::onClick(int16_t, int16_t)
    {
        activate();
    }

    void VehiclePreviewItem::activate()
    {
        EnsureDisplayName();
        const auto hash = m_Hash;
        const auto name = CurrentDisplayName();
        FiberPool::queueJob([hash, name] {
            Stand::SpawnVehicle(hash, name);
        });
    }
}
