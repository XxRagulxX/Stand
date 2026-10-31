#pragma once

#include "Game/gta_fwddecl.hpp"

#include "Game/datBase.hpp"
#include "Game/gta_extensible.hpp"
#include "Game/vector.hpp"
#include "Util/hashtype.hpp"

#include <soup/structing.hpp>

namespace rage
{
	class fwDynamicArchetypeComponent
	{
	public:
		rage::Vec4V m_AIAvoidancePoint;
		rage::atHashString m_expressionHash;
		phArchetypeDamp* m_pPhysicsArch;
		int16_t m_clipDictionaryIndex;
		int16_t m_blendShapeFileIndex;
		int16_t m_expressionDictionaryIndex;
		uint16_t m_physicsArchRefs;
		int8_t m_poseMatcherFileIndex;
		int8_t m_poseMatcherProneFileIndex;
		int16_t m_creatureMetadataFileIndex : 14;
		uint16_t m_bAutoStartAnim : 1;
		uint16_t m_bHasPhysicsInDrawable : 1;
	};
	static_assert(offsetof(fwDynamicArchetypeComponent, m_pPhysicsArch) == 0x18);

#pragma pack(push, 1)
	class fwArchetype : public datBase
	{
	public:
		enum DrawableType
		{
			DT_UNINITIALIZED,
			DT_FRAGMENT,
			DT_DRAWABLE,
			DT_DRAWABLEDICTIONARY,
			DT_ASSETLESS,
			DT_COUNT
		};

		virtual ~fwArchetype() = default;

		fwExtensionList extension_list;
		hash_t hash;
		PAD(0x18 + 4, 0x50) uint32_t flags;
		PAD(0x54, 0x58) fwDynamicArchetypeComponent* m_DynamicComponent;
		uint8_t m_DrawableType;
		PAD(0x60 + 1, 0x6A) uint16_t m_assignedStreamingSlot;
		PAD(0x6A + 2, 0x70);

		[[nodiscard]] uint32_t getAttribute() const noexcept
		{
			return (flags >> 26) & 0b11111;
		}

		[[nodiscard]] bool GetHasBoundInDrawable() const noexcept
		{
			return m_DrawableType == DT_DRAWABLE
				&& m_DynamicComponent
				&& m_DynamicComponent->m_bHasPhysicsInDrawable
				;
		}
	};
	static_assert(sizeof(fwArchetype) == 0x70);
#pragma pack(pop)
}

enum ModelInfoType : uint8_t
{
	MI_TYPE_NONE,
	MI_TYPE_BASE,
	MI_TYPE_MLO,
	MI_TYPE_TIME,
	MI_TYPE_WEAPON,
	MI_TYPE_VEHICLE,
	MI_TYPE_PED,
	MI_TYPE_COMPOSITE,
};

#pragma pack(push, 1)
class CBaseModelInfo : public rage::fwArchetype
{
public:
	virtual ~CBaseModelInfo() = default;
	virtual void Init() = 0;

	CCustomShaderEffectBaseType* custom_shader_effect;
	PAD(0x70 + 8, 0x9D) uint8_t m_type : 5;
	PAD(0x9D + 1, 0xB0);

	[[nodiscard]] uint8_t GetModelType() const noexcept
	{
		return m_type;
	}
};
static_assert(sizeof(CBaseModelInfo) == 0xB0);
#pragma pack(pop)

