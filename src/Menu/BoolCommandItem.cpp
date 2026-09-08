#include "Menu/Items.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	BoolCommandItem::BoolCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<CommandToggleLegacy>(id)),
	    m_LabelOverride(label_override)
	{
	}

	// No-op - this whole classic tree no longer has any renderer of its
	// own (ClassicUI, the DirectXTK widget kit that used to draw it, was
	// removed once the project moved fully onto the Stand-style Grid
	// menu - see Menu/UIManager.hpp's own class comment). Still reachable
	// from Lua via a Group's own :draw() method (Scripting/LuaMenu.cpp's
	// GroupDraw), which recurses into every child item's Draw() - kept
	// as a harmless no-op rather than deleted so that call doesn't need
	// its own special-casing.
	void BoolCommandItem::Draw()
	{
	}
}
