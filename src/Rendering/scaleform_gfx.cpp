#include "Rendering/scaleform_gfx.hpp"

#include "Rendering/actionscript.hpp"
#include "Game/pointers.hpp"

bool GFxValue::ObjectInterface::GetMember(void* pdata, const char* name, GFxValue* pval) const
{
	return Stand::pointers::GFxValue_ObjectInterface_GetMember(this, pdata, name, pval, false);
}

unsigned int GFxValue::ObjectInterface::GetArraySize(void* pdata) const
{
	GASObjectInterface* pobj = static_cast<GASObjectInterface*>(pdata);
	const GASArrayObject* parr = static_cast<const GASArrayObject*>(pobj);
	return parr->size;
}
