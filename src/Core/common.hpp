#pragma once

// This is the PCH (pre-compiled header). It makes Stand ~33% faster to compile.

// Add headers here that are: Commonly used, mid-large size, and seldom modified.
// - Headers that are too rare or simple might reduce compile time if added here.
// - Headers that are often modified will make working on the project more painful.

// C
#include <cstddef>
#include <cstdint>

// C++
#include <array>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// GTA V
#include "Game/gta_fwddecl.hpp"

#include "Network/rlGamerInfo.hpp"

// Soup
#include "lib/soup/ObfusString.hpp"

// Stand
#include "Game/fwddecl.hpp"
#include "Game/typedecl.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

#include "Localization/lang.hpp"

#include "Game/natives.hpp"
