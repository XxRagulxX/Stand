#include "Game/fwRefAwareBase.hpp"

#include "Game/pointers.hpp"

void rage::fwRefAwareBase::RemoveKnownRef(void** ppReference)
{
	Stand::pointers::remove_known_ref(this, ppReference);
}
