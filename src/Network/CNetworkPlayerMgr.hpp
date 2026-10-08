#pragma once
#include "Network/netPlayerMgrBase.hpp"
#include "Network/CNetGamePlayer.hpp"
#include "Game/gta_player.hpp"

class CNetworkPlayerMgr : public rage::netPlayerMgrBase
{
public:
	virtual ~CNetworkPlayerMgr() = default;
	virtual void _0x08() = 0;
	virtual void _0x10() = 0;
	virtual void _0x18() = 0;
	virtual void _0x20() = 0;
	virtual void __fastcall removePlayer(CNetGamePlayer*) = 0;

private:
	char pad_0x0008[0x00E8 - 0x0008];
	void* unk_0x0E8;
public:
	CNetGamePlayer* localPlayer;
	char pad_0x0F8[0x180 - 0x0F8];
	int32_t NumPlayers;
	char pad_0x184[0x188 - 0x184];
	CNetGamePlayer* Players[32];

	[[nodiscard]] compactplayer_t getHostIndex() const;
};