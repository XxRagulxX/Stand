#pragma once

#include <cstdint>
#include <Windows.h>

#include "Game/gta_fwddecl.hpp"

namespace Stand
{
	struct pointers
	{
		static inline CPedFactory** ped_factory{};
		static inline bool* is_session_started{};
		static inline CReplayInterfaceGame** replay_interface{};
		static inline uint32_t* extension_id_script_guid{};
		static inline uint32_t* entity_extension_id_head_blend_data{};
		static inline rage::fwPool<rage::fwScriptGuid>** script_guid_pool{};
		static inline CNetworkObjectMgr** network_object_mgr{};
		static inline CNetworkPlayerMgr** network_player_mgr{};
		static inline CNetworkSession** network_session{};

		static inline int (__fastcall* CTheScripts_GetGUIDFromEntity)(const rage::fwEntity&){};
		static inline bool (__fastcall* CPedFactory_DestroyPed)(CPedFactory*, CPed*, bool){};
		static inline void (__fastcall* vehicle_commands__DeleteScriptVehicle)(CVehicle*){};
		static inline void (__fastcall* CObjectPopulation_DestroyObject)(CObject*, bool){};
		static inline void (__fastcall* CExplosionEvent_Trigger)(void* explosionArgs, CProjectile* pProjectile){};
		static inline void (__fastcall* CGiveControlEvent_Trigger)(const rage::netPlayer* player, rage::netObject* pObject, int migrationType){};
		static inline void (__fastcall* CRagdollRequestEvent_Trigger)(uint16_t pedID){};
		static inline void (__fastcall* CWeaponDamageEvent_Trigger)(
			CEntity* pParentEntity,
			CPed* pHitEntity,
			const void* worldHitPosition,
			int hitComponent,
			bool bOverride,
			uint32_t weaponHash,
			float weaponDamage,
			int tyreIndex,
			int suspensionIndex,
			uint32_t damageFlags,
			uint32_t actionResultId,
			uint16_t meleeId,
			uint32_t nForcedReactionDefinitionId,
			bool hitEntityWeapon,
			bool hitWeaponAmmoAttachment,
			bool silenced,
			bool firstBullet,
			const void* hitDirection
		){};
		static inline void (__fastcall* CVehicleDamage_BreakOffWheel)(CVehicleDamage* _this, int wheelIndex, float ptfxProbability, float deleteProbability, float burstTyreProbability, bool dueToExplosion, bool bNetworkCheck){};
		static inline void (__fastcall* rage_fwEntity_GetAttachedTo)(void){};

		static inline HWND hwnd{};
		static inline int32_t* CLoadingScreens_ms_Context{};

		static inline struct CMultiplayerChat** chat_box{};
	};
}
