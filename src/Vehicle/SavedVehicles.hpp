#pragma once

// Empty on purpose - this used to declare BuildSavedVehiclesMenu(), the
// classic ImGui menu builder for Vehicle > Saved Vehicles. That menu now
// lives in Rendering/SavedVehiclesGrid.cpp instead; SavedVehicles.cpp
// itself stays around only for the CommandToggle registration that Grid
// file still looks up by name (see its own comment).
