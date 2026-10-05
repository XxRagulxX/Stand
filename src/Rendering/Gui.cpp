#include "Rendering/Gui.hpp"
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    Gui g_gui{};

    CommandList* Gui::getCurrentUiList() const noexcept
    {
        if (m_active_list.empty())
            return root_list.get();
        return m_active_list.back();
    }
}
