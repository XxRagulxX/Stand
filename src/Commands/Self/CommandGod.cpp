#include "Commands/Self/CommandGod.hpp"

// No "Self" Stand::CommandList root exists anywhere in this codebase yet
// (the only one that exists at all is CommandStandWidgetsTest5.cpp's own
// private test root) - parent=nullptr here, same as real Stand's own
// CommandList::isRoot() convention for a list with no parent, except
// this is a leaf command rather than a list. Nothing in this project's
// own port ever dereferences a command's own `parent` for a leaf
// (CommandList::createChild() is what actually links a child into a
// list's own `children` vector and takes ownership - parent is metadata
// only, see Commands/Widgets/Command.hpp's own constructor), so a
// nullptr-parented leaf owned by a function-local static (same
// lifetime/ownership shape the legacy `static Godmode _Godmode{...}`
// this replaces already had) is safe and requires no tree to exist.
//
// Function-local static rather than a plain namespace-scope global -
// same "avoid static-init-order issues, safe on first real use" reasoning
// CommandStandWidgetsTest5.cpp's own GetStandTreeTestRoot() already
// documents (FiberPool/CommandRegistry/CommandTickDispatch all need to
// already exist by the time this constructs, which isn't guaranteed at
// arbitrary global-init time but is guaranteed by the time SelfGrid's
// own populate() first runs, well after Main() has finished setting up).
namespace Stand::Features
{
	Stand::CommandGod& GetCommandGod()
	{
		static Stand::CommandGod instance{nullptr};
		return instance;
	}
}
