#pragma once

#include <soup/Matrix.hpp>

#include "Game/struct_base.hpp"
#include "Game/CBaseModelInfo.hpp"
#include "Game/gta_extensible.hpp"
#include "Game/typedecl.hpp"

#pragma pack(push, 1)
namespace rage
{
	class fwAttachmentEntityExtension
	{
	public:
		fwEntity* m_pAttachParent;
		fwEntity* m_pAttachChild;
		fwEntity* m_pAttachSibling;
		fwEntity* m_pThisEntity;
	};
	static_assert(sizeof(fwAttachmentEntityExtension) == 0x20);

	class fwDynamicEntityComponent
	{
	public:
		rage::Vector3 m_vAnimatedVelocity;
		rage::Vector3 m_vecPrevPos;
		class fwEntityDesc* m_pEntityDesc;
		class crSkeleton* m_pSkeleton;
		class crCreature* m_pCreature;
		class grbTargetManager* m_pTargetManager;
		class fwAnimDirector* m_pAnimDirector;
		fwAttachmentEntityExtension* m_pAttachmentEntityExtension;
		phCollider* m_pCollider;
		fwEntity* m_pNoCollisionEntity;
		float m_fAnimatedAngularVelocity;
		float m_fForceAddToBoundRadius;
		uint8_t m_nNoCollisionFlags;
	};
	static_assert(offsetof(fwDynamicEntityComponent, m_nNoCollisionFlags) == 0x68);

	class fwEntity : public fwExtensibleBase
	{
	public:
		enum Flags
		{
			IS_FIXED = 0x20000
		};

		CBaseModelInfo* archetype;
		uint8_t type;
		uint8_t protected_flags;
		uint16_t render_flags;
		uint32_t base_flags;
		class phInst* ph_inst;
		PAD(0x30 + 8, 0x48) class fwDrawData* draw_handler;
		fwDynamicEntityComponent* m_DynamicEntityComponent;
		PAD(0x50 + 8, 0x60) soup::Matrix matrix;

		[[nodiscard]] const soup::Matrix& GetMatrix() const
		{
			return matrix;
		}

		[[nodiscard]] fwEntity* GetAttachedTo();

		[[nodiscard]] const fwAttachmentEntityExtension* GetAttachmentExtension() const
		{
			if (m_DynamicEntityComponent)
			{
				return m_DynamicEntityComponent->m_pAttachmentEntityExtension;
			}
			return nullptr;
		}
	};
	static_assert(sizeof(fwEntity) == 0x60 + sizeof(soup::Matrix));
}

struct CEntityFlags
{
	uint32_t bRenderDamaged : 1;
	uint32_t bUseAltFadeDistance : 1;
	uint32_t bIsProcObject : 1;
	uint32_t bIsCompEntityProcObject : 1;
	uint32_t bIsFloatingProcObject : 1;
	uint32_t bLightObject : 1;
	uint32_t bIsFrag : 1;
	uint32_t bInMloRoom : 1;
	uint32_t bCreatedProcObjects : 1;
	uint32_t bPossiblyTouchesWater : 1;
	uint32_t bWillSpawnPeds : 1;
	uint32_t bAlreadyInAudioList : 2;
	uint32_t bLightsIgnoreDayNightSetting : 1;
	uint32_t bLightsCanCastStaticShadows : 1;
	uint32_t bLightsCanCastDynamicShadows : 1;
	uint32_t bTimedEntityDelayedState : 1;
	uint32_t bIsOnFire : 1;
	uint32_t bHasExploded : 1;
	uint32_t bHasSpawnPoints : 1;
	uint32_t bAddtoMotionBlurMask : 1;
	uint32_t bIsEntityProcObject : 1;
	uint32_t bAlwaysPreRender : 1;
	uint32_t trafficLightOverride : 2;
	uint32_t bNeverDummy : 1;
	uint32_t bFoundInPvs : 1;
	uint32_t bPreRenderedThisFrame : 1;
	uint32_t bUpdatingThroughAnimQueue : 1;
	uint32_t bUseOcclusionQuery : 1;
	uint32_t bUseMaxDistanceForWaterReflection : 1;
	uint32_t bCloseToCamera : 1;
};
static_assert(sizeof(CEntityFlags) == sizeof(uint32_t));

class CEntity : public rage::fwEntity
{
	INIT_PAD(rage::fwEntity, 0xB0) uint8_t visibility_flags;
	PAD(0xB0 + 1, 0xC0) CEntityFlags m_nFlags;
	PAD(0xC4, 0xD0);
};
static_assert(sizeof(CEntity) == 0xD0);

enum
{
	STATUS_PLAYER,
	STATUS_PHYSICS,
	STATUS_ABANDONED,
	STATUS_WRECKED,
	STATUS_PLAYER_DISABLED,
	STATUS_OUT_OF_CONTROL
};

struct CDynamicEntityFlags
{
	uint16_t nStatus : 3;
	uint16_t bFrozenByInterior : 1;
	uint16_t bFrozen : 1;
	uint16_t bCheckedForDead : 1;
	uint16_t bIsGolfBall : 1;
	uint16_t bForcePrePhysicsAnimUpdate : 1;
	uint16_t bIsBreakableGlass : 1;
	uint16_t bIsOutOfMap : 1;
	uint16_t bOverridePhysicsBounds : 1;
	uint16_t bHasMovedSinceLastPreRender : 1;
	uint16_t bUseExtendedBoundingBox : 1;
	uint16_t bIsStraddlingPortal : 1;
	uint16_t nPopType : 4;
	uint16_t nPopTypePrev : 4;
	uint16_t bReplayWarpedThisFrame : 1;
};

class CPortalTrackerBase
{
public:
	virtual ~CPortalTrackerBase() = default;
	rage::fwEntity* entity;
};
static_assert(sizeof(CPortalTrackerBase) == 0x10);

class CPortalTracker : public CPortalTrackerBase
{
};

class CDynamicEntity : public CEntity
{
public:
	rage::netObject* m_net_object;
	CDynamicEntityFlags m_nDEflags;
	uint16_t m_randomSeed;
	uint8_t m_FrameCountLastVisible;
	uint8_t m_portalStraddlingContainerIndex;
	PAD(0xE0, 0xF0) CPortalTracker portal_tracker;
};

class CPhysical : public CDynamicEntity
{
public:
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
	float max_health;

	rage::fwEntity* GetAttachParentForced() const
	{
		if (const rage::fwAttachmentEntityExtension* extension = GetAttachmentExtension())
		{
			return extension->m_pAttachParent;
		}
		return nullptr;
	}

	rage::fwEntity* GetChildAttachment() const
	{
		if (const rage::fwAttachmentEntityExtension* extension = GetAttachmentExtension())
		{
			return extension->m_pAttachChild;
		}
		return nullptr;
	}

	rage::fwEntity* GetSiblingAttachment() const
	{
		if (const rage::fwAttachmentEntityExtension* extension = GetAttachmentExtension())
		{
			return extension->m_pAttachSibling;
		}
		return nullptr;
	}
};
static_assert(sizeof(CPhysical) == 0x284 + 4);
static_assert(offsetof(CPhysical, relationship_hash) == 0x194);
#pragma pack(pop)
