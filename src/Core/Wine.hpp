#include "Core/ModuleMgr.hpp"
#include "Util/Joaat.hpp"

#include <optional>

namespace Stand {
    inline std::optional<bool> InWine()
    {
        auto module = g_ModuleMgr.Get("ntdll.dll"_J);
        if (!module)
            return std::nullopt;
        return module->IsExported("wine_get_version");
    }
}