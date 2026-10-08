#pragma once

#include "Game/struct_base.hpp"

#include <soup/BigBitset.hpp>

#include "Game/fwRefAwareBase.hpp"
#include "Game/fwRegdRef.hpp"
#include "Game/gta_entity.hpp"

#pragma pack(push, 1)
class CPedFactory
{
public:
	virtual ~CPedFactory() = default;
	virtual void _0x08() = 0;
	virtual void _0x10() = 0;
	virtual void _0x18() = 0;
	virtual void _0x20() = 0;
	virtual void _0x28() = 0;
	virtual void deletePed(CPed*, bool) = 0;

	CPed* m_local_ped;
};
static_assert(sizeof(CPedFactory) == 0x10);

class CInventoryListener : public rage::fwRefAwareBase
{
};

class audWeaponInventoryListener : public CInventoryListener
{
};

struct CPedConfigFlags
{
	PAD(0, 0x08) soup::BigBitset<60> m_Flags;
};

struct CPedResetFlags
{
	PAD(0, 0x20) soup::BigBitset<60> m_Flags;
};

class CPedWeaponManager;

class CPed : public CPhysical
{
public:
	INIT_PAD(CPhysical, 0xD10) CVehicle* current_or_last_vehicle;
	PAD(0xD10 + 8, 0xDC0) audWeaponInventoryListener weapon_inventory_listener;
	PAD(0xDC0 + sizeof(audWeaponInventoryListener), 0xE32) bool is_in_vehicle;
	PAD(0xE32 + 1, 0x10A0) class CPedIntelligence* intelligence;
	CPlayerInfo* player_info;
	class CPedInventory* inventory;
	CPedWeaponManager* weapon_manager;
	PAD(0x10B8 + 8, 0x13C8) class CPedClothCollision* cloth_collision;
	PAD(0x13C8 + 8, 0x143C) CPedConfigFlags m_PedConfigFlags;
	CPedResetFlags m_PedResetFlags;
	PAD(0x1480 + sizeof(CPedResetFlags), 0x150C) float m_armor;
	PAD(0x150C + 4, 0x1530) rage::fwRegdRef<CVehicle> m_pMyVehicle;
	PAD(0x1530 + 8, 0x1964) uint8_t m_uStickyCount;
	uint8_t m_uStickToPedProjCount;
	uint8_t m_uFlareGunProjCount;

	[[nodiscard]] bool IsAPlayerPed() const noexcept
	{
		return player_info != nullptr;
	}

	[[nodiscard]] CVehicle* getVehicle(bool include_last = false) const noexcept
	{
		if (include_last || is_in_vehicle)
		{
			return current_or_last_vehicle;
		}
		return nullptr;
	}
};
static_assert(sizeof(CPed) == 0x1966 + 1);
static_assert(offsetof(CPed, player_info) == 0x10A8);

class CWeapon : public rage::fwRefAwareBase
{
	INIT_PAD(rage::fwRefAwareBase, 0x20) rage::Vec3V m_muzzlePos;
	PAD(0x30, 0x58) rage::fwRegdRef<CDynamicEntity> m_pDrawableEntity;
	PAD(0x60, 0x178) rage::fwRegdRef<CEntity> m_pMuzzleEntity;
};
static_assert(offsetof(CWeapon, m_muzzlePos) == 0x20);
static_assert(offsetof(CWeapon, m_pDrawableEntity) == 0x58);
static_assert(offsetof(CWeapon, m_pMuzzleEntity) == 0x178);

class CItemInfo : public rage::fwRefAwareBase
{
public:
	hash_t name_hash;
	hash_t model_hash;
	hash_t audio_hash;
	hash_t slot_hash;
};
static_assert(sizeof(CItemInfo) == 0x20);

class CWeaponInfo : public CItemInfo
{
	INIT_PAD(CItemInfo, 0x060) class CAmmoInfo* m_AmmoInfo;
	PAD(0x060 + 8, 0x074) float m_AccuracySpread;
	float m_AccurateModeAccuracyModifier;
	float m_RunAndGunAccuracyModifier;
	float m_RunAndGunAccuracyMinOverride;
	float m_RecoilAccuracyMax;
	float m_RecoilErrorTime;
	float m_RecoilRecoveryRate;
	float m_RecoilAccuracyToAllowHeadShotAI;
	float m_MinHeadShotDistanceAI;
	float m_MaxHeadShotDistanceAI;
	float m_HeadShotDamageModifierAI;
	float m_RecoilAccuracyToAllowHeadShotPlayer;
	float m_MinHeadShotDistancePlayer;
	float m_MaxHeadShotDistancePlayer;
	float m_HeadShotDamageModifierPlayer;
	float m_Damage;
	float m_DamageTime;
	float m_DamageTimeInVehicle;
	float m_DamageTimeInVehicleHeadShot;
	PAD(0x0bc + 4, 0x0f8) float m_ForceMaxStrengthMult;
	float m_ForceFalloffRangeStart;
	float m_ForceFalloffRangeEnd;
	float m_ForceFalloffMin;
	float m_ProjectileForce;
	float m_FragImpulse;
	float m_Penetration;
	float m_VerticalLaunchAdjustment;
	float m_DropForwardVelocity;
	float m_Speed;
	unsigned m_BulletsInBatch;
	float m_BatchSpread;
	float m_ReloadTimeMP;
	float m_ReloadTimeSP;
	float m_VehicleReloadTime;
	float m_AnimReloadRate;
	int m_BulletsPerAnimLoop;
	float m_TimeBetweenShots;
	float m_TimeLeftBetweenShotsWhereShouldFireIsCached;
	float m_SpinUpTime;
	float m_SpinTime;
	float m_SpinDownTime;
	float m_AlternateWaitTime;
	float m_BulletBendingNearRadius;
	float m_BulletBendingFarRadius;
	float m_BulletBendingZoomedRadius;
	float m_FirstPersonBulletBendingNearRadius;
	float m_FirstPersonBulletBendingFarRadius;
	float m_FirstPersonBulletBendingZoomedRadius;
	PAD(0x168 + 4, 0x250) int m_InitialRumbleDuration;
	float m_InitialRumbleIntensity;
	float m_InitialRumbleIntensityTrigger;
	int m_RumbleDuration;
	float m_RumbleIntensity;
	float m_RumbleIntensityTrigger;
	float m_RumbleDamageIntensity;
	int m_InitialRumbleDurationFps;
	float m_InitialRumbleIntensityFps;
	int m_RumbleDurationFps;
	float m_RumbleIntensityFps;
	float m_NetworkPlayerDamageModifier;
	float m_NetworkPedDamageModifier;
	float m_NetworkHeadShotPlayerDamageModifier;
	float m_LockOnRange;
	float m_WeaponRange;
	float m_AiSoundRange;
	float m_AiPotentialBlastEventRange;
	float m_DamageFallOffRangeMin;
	float m_DamageFallOffRangeMax;
	PAD(0x29c + 4, 0x2a8) float m_DamageFallOffModifier;
	PAD(0x2a8 + 4, 0x2e4) hash_t m_RecoilShakeHash;
	hash_t m_RecoilShakeHashFirstPerson;
	hash_t m_AccuracyOffsetShakeHash;
	unsigned int m_MinTimeBetweenRecoilShakes;
	float m_RecoilShakeAmplitude;
	float m_ExplosionShakeAmplitude;
	PAD(0x2f8 + 4, 0x8fc) float m_BulletDirectionOffsetInDegrees;
	float m_BulletDirectionPitchOffset;
	float m_BulletDirectionPitchHomingOffset;
};
static_assert(sizeof(CWeaponInfo) == 0x904 + 4);

class CAmmoInfo : public CItemInfo
{
public:
	int m_AmmoMax;
	int m_AmmoMax50;
	int m_AmmoMax100;
	int m_AmmoMaxMP;
	int m_AmmoMax50MP;
	int m_AmmoMax100MP;
};
static_assert(sizeof(CAmmoInfo) == 0x34 + 4);

class CPedWeaponSelector
{
public:
	hash_t selected_weapon_hash;
	PAD(0x04, 0x08) CWeaponInfo* selected_weapon_info;
	PAD(0x10, 0x40);
};
static_assert(sizeof(CPedWeaponSelector) == 0x40);

class CPedEquippedWeapon
{
	CPed* ped;
	bool weaponAnimsRequested;
	bool weaponAnimsStreamed;
	PAD(0x0A, 0x10) CWeaponInfo* equippedWeaponInfo;
	CWeaponInfo* equippedVehicleWeaponInfo;
	rage::fwRegdRef<CObject> object;
	rage::fwRegdRef<CObject> secondObject;
	rage::fwRegdRef<CWeapon> unarmedWeapon;
	rage::fwRegdRef<CWeapon> unarmedKungFuWeapon;
	rage::fwRegdRef<CWeapon> weaponNoModel;
};
static_assert(sizeof(CPedEquippedWeapon) == 0x40 + 8);

class CPedWeaponManager : public CInventoryListener
{
public:
	CPed* ped;
	CPedWeaponSelector selector;
	CPedEquippedWeapon equippedWeapon;

	[[nodiscard]] const CWeapon* GetEquippedWeapon() const;
};
static_assert(sizeof(CPedWeaponManager) == 0x58 + sizeof(CPedEquippedWeapon));

struct CWeaponModelInfo : public CBaseModelInfo
{
	int32_t m_nBoneIndices[36];
};
#pragma pack(pop)
