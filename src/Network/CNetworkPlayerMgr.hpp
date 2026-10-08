#pragma once
#include "Network/netPlayerMgrBase.hpp"

class CNetworkPlayerMgr : public rage::netPlayerMgrBase
{
public:
	virtual ~CNetworkPlayerMgr() = default;
	virtual void _0x40() = 0;
	virtual void _0x48() = 0;
	virtual void _0x50() = 0;
	virtual void _0x58() = 0;
	virtual void __fastcall removePlayer(CNetGamePlayer*) = 0;
};
