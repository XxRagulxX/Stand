#pragma once

// Empty on purpose - this used to declare BuildSpawnVehicleMenu(), the
// classic ImGui menu builder for Vehicle > Spawn. That menu now lives in
// Rendering/VehicleSpawnNewGrid.cpp/VehicleSpawnPersonalGrid.cpp instead;
// SpawnVehicle.cpp itself stays around only for the CommandToggle
// registrations those Grid files still look up by name (see its own
// comment).
