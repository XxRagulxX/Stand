#pragma once

#include "Game/eExplosionTag.hpp"
#include "Game/typedecl.hpp"

#define DEFAULT_EXPLOSION_SCALE 1.0f

struct CExplosionManager
{
	class CExplosionArgs
	{
	public:
		CExplosionArgs() = default;

		CExplosionArgs(eExplosionTag explosionTag, const Stand::v3& explosionPosition)
			: m_explosionTag(explosionTag), m_explosionPosition(explosionPosition)
		{
		}

		eExplosionTag m_explosionTag{};
		char pad_0x04[0x0C]{};
		Stand::v3 m_explosionPosition{};
		CEntity* m_pExplodingEntity = nullptr;
		CEntity* m_pEntExplosionOwner = nullptr;
		CEntity* m_pEntIgnoreDamage = nullptr;
		uint32_t m_activationDelay = 0;
		float m_sizeScale = DEFAULT_EXPLOSION_SCALE;
		uint32_t m_camShakeNameHash = 0;
		float m_fCamShake = -1.0f;
		float m_fCamShakeRollOffScaling = -1.0f;
		bool m_bMakeSound = true;
		bool m_bNoFx = false;
		bool m_bInAir = false;
		char pad_0x4F[0x11]{};
		CEntity* m_pAttachEntity = nullptr;
		char pad_0x68[0x26]{};
		bool m_bIsLocalOnly = false;
		bool m_bNoDamage = false;
		bool m_bAttachedToVehicle = false;
		bool m_bDetonatingOtherPlayersExplosive = false;
		bool m_bDisableDamagingOwner = false;
	};
};
