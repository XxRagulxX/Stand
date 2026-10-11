#pragma once

#include <cstdint>

namespace rage
{
	using atHashValue = uint32_t;

	struct Vector2
	{
		float x = 0.0f, y = 0.0f;

		[[nodiscard]] bool isInBounds() const noexcept { return x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f; }
	};

	struct Vector3
	{
		float x, y, z, w;
	};
	static_assert(sizeof(Vector3) == 0x10);

	class fwExtensionList;
	class fwExtension;
	struct phArchetypeDamp;

	class netObject;
	class netObjectMgrBase;
}

class CBaseModelInfo;
class CPhysical;
class CObject;
class CPed;
class CVehicle;
class CPlayerInfo;
class CPedHeadBlendData;

using compactplayer_t = uint8_t;

struct CCustomShaderEffectBaseType;
struct CVehicleModelInfoData;
struct CVehicleLayoutInfo;
