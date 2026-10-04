#pragma once
#include <functional>
#include <optional>
#include <stack>
#include "Scripting/Natives.hpp"

namespace Stand
{
    struct TpUtil
    {
        struct PositionState
        {
            Vector3 coords;
            float heading;
            int veh;
        };

        inline static std::optional<Vector3> last_tp{};
        inline static std::stack<PositionState> undo_stack{};

        static void teleportWithRedirects(float x, float y, float z, bool z_is_guessed, std::function<void()>&& callback = {});
        static void teleport(float x, float y, float z, bool z_is_guessed, std::function<void()>&& callback = {});
        static void teleport_exact(float x, float y, float z);
        static void undo_teleport();
        static void teleport_to_blip(Blip blip);
        static void teleport_to_veh(Vehicle veh);

    private:
        struct TpCoords { float x, y, z; bool z_exact; };

        static float vDist2D(float x1, float y1, float x2, float y2);
        static int getPlayerVehicle();
        static Vector3 getPlayerPos();
        static float getEntityHalfHeight(int entity);
        static PositionState saveState();
        static TpCoords followRedirects(float x, float y, float z, bool z_is_guessed, bool in_vehicle);
        static void onPreTp(const Vector3& pos);
        static void onPostTp(const Vector3& pos);
    };
}
