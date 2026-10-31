#pragma once

#include "Game/CDynamicEntity.hpp"

#include "Game/atArray.hpp"
#include "Game/CVehicleFlags.hpp"
#include "Game/fwRegdRef.hpp"
#include "Game/VehicleType.hpp"
#include "Util/hashtype.hpp"

#include <soup/structing.hpp>

enum eCarLockState : uint32_t;
class CHandlingData;
class CVehicleWeapon;
class CVehicleGadget;

#pragma pack(push, 1)
class CWheel
{
public:
	PAD(0x000, 0x120) CHandlingData* handling_data;
	/* 0x128 */ CVehicle* vehicle;
	PAD(0x128 + 8, 0x218) float m_fReducedGripMult;
};
static_assert(sizeof(CWheel) == 0x218 + 4);
#pragma pack(pop)

class CVehicleDamage {};

class CCarDoor
{
public:
	enum eDoorFlags : uint32_t
	{
		AXIS_X = 0x1,
		AXIS_Y = 0x2,
		AXIS_Z = 0x4,
		AXIS_SLIDE_X = 0x8,
		AXIS_SLIDE_Y = 0x10,
		AXIS_SLIDE_Z = 0x20,
		AXIS_MASK = 0x3F,
		DRIVEN_NORESET_NETWORK = 0x80,
		DRIVEN_SHUT = 0x100,
		DRIVEN_AUTORESET = 0x200,
		DRIVEN_NORESET = 0x400,
		DRIVEN_GAS_STRUT = 0x800,
		DRIVEN_SWINGING = 0x1000,
		DRIVEN_SMOOTH = 0x2000,
		DRIVEN_SPECIAL = 0x4000,
		DRIVEN_USE_STIFFNESS_MULT = 0x8000,
		DRIVEN_MASK = 0xFF80,
		DRIVEN_NOT_SPECIAL_MASK = 0x9F80,
		WILL_LOCK_SWINGING = 0x10000,
		WILL_LOCK_DRIVEN = 0x20000,
		IS_ARTICULATED = 0x40000,
		IS_BROKEN_OFF = 0x80000,
		BIKE_DOOR = 0x100000,
		AERO_BONNET = 0x200000,
		GAS_BOOT = 0x400000,
		DONT_BREAK = 0x800000,
		PROCESS_FORCE_AWAKE = 0x1000000,
		RELEASE_AFTER_DRIVEN = 0x2000000,
		SET_TO_FORCE_OPEN = 0x4000000,
		PED_DRIVING_DOOR = 0x8000000,
		PED_USING_DOOR_FOR_COVER = 0x10000000,
		LOOSE_LATCHED_DOOR = 0x20000000,
		DRIVEN_BY_PHYSICS = 0x40000000,
		RENDER_INVISIBLE = 0x80000000,
	};

	/* 0x00 */ eCarLockState m_eDoorLockState;
	PAD(0x00 + 4, 0x18) uint32_t m_nDoorFlags;
	PAD(0x18 + 4, 0x70);
};
static_assert(sizeof(CCarDoor) == 0x70);
static_assert(offsetof(CCarDoor, m_nDoorFlags) == 0x18);

struct CTransmission
{
	/* 0x00 */ int16_t m_nGear;
	/* 0x02 */ int16_t m_nPrevGear;
	PAD(0x04, 0x48) float rpm;
};

struct CVehicleWeaponMgr
{
	PAD(0, 0x68) CVehicleWeapon* m_pVehicleWeapons[6];
	PAD(0x68 + (8 * 6), 0x114) int m_iNumVehicleWeapons;
};

#pragma pack(push, 1)
class CSubmarineHandling
{
public:
	float m_fPitchControl;
	float m_fYawControl;
	float m_fDiveControl;
	float m_fMaxDepthReached;
	int32_t m_nAirLeaks;
};

class CPhysical : public CDynamicEntity
{
	struct CPhysicalFlags
	{
		uint32_t bIsInWater : 1;
		uint32_t bWasInWater : 1;
		uint32_t bDontLoadCollision : 1;
		uint32_t bAllowFreezeIfNoCollision : 1;
		uint32_t bNotDamagedByBullets : 1;
		uint32_t bNotDamagedByFlames : 1;
		uint32_t bNotDamagedByCollisions : 1;
		uint32_t bNotDamagedByMelee : 1;
		uint32_t bNotDamagedByAnything : 1;
		uint32_t bNotDamagedByAnythingButHasReactions : 1;
		uint32_t bOnlyDamagedByPlayer : 1;
		uint32_t bIgnoresExplosions : 1;
		uint32_t bOnlyDamagedByRelGroup : 1;
		uint32_t bNotDamagedByRelGroup : 1;
		uint32_t bOnlyDamagedWhenRunningScript : 1;
		uint32_t bNotDamagedBySteam : 1;
		uint32_t bNotDamagedBySmoke : 1;
		uint32_t bExplodeInstantlyWhenChecked : 1;
		uint32_t bFlyer : 1;
		uint32_t bRenderScorched : 1;
		uint32_t bCarriedByRope : 1;
		uint32_t bMoved : 1;
		uint32_t bPossiblyTouchesWaterIsUpToDate : 1;
		uint32_t bModifiedBounds : 1;
		uint32_t bIsNotBuoyant : 1;
	};
	static_assert(sizeof(CPhysicalFlags) == 0x04);

	INIT_PAD(CDynamicEntity, 0x188) CPhysicalFlags m_nPhysicalFlags;
	PAD(0x18C, 0x190) bool m_bDontResetDamageFlagsOnCleanupMissionState : 1;
	PAD(0x191, 0x194) uint32_t relationship_hash;
	PAD(0x198, 0x280) float m_health;
	/* 0x284 */ float max_health;
};
static_assert(sizeof(CPhysical) == 0x284 + 4);
static_assert(offsetof(CPhysical, relationship_hash) == 0x194);

class CVehicle : public CPhysical
{
public:
	virtual ~CVehicle() override = 0; // 0x30
	virtual void _038() = 0;
	virtual void _040() = 0;
	virtual void _048() = 0;
	virtual void _050() = 0;
	virtual void _058() = 0;
	virtual void _060() = 0;
	virtual void _068() = 0;
	virtual void _070() = 0;
	virtual void _078() = 0;
	virtual void _080() = 0;
	virtual void _088() = 0;
	virtual void _090() = 0;
	virtual void _098() = 0;
	virtual void _0A0() = 0;
	virtual void _0A8() = 0;
	virtual void _0B0() = 0;
	virtual void _0B8() = 0;
	virtual void _0C0() = 0;
	virtual void _0C8() = 0;
	virtual void _0D0() = 0;
	virtual void _0D8() = 0;
	virtual void _0E0() = 0;
	virtual void _0E8() = 0;
	virtual void _0F0() = 0;
	virtual void _0F8() = 0;
	virtual void _100() = 0;
	virtual void _108() = 0;
	virtual void _110() = 0;
	virtual void _118() = 0;
	virtual void _120() = 0;
	virtual void _128() = 0;
	virtual void _130() = 0;
	virtual void _138() = 0;
	virtual void _140() = 0;
	virtual void _148() = 0;
	virtual void _150() = 0;
	virtual void _158() = 0;
	virtual void _160() = 0;
	virtual void _168() = 0;
	virtual void _170() = 0;
	virtual void _178() = 0;
	virtual void _180() = 0;
	virtual void _188() = 0;
	virtual void _190() = 0;
	virtual void _198() = 0;
	virtual void _1A0() = 0;
	virtual void _1A8() = 0;
	virtual void _1B0() = 0;
	virtual void _1B8() = 0;
	virtual void _1C0() = 0;
	virtual void _1C8() = 0;
	virtual void _1D0() = 0;
	virtual void _1D8() = 0;
	virtual void _1E0() = 0;
	virtual void _1E8() = 0;
	virtual void _1F0() = 0;
	virtual void _1F8() = 0;
	virtual void _200() = 0;
	virtual void _208() = 0;
	virtual void _210() = 0;
	virtual void _218() = 0;
	virtual void _220() = 0;
	virtual void _228() = 0;
	virtual void _230() = 0;
	virtual void _238() = 0;
	virtual void _240() = 0;
	virtual void _248() = 0;
	virtual void _250() = 0;
	virtual void _258() = 0;
	virtual void _260() = 0;
	virtual void _268() = 0;
	virtual void _270() = 0;
	virtual void _278() = 0;
	virtual void _280() = 0;
	virtual void _288() = 0;
	virtual void _290() = 0;
	virtual void _298() = 0;
	virtual void _2A0() = 0;
	virtual void _2A8() = 0;
	virtual void _2B0() = 0;
	virtual void _2B8() = 0;
	virtual void _2C0() = 0;
	virtual void _2C8() = 0;
	virtual void _2D0() = 0;
	virtual void _2D8() = 0;
	virtual void _2E0() = 0;
	virtual void _2E8() = 0;
	virtual void _2F0() = 0;
	virtual void _2F8() = 0;
	virtual void _300() = 0;
	virtual void _308() = 0;
	virtual void _310() = 0;
	virtual void _318() = 0;
	virtual void _320() = 0;
	virtual void _328() = 0;
	virtual void _330() = 0;
	virtual void _338() = 0;
	virtual void _340() = 0;
	virtual void _348() = 0;
	virtual void _350() = 0;
	virtual void _358() = 0;
	virtual void _360() = 0;
	virtual void _368() = 0;
	virtual void _370() = 0;
	virtual void _378() = 0;
	virtual void _380() = 0;
	virtual void _388() = 0;
	virtual void _390() = 0;
	virtual void _398() = 0;
	virtual void _3A0() = 0;
	virtual void _3A8() = 0;
	virtual void _3B0() = 0;
	virtual void _3B8() = 0;
	virtual void _3C0() = 0;
	virtual void _3C8() = 0;
	virtual void _3D0() = 0;
	virtual void _3D8() = 0;
	virtual void _3E0() = 0;
	virtual void _3E8() = 0;
	virtual void _3F0() = 0;
	virtual void _3F8() = 0;
	virtual void _400() = 0;
	virtual void _408() = 0;
	virtual void _410() = 0;
	virtual void _418() = 0;
	virtual void _420() = 0;
	virtual void _428() = 0;
	virtual void _430() = 0;
	virtual void _438() = 0;
	virtual void _440() = 0;
	virtual void _448() = 0;
	virtual void _450() = 0;
	virtual void _458() = 0;
	virtual void _460() = 0;
	virtual void _468() = 0;
	virtual void _470() = 0;
	virtual void _478() = 0;
	virtual void _480() = 0;
	virtual void _488() = 0;
	virtual void _490() = 0;
	virtual void _498() = 0;
	virtual void _4A0() = 0;
	virtual void _4A8() = 0;
	virtual void _4B0() = 0;
	virtual void _4B8() = 0;
	virtual void _4C0() = 0;
	virtual void _4C8() = 0;
	virtual void _4D0() = 0;
	virtual void _4D8() = 0;
	virtual void _4E0() = 0;
	virtual void _4E8() = 0;
	virtual void _4F0() = 0;
	virtual void _4F8() = 0;
	virtual void _500() = 0;
	virtual void _508() = 0;
	virtual void _510() = 0;
	virtual void _518() = 0;
	virtual void _520() = 0;
	virtual void _528() = 0;
	virtual void _530() = 0;
	virtual void _538() = 0;
	virtual void _540() = 0;
	virtual void _548() = 0;
	virtual void _550() = 0;
	virtual void _558() = 0;
	virtual void _560() = 0;
	virtual void _568() = 0;
	virtual void _570() = 0;
	virtual void _578() = 0;
	virtual void _580() = 0;
	virtual void _588() = 0;
	virtual void _590() = 0;
	virtual void _598() = 0;
	virtual void _5A0() = 0;
	virtual void _5A8() = 0;
	virtual void _5B0() = 0;
	virtual void _5B8() = 0;
	virtual void _5C0() = 0;
	virtual void _5C8() = 0;
	virtual void _5D0() = 0;
	virtual void _5D8() = 0;
	virtual void _5E0() = 0;
	virtual void _5E8() = 0;
	virtual void _5F0() = 0;
	virtual void _5F8() = 0;
	virtual void _600() = 0;
	virtual void _608() = 0;
	virtual void _610() = 0;
	virtual void _618() = 0;
	virtual void _620() = 0;
	virtual void _628() = 0;
	virtual void _630() = 0;
	virtual void _638() = 0;
	virtual void _640() = 0;
	virtual CSubmarineHandling* GetSubHandling() = 0; // 0x648

	enum EGliderState
	{
		NOT_GLIDING,
		DEPLOYING_WINGS,
		GLIDING,
		RETRACTING_WINGS,

		GLIDER_STATE_MAX
	};

	INIT_PAD(CPhysical, 0x0300) float boost_charge;
	/* 0x0304 */ float boost_recharge_speed;
	PAD(0x0304 + 4, 0x033C) hash_t m_ParachuteModelHash;
	EGliderState m_gliderState;
	float m_wingDeployedRatio;
	float m_specialFlightModeTargetRatio;
	PAD(0x0338 + 4, 0x0354) bool m_disableHoverModeFlight;
	PAD(0x0354 + 1, 0x0360) float m_specialFlightModeRatio;
	PAD(0x0360 + 4, 0x0378) bool m_bDoingJump;
	PAD(0x0378 + 1, 0x0394) bool m_bDoHigherJump;
	PAD(0x03A4 + 1, 0x0420) CVehicleDamage m_vehicleDamage;
	PAD(0x0420 + sizeof(CVehicleDamage), 0x0880) CTransmission m_Transmission;
	PAD(0x0880 + sizeof(CTransmission), 0x0960) class CHandlingData* handling_data;
	/* 0x0968 */ CVehicleFlags m_nVehicleFlags;
	PAD(0x0968 + sizeof(CVehicleFlags), 0x0A54) float m_fHomingProjectileDistance;
	PAD(0x0A54 + 4, 0x0AEE) bool m_bAllowHomingMissleLockOnSynced;
	PAD(0x0AEE + 1, 0x0C28) VehicleType vehicle_type;
	PAD(0x0C28 + 4, 0x0C30) CWheel** wheels;
	/* 0x0C38 */ int32_t num_wheels;
	PAD(0x0C38 + 4, 0x0C48) CCarDoor* m_pDoors;
	/* 0x0C50 */ int32_t m_nNumDoors;
	PAD(0x0C50 + 4, 0x0C80) CVehicleWeaponMgr* m_pVehicleWeaponMgr;
	PAD(0x0C80 + 8, 0x0C90) rage::atArray<CVehicleGadget*> m_pVehicleGadgets;
	/* 0x0C98 */ float m_fVerticalVelocityToKnockOffThisBike;
	/* 0x0C9C */ float m_fGravityForWheelIntegrator;

	[[nodiscard]] bool InheritsFromHeli() const noexcept;

	[[nodiscard]] hash_t getModelAsObservedByOthers() const noexcept;
};
static_assert(offsetof(CVehicle, vehicle_type) == 0x0C28);
static_assert(offsetof(CVehicle, m_pVehicleWeaponMgr) == 0x0C80);
static_assert(offsetof(CVehicle, m_pVehicleGadgets) == 0x0C90);

class CAutomobile : public CVehicle
{
	struct CAutomobileFlags
	{
		uint8_t bTaxiLight : 1;
		uint8_t bWaterTight : 1;
		uint8_t bIsBoggedDownInSand : 1;
		uint8_t bFireNetworkCannon : 1;
		uint8_t bBurnoutModeEnabled : 1;
		uint8_t bInBurnout : 1;
		uint8_t bReduceGripModeEnabled : 1;
		uint8_t bPlayerIsRevvingEngineThisFrame : 1;
		uint8_t bLastSpaceAvaliableToChangeBounds : 1;
		uint8_t bDropsMoneyWhenBlownUp : 1;
		uint8_t bHydraulicsBounceRaising : 1;
		uint8_t bHydraulicsBounceLanding : 1;
		uint8_t bHydraulicsJump : 1;
		uint8_t bForceUpdateGroundClearance : 1;
		uint8_t bWheelieModeEnabled : 1;
		uint8_t bInWheelieMode : 1;
		uint8_t bInWheelieModeWheelsUp : 1;
	};

	INIT_PAD(CVehicle, 0x1441 + 0x50 + 0x10) bool m_bExplosionEffectSlick;
	PAD(0x1441 + 1, 0x144C) uint32_t m_uLastHitByExplosionEffectSlick;
	PAD(0x144C + 4, 0x1518) CAutomobileFlags m_nAutomobileFlags;
	PAD(0x1518 + sizeof(CAutomobileFlags), 0x1A10) void* active_parachute;
	/* 0x1A18 */ int32_t parachute_model_unk_index;
	PAD(0x1A18 + 4, 0x1A20) int32_t parachute_model_info_index;
	PAD(0x1A20 + 4, 0x1A3C) uint8_t parachute_texture_variation;
	/* 0x1A3D */ uint8_t can_activate_parachute;
	PAD(0x1A9E, 0x1B00);
};

class CAircraftDamage {};

class CPlane : public CAutomobile
{
public:
	/* 0x1B00 */ float turbulence_multiplier;
	/* 0x1B04 */ float wind_multiplier;
	PAD(0x1B04 + 4, 0x1DD0) CAircraftDamage m_aircraftDamage;
};
static_assert(offsetof(CPlane, turbulence_multiplier) == 0x1B00);
static_assert(offsetof(CPlane, m_aircraftDamage) == 0x1DD0);

class CRotaryWingAircraft : public CAutomobile
{
public:
	/* 0x1B00 */ float main_rotor_health;
	float tail_rotor_health;
	float tail_boom_health;
};

class CHeli : public CRotaryWingAircraft
{
};

class CBoat : public CVehicle
{
};
#pragma pack(pop)

class CTrain : public CVehicle
{
	INIT_PAD(CVehicle, 0x14C0 + 0x50 + 0x10) float m_desiredCruiseSpeed;
	/* 0x14C4 */ float m_trackForwardSpeed;
	/* 0x14C8 */ float m_trackPosFront;
	/* 0x14CC */ float m_trackForwardAccPortion;
	/* 0x14D0 */ float m_frontBackSwingAngPrev;
	/* 0x14D4 */ float m_trackSteepness;
	/* 0x14D8 */ float m_wheelSquealIntensity;
	/* 0x14DC */ uint32_t m_currentNode;
	/* 0x14E0 */ uint32_t m_oldCurrentNode;
	/* 0x14E8 */ rage::fwRegdRef<CTrain> m_pEngine;
	/* 0x14F0 */ rage::fwRegdRef<CTrain> linked_forward;
	/* 0x14F8 */ rage::fwRegdRef<CTrain> linked_backward;
	PAD(0x1500, 0x150F) int8_t m_trackIndex;

	[[nodiscard]] bool isForwardLinkedTo(CTrain* target)
	{
		for (CTrain* t = linked_forward; t != nullptr; t = t->linked_forward)
		{
			if (t == target)
			{
				return true;
			}
		}
		return false;
	}

	[[nodiscard]] bool isBackwardLinkedTo(CTrain* target)
	{
		for (CTrain* t = linked_backward; t != nullptr; t = t->linked_backward)
		{
			if (t == target)
			{
				return true;
			}
		}
		return false;
	}
};
static_assert(offsetof(CTrain, m_pEngine) == 0x1548);
static_assert(offsetof(CTrain, linked_forward) == 0x1550);
static_assert(offsetof(CTrain, linked_backward) == 0x1558);

class CTrainTrack
{
	PAD(0, 0x3C) uint32_t m_iLastTrainReportingTimeMS;
	bool m_bTrainActive;
	PAD(0x41, 0x250);
};
static_assert(sizeof(CTrainTrack) == 0x250);
