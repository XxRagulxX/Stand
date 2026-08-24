#pragma once

#include "gta_fwddecl.hpp"
#include "struct_base.hpp"
#include "rtti_shim.hpp"

#include "net.hpp"
#include "rlclancommon.hpp"
#include "rlSessionInfo.hpp"

namespace rage
{
	class netPlayer
	{
	public:
		RAGE_RTTI_SHIM
		/* 0x30 */ virtual ~netPlayer() = default;
		/* 0x38 */ virtual void Shutdown() {}
		/* 0x40 */ virtual bool IsPhysical() const { return true; }
		/* 0x48 */ virtual const char* GetLogName() const { return nullptr; }
		/* 0x50 */ virtual uint64_t GetRlPeerId() { return 0; } // guess, from rdr3
		/* 0x58 */ virtual void ActiveUpdate() {}
		/* 0x60 */ virtual bool IsHost() { return false; }
		/* 0x68 */ virtual rage::rlGamerInfo* GetGamerInfo() const { return nullptr; }

		[[nodiscard]] const rage::rlGamerInfo* GetGamerInfoImpl() const;

		PAD(0x08, 0x58) int32_t channel_7_cxn_id;
		EndpointId endpoint_id;
		uint8_t active_id; // "Active Index" (0x2C in 323)
		uint8_t player_id; // "Physical Index" (0x2D in 323)
		uint32_t m_NatType;
		PAD(0x28, 0x8C) uint32_t m_sizeOfNetArrayData;

		[[nodiscard]] rage::netEndpoint* getEndpoint() const;
	};
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
	static_assert(offsetof(netPlayer, player_id) == 0x61); // 3889: 0xC1 -> 0x61 (-0x60)
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
}

#define MAX_BUBBLES 10
#define INVALID_BUBBLE_ID MAX_BUBBLES

class CRoamingBubbleMemberInfo
{
public:
	uint32_t m_BubbleID;
	uint32_t m_PlayerID;

	[[nodiscard]] bool IsValid() const noexcept
	{
		return m_BubbleID != INVALID_BUBBLE_ID;
	}
};

#pragma pack(push, 1)
class CNetGamePlayer : public rage::netPlayer
{
public:
	/* 0x70 */ virtual void DirtyGamerDataSyncNode() {}

	enum Flags
	{
		IS_QA							= 1 << 0, // CNetGamePlayer::ActiveUpdate, CNetObjPlayer::SetPlayerGamerData
		IS_DEV							= 1 << 1, // CNetGamePlayer::ActiveUpdate, CNetObjPlayer::SetPlayerGamerData, "RemoteIsRockstarDev"
		IS_CHEATER						= 1 << 2, // CNetGamePlayer::ActiveUpdate
		HAS_COMMUNICATION_PRIVILEGES	= 1 << 4,
	};

	INIT_PAD(rage::netPlayer, 0xE8) CPlayerInfo* player_info;
	/* 0x0A8 */ uint32_t m_MatchmakingGroup;
	/* 0x0AC */ bool m_bIsSpectator;
	PAD(0x0AC + 1, 0x0B0) CRoamingBubbleMemberInfo m_RoamingBubbleMemberInfo;
	/* 0x0B8 */ rage::rlClanMembershipData clan_membership;
	/* 0x170 */ char m_crewRankTitle[RL_CLAN_NAME_MAX_CHARS];
	PAD(0x170 + RL_CLAN_NAME_MAX_CHARS, 0x190) rage::rlClanId m_nCrewId;
	PAD(0x190 + 8, 0x2B0) uint32_t m_TextChatFlags; // 81 A1 ? ? ? ? F0 00 00 00
	/* 0x2AC */ bool m_TextChatFlagsDirty;
	PAD(0x2AC + 1, 0x2AF) uint32_t m_cheatsNotified;
	PAD(0x2AF + 4, 0x30C) uint32_t flags;
	PAD(0x30C + 4, 0x318) uint32_t m_PlayerAccountId;

	[[nodiscard]] rage::rlGamerInfo* GetGamerInfoImpl();
	[[nodiscard]] const rage::rlGamerInfo* GetGamerInfoImpl() const;
	[[nodiscard]] bool isHost() const;

	[[nodiscard]] bool IsRockstarDev() const { return flags & IS_DEV; }
	[[nodiscard]] bool IsRockstarQA() const { return flags & IS_QA; }
	[[nodiscard]] bool IsCheater() const { return flags & IS_CHEATER; }
};
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
static_assert(offsetof(CNetGamePlayer, player_info) == 0xE8); // 3889: 0x148 -> 0xE8 (-0x60)
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#pragma pack(pop)
