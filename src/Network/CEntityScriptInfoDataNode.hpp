#pragma once
#include "Network/CProjectBaseSyncDataNode.hpp"
#include "Scripting/CGameScriptObjInfo.hpp"

struct CEntityScriptInfoDataNode : CSyncDataNodeInfrequent
{
public:
	bool m_HasScriptInfo;
	CGameScriptObjInfo m_ScriptInfo;
};
static_assert(sizeof(CEntityScriptInfoDataNode) == 0x118);