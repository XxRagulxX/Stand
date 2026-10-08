#pragma once

#include <cstdint>

namespace rage
{
	using atHashValue = uint32_t;

	using netSequence = uint16_t;

	template <typename T>
	class atArray;

	template <typename T, size_t S>
	struct atFixedArray;

	class datBitBuffer;
	class datExportBuffer;
	class datImportBuffer;
	class RsonWriter;

	class sysMemAllocator;

	class scriptIdBase;
	class scriptId;
	class scriptHandler;
	class scriptHandlerNetComponent;
	class scriptHandlerObject;
	class scriptHandlerMgr;

	class scrProgram;
	class scrProgramTable;

	class scrThreadContext;
	class scrThread;
	union scrValue;
	class tlsContext;

	class scriptResource;

	class netLoggingInterface;
	class netLogStub;

	class netPlayer;
	class netPlayerMgrBase;

	struct netPeerAddress;
}
#include "Network/netPeerId.hpp"
namespace rage
{

	class netConnectionManager;
	class netEndpoint;
	class netAddress;
	class netConnection;
	class Cxn;
	struct netTunnelDesc;

	class ReceivedMessageData;

	class netIpAddress;
	class netSocketAddress;

	class netGameEvent;
	class netEventMgr;

	class scrNativeCallContext;
	class scrNativeRegistrationTable;

	struct Vector2;
	struct Vector3;
	struct vector4;
	struct scrVector3;
	struct scrVector4;

	class Matrix34;

	class fwRefAwareBase;
	class fwExtensibleBase;
	class fwExtensionList;
	class fwExtension;
	class fwEntity;
	class fwArchetype;

	class fwModelId;

	class GenericPool;
	class VehiclePool;

	struct rlGamerHandle;
	struct rlClanMembershipData;
	struct rlPeerInfo;
	class rlGamerInfo;

	class snSession;
	class snPeer;
	class rlSession;
	struct rlSessionInfo;
	struct rlSessionDetail;
	struct rlSessionDetailResponse;
	struct rlMatchingAttributes;
	struct rlScMatchmakingMatchId;

	struct snJoinSessionTask;
	struct snMigrateSessionTask;

	class snEvent;

	struct rlGetGamerStateTask;
	struct rlScMatchmakingFindTask;

	struct rlMetric;

	class rlMatchingFilter;
	struct netStatus;
	struct parTreeNode;
	struct rlRosResult;

	struct phArchetype;
	struct phArchetypeDamp;

	struct phCollider;

	struct rlPresenceEvent;
	struct rlPresenceEventJoinedViaPresence;

	struct rlRosCredentials;
	class rlRosGeoLocInfo;

	template <typename T>
	class sysObfuscated_Mutate;

}
#include "Game/fwPool.hpp"
namespace rage
{

	struct fwScriptGuid;

	struct ClassId;

	struct fwProfanityFilter;

	enum eMigrationType : int32_t;

	struct sysDependency;

	class rlProfileStatsReadResults;

	class netObject;
	class netObjectMgrBase;
}

class GtaThread;

class CGameScriptId;
class CGameScriptHandler;
class CGameScriptHandlerNetwork;
class CGameScriptHandlerMgr;

class CEntity;
class CDynamicEntity;
class CPhysical;

class CObject;
class CPed;
class CPickup;
class CProjectile;
class CRotaryWingAircraft;
class CTrain;
class CVehicle;
class CPlane;

class CPedFactory;
class CVehicleFactory;

class CVehicleGadget;
class CVehicleWeapon;

class CNetGamePlayer;
class CNetGamePlayerDataMsg;
class CNetworkPlayerMgr;
class CPlayerInfo;
class CNetworkObjectMgr;

class CReplayInterfaceGame;

class CScriptedGameEvent;
class CNetworkIncrementStatEvent;

struct CNetworkTextChat;
struct CMultiplayerChat;

class CNetworkVoice;

class CWeapon;
class CWeaponInfo;
class CWeaponInfoManager;

struct WaterQuad;

class CPedHeadBlendData;

class CBaseModelInfo;
class CVehicleModelInfo;
enum VehicleType : unsigned int;

enum eCarLockState : uint32_t;

class CVehicleModelInfoData;
class CVehicleLayoutInfo;

class CDrivebyWeaponGroup;
class CVehicleDriveByInfo;

struct CVehicleMetadataMgr;

class CVehicleDamage;
class CAircraftDamage;

class CHandlingData;
class CHandlingDataMgr;

class CNetworkSession;
class CBlacklistedGamers;

class CExtraContentManager;
class CNetworkAssetVerifier;

class CNetObjEntity;
class CNetObjPhysical;
struct CNetObjPlayer;
class CNetObjVehicle;
